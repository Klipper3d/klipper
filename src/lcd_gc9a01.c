// Commands for sending messages to a GC9A01 circular TFT display driver
//
// Copyright (C) 2025  Eug Krashtan <eug.krashtan@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_MACH_AVR
#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_from_us
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_SHUTDOWN
#include "spicmds.h" // spidev_transfer
#include "generic/armcm_timer.h" // udelay

#define GC9A01_LINE_SIZE 240

struct gc9a01 {
    struct spidev_s *spi;
    struct gpio_out dc, reset;
    uint32_t last_cmd_time, cmd_wait_ticks;
    uint16_t rotation;
};


/****************************************************************
 * GC9A01 Commands
 ****************************************************************/

// Common GC9A01 commands
#define GC9A01_SLPOUT   0x11  // Sleep Out
#define GC9A01_INVON    0x21  // Display Inversion On
#define GC9A01_DISPON   0x29  // Display On
#define GC9A01_CASET    0x2A  // Column Address Set
#define GC9A01_RASET    0x2B  // Row Address Set
#define GC9A01_RAMWR    0x2C  // Memory Write
#define GC9A01_MADCTL   0x36  // Memory Data Access Control
#define GC9A01_COLMOD   0x3A  // Interface Pixel Format

// Extended/undocumented registers
#define GC9A01_INTER_REG_EN1  0xFE  // Inter Register Enable 1
#define GC9A01_INTER_REG_EN2  0xEF  // Inter Register Enable 2
#define GC9A01_INREGEN2       0xEB  // Inter Register Enable 2 config
#define GC9A01_FRAMERATE      0x90  // Frame Rate Control
#define GC9A01_DISPLAYFUNC    0xB6  // Display Function Control
#define GC9A01_POWERCTL1      0xC3  // Power Control 1
#define GC9A01_POWERCTL2      0xC4  // Power Control 2
#define GC9A01_POWERCTL3      0xC9  // Power Control 3
#define GC9A01_POWERCTL4      0xBE  // Power Control 4
#define GC9A01_VCOCTL         0xE1  // VCO Control
#define GC9A01_VCOMCTL        0xDF  // VCOM Control
#define GC9A01_DISPOUTCTL1    0xBC  // Display Output Control 1
#define GC9A01_DISPOUTCTL2    0xBD  // Display Output Control 2
#define GC9A01_GATECTL        0xFF  // Gate Control
#define GC9A01_GAMMA1         0xF0  // Set Gamma 1
#define GC9A01_GAMMA2         0xF1  // Set Gamma 2
#define GC9A01_GAMMA3         0xF2  // Set Gamma 3
#define GC9A01_GAMMA4         0xF3  // Set Gamma 4

// Extended inter-register commands
#define GC9A01_IREG_0x84      0x84
#define GC9A01_IREG_0x85      0x85
#define GC9A01_IREG_0x86      0x86
#define GC9A01_IREG_0x87      0x87
#define GC9A01_IREG_0x88      0x88
#define GC9A01_IREG_0x89      0x89
#define GC9A01_IREG_0x8A      0x8A
#define GC9A01_IREG_0x8B      0x8B
#define GC9A01_IREG_0x8C      0x8C
#define GC9A01_IREG_0x8D      0x8D
#define GC9A01_IREG_0x8E      0x8E
#define GC9A01_IREG_0x8F      0x8F

static uint16_t palette[16] = {
    0x0000, // 0: RGB(  0,  0,  0) #000000 Black
    0xF800, // 1: RGB(255,  0,  0) #FF0000 Red
    0x07E0, // 2: RGB(  0,255,  0) #00FF00 Green
    0xFFE0, // 3: RGB(255,255,  0) #FFFF00 Yellow
    0x001F, // 4: RGB(  0,  0,255) #0000FF Blue
    0xF81F, // 5: RGB(255,  0,255) #FF00FF Magenta
    0x07FF, // 6: RGB(  0,255,255) #00FFFF Cyan
    0xFFFF, // 7: RGB(255,255,255) #FFFFFF White
    0x8410, // 8: RGB(132,130,132) #848284 Dark Gray
    0x4208, // 9: RGB( 66, 65, 66) #424142 Very Dark Gray
    0xA514, // 10: RGB(165,162,165) #A5A2A5 Medium Gray
    0x6318, // 11: RGB( 99, 97,198) #6361C6 Blue-Gray
    0xC618, // 12: RGB(198,195,198) #C6C3C6 Light Gray
    0x39E7, // 13: RGB( 57, 60, 57) #393C39 Dark Green-Gray
    0xE73C, // 14: RGB(231,231,231) #E7E7E7 Very Light Gray
    0xFFFF  // 15: RGB(255,255,255) #FFFFFF White
};

/****************************************************************
 * Timing functions
 ****************************************************************/

void __attribute__((weak))
// ToDo: use existing udelay function from generic/armcm_timer.c
udelay(uint32_t us)
{
    if (CONFIG_MACH_AVR)
        // Slower MCUs don't require a delay
        return;

    uint32_t end = timer_read_time() + timer_from_us(us);
    while (timer_is_before(timer_read_time(), end))
        irq_poll();
}


/****************************************************************
 * Transmit functions
 ****************************************************************/

// Send a command byte to the GC9A01
static void
gc9a01_send_cmd(struct gc9a01 *g, uint8_t cmd)
{
    gpio_out_write(g->dc, 0);  // Command mode
    spidev_transfer(g->spi, 1, sizeof(cmd), &cmd);
}

// Send data bytes to the GC9A01
static void
gc9a01_send_data(struct gc9a01 *g, uint8_t len, uint8_t *data)
{
    gpio_out_write(g->dc, 1);  // Data mode
    spidev_transfer(g->spi, 1, len, data);
}

// Send a command followed by data
static void
gc9a01_cmd_with_data(struct gc9a01 *g, uint8_t cmd, uint8_t len, uint8_t *data)
{
    gc9a01_send_cmd(g, cmd);
    if (len > 0)
        gc9a01_send_data(g, len, data);
}

// Hardware reset sequence
static void
gc9a01_hw_reset(struct gc9a01 *g)
{
    gpio_out_write(g->reset, 1);
    udelay(10);
    gpio_out_write(g->reset, 0);
    udelay(10);
    gpio_out_write(g->reset, 1);
    udelay(120000);  // 120ms
}


/****************************************************************
 * Interface
 ****************************************************************/

void
command_config_gc9a01(uint32_t *args)
{
    struct gc9a01 *g = oid_alloc(args[0], command_config_gc9a01, sizeof(*g));

    g->spi = spidev_oid_lookup(args[1]);
    g->dc = gpio_out_setup(args[2], 0);
    g->reset = gpio_out_setup(args[3], 0);
    g->rotation = args[4];
    g->cmd_wait_ticks = args[5];

    // Hardware reset
    gc9a01_hw_reset(g);

    // Initial Sequence from GC9A01 code example
    gc9a01_send_cmd(g, GC9A01_INTER_REG_EN2);

    uint8_t eb_data[] = {0x14};
    gc9a01_cmd_with_data(g, GC9A01_INREGEN2, 1, eb_data);

    gc9a01_send_cmd(g, GC9A01_INTER_REG_EN1);
    gc9a01_send_cmd(g, GC9A01_INTER_REG_EN2);

    gc9a01_cmd_with_data(g, GC9A01_INREGEN2, 1, eb_data);

    uint8_t param84[] = {0x40};
    gc9a01_cmd_with_data(g, GC9A01_IREG_0x84, 1, param84);

    uint8_t param85[] = {0xFF};
    gc9a01_cmd_with_data(g, GC9A01_IREG_0x85, 1, param85);

    uint8_t param86[] = {0xFF};
    gc9a01_cmd_with_data(g, GC9A01_IREG_0x86, 1, param86);

    uint8_t param87[] = {0xFF};
    gc9a01_cmd_with_data(g, GC9A01_IREG_0x87, 1, param87);

    uint8_t param88[] = {0x0A};
    gc9a01_cmd_with_data(g, GC9A01_IREG_0x88, 1, param88);

    uint8_t param89[] = {0x21};
    gc9a01_cmd_with_data(g, GC9A01_IREG_0x89, 1, param89);

    uint8_t param8A[] = {0x00};
    gc9a01_cmd_with_data(g, GC9A01_IREG_0x8A, 1, param8A);

    uint8_t param8B[] = {0x80};
    gc9a01_cmd_with_data(g, GC9A01_IREG_0x8B, 1, param8B);

    uint8_t param8C[] = {0x01};
    gc9a01_cmd_with_data(g, GC9A01_IREG_0x8C, 1, param8C);

    uint8_t param8D[] = {0x01};
    gc9a01_cmd_with_data(g, GC9A01_IREG_0x8D, 1, param8D);

    uint8_t param8E[] = {0xFF};
    gc9a01_cmd_with_data(g, GC9A01_IREG_0x8E, 1, param8E);

    uint8_t param8F[] = {0xFF};
    gc9a01_cmd_with_data(g, GC9A01_IREG_0x8F, 1, param8F);

    // Display function control
    uint8_t b6_data[] = {0x00, 0x20};
    gc9a01_cmd_with_data(g, GC9A01_DISPLAYFUNC, 2, b6_data);

    // Pixel Format Set - 16-bit color (0x55)
    uint8_t colmod_data[] = {0x55};
    gc9a01_cmd_with_data(g, GC9A01_COLMOD, 1, colmod_data);

    // Memory Data Access Control - apply rotation
    uint8_t madctl_value = 0x08;  // BGR bit
    if (g->rotation == 90)
        madctl_value |= 0x60;
    else if (g->rotation == 180)
        madctl_value |= 0xC0;
    else if (g->rotation == 270)
        madctl_value |= 0xA0;
    uint8_t madctl_data[] = {madctl_value};
    gc9a01_cmd_with_data(g, GC9A01_MADCTL, 1, madctl_data);

    uint8_t param90[] = {0x08, 0x08, 0x08, 0x08, 0x08};
    gc9a01_cmd_with_data(g, GC9A01_FRAMERATE, 5, param90);

    uint8_t paramBD[] = {0x06};
    gc9a01_cmd_with_data(g, GC9A01_DISPOUTCTL2, 1, paramBD);

    uint8_t paramBC[] = {0x00};
    gc9a01_cmd_with_data(g, GC9A01_DISPOUTCTL1, 1, paramBC);

    uint8_t paramFF[] = {0x60, 0x01, 0x04};
    gc9a01_cmd_with_data(g, GC9A01_GATECTL, 3, paramFF);

    // Power control
    uint8_t paramC3[] = {0x13};
    gc9a01_cmd_with_data(g, GC9A01_POWERCTL1, 1, paramC3);

    uint8_t paramC4[] = {0x13};
    gc9a01_cmd_with_data(g, GC9A01_POWERCTL2, 1, paramC4);

    uint8_t paramC9[] = {0x22};
    gc9a01_cmd_with_data(g, GC9A01_POWERCTL3, 1, paramC9);

    uint8_t paramBE[] = {0x11};
    gc9a01_cmd_with_data(g, GC9A01_POWERCTL4, 1, paramBE);

    uint8_t paramE1[] = {0x10, 0x0E};
    gc9a01_cmd_with_data(g, GC9A01_VCOCTL, 2, paramE1);

    uint8_t paramDF[] = {0x21, 0x0C, 0x02};
    gc9a01_cmd_with_data(g, GC9A01_VCOMCTL, 3, paramDF);

    // Gamma
    uint8_t paramF0[] = {0x45, 0x09, 0x08, 0x08, 0x26, 0x2A};
    gc9a01_cmd_with_data(g, GC9A01_GAMMA1, 6, paramF0);

    uint8_t paramF1[] = {0x43, 0x70, 0x72, 0x36, 0x37, 0x6F};
    gc9a01_cmd_with_data(g, GC9A01_GAMMA2, 6, paramF1);

    uint8_t paramF2[] = {0x45, 0x09, 0x08, 0x08, 0x26, 0x2A};
    gc9a01_cmd_with_data(g, GC9A01_GAMMA3, 6, paramF2);

    uint8_t paramF3[] = {0x43, 0x70, 0x72, 0x36, 0x37, 0x6F};
    gc9a01_cmd_with_data(g, GC9A01_GAMMA4, 6, paramF3);

    uint8_t paramED[] = {0x1B, 0x0B};
    gc9a01_cmd_with_data(g, 0xED, 2, paramED);

    uint8_t paramAE[] = {0x77};
    gc9a01_cmd_with_data(g, 0xAE, 1, paramAE);

    uint8_t paramCD[] = {0x63};
    gc9a01_cmd_with_data(g, 0xCD, 1, paramCD);

    // Frame rate control
    uint8_t param70[] = {0x07, 0x07, 0x04, 0x0E, 0x0F, 0x09, 0x07, 0x08, 0x03};
    gc9a01_cmd_with_data(g, 0x70, 9, param70);

    // Positive gamma correction
    uint8_t param62[] = {0x18, 0x0D, 0x71, 0xED, 0x70, 0x70, 0x18, 0x0F,
                        0x71, 0xEF, 0x70, 0x70};
    gc9a01_cmd_with_data(g, 0x62, 12, param62);

    uint8_t param63[] = {0x18, 0x11, 0x71, 0xF1, 0x70, 0x70, 0x18, 0x13,
                        0x71, 0xF3, 0x70, 0x70};
    gc9a01_cmd_with_data(g, 0x63, 12, param63);

    uint8_t param64[] = {0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07};
    gc9a01_cmd_with_data(g, 0x64, 7, param64);

    uint8_t param66[] = {0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0x00,
                        0x00, 0x00};
    gc9a01_cmd_with_data(g, 0x66, 10, param66);

    uint8_t param67[] = {0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x54, 0x10,
                        0x32, 0x98};
    gc9a01_cmd_with_data(g, 0x67, 10, param67);

    uint8_t param74[] = {0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00};
    gc9a01_cmd_with_data(g, 0x74, 7, param74);

    uint8_t param98[] = {0x3E, 0x07};
    gc9a01_cmd_with_data(g, 0x98, 2, param98);

    // Tearing effect line ON
    gc9a01_send_cmd(g, 0x35);

    // Inversion On
    gc9a01_send_cmd(g, GC9A01_INVON);

    // Sleep Out
    gc9a01_send_cmd(g, GC9A01_SLPOUT);
    udelay(120000);

    // Display On
    gc9a01_send_cmd(g, GC9A01_DISPON);
}
DECL_COMMAND(command_config_gc9a01,
             "config_gc9a01 oid=%c spi_oid=%c dc_pin=%u"
             " reset_pin=%u rotation=%c delay_ticks=%u");

void
command_gc9a01_fill_rect(uint32_t *args)
{
    struct gc9a01 *g = oid_lookup(args[0], command_config_gc9a01);
    uint16_t x0 = args[1];
    uint16_t y0 = args[2];
    uint16_t x1 = args[3];
    uint16_t y1 = args[4];
    uint8_t color_idx = args[5] & 0x0F;
    uint16_t color = palette[color_idx];
    uint32_t pixels = (x1 - x0 + 1) * (y1 - y0 + 1) * 2;
    uint8_t color_high = (color >> 8) & 0xFF;
    uint8_t color_low = color & 0xFF;
    uint8_t line_buf[GC9A01_LINE_SIZE];

    // Set column address
    uint8_t caset_data[] = {
        (x0 >> 8) & 0xFF, x0 & 0xFF,
        (x1 >> 8) & 0xFF, x1 & 0xFF
    };
    gc9a01_cmd_with_data(g, GC9A01_CASET, 4, caset_data);

    // Set row address
    uint8_t raset_data[] = {
        (y0 >> 8) & 0xFF, y0 & 0xFF,
        (y1 >> 8) & 0xFF, y1 & 0xFF
    };
    gc9a01_cmd_with_data(g, GC9A01_RASET, 4, raset_data);

    gc9a01_send_cmd(g, GC9A01_RAMWR);

    // Send buffer
    while (pixels > 0) {
        uint16_t bytes_to_send = (pixels > (GC9A01_LINE_SIZE)) ?
                                  (GC9A01_LINE_SIZE) : pixels;
        // Refresh line buffer with color after each send (RX may overwrite it)
        for (uint16_t i = 0; i < bytes_to_send; i += 2) {
            line_buf[i] = color_high;
            line_buf[i+1] = color_low;
        }
        gc9a01_send_data(g, bytes_to_send, line_buf);
        pixels -= bytes_to_send;
    }
}
DECL_COMMAND(command_gc9a01_fill_rect,
             "gc9a01_fill_rect oid=%c x0=%hu y0=%hu x1=%hu y1=%hu color=%c");

void
command_gc9a01_send_data(uint32_t *args)
{
    struct gc9a01 *g = oid_lookup(args[0], command_config_gc9a01);
    uint8_t len = args[1], *data = command_decode_ptr(args[2]);
    uint8_t out_buf[GC9A01_LINE_SIZE];
    uint16_t out_idx = 0;

    for (uint8_t i = 0; i < len; i++) {
        uint8_t byte = data[i];
        uint8_t idx1 = (byte >> 4) & 0x0F;
        uint8_t idx2 = byte & 0x0F;
        uint16_t color1 = palette[idx1];
        uint16_t color2 = palette[idx2];

        out_buf[out_idx++] = (color1 >> 8) & 0xFF;
        out_buf[out_idx++] = color1 & 0xFF;
        out_buf[out_idx++] = (color2 >> 8) & 0xFF;
        out_buf[out_idx++] = color2 & 0xFF;

        if (out_idx >= GC9A01_LINE_SIZE) {
            gc9a01_send_data(g, out_idx, out_buf);
            out_idx = 0;
        }
    }

    if (out_idx > 0)
        gc9a01_send_data(g, out_idx, out_buf);
}
DECL_COMMAND(command_gc9a01_send_data, "gc9a01_send_data oid=%c data=%*s");

void
command_gc9a01_start_write(uint32_t *args)
{
    struct gc9a01 *g = oid_lookup(args[0], command_config_gc9a01);
    gc9a01_send_cmd(g, GC9A01_RAMWR);
}
DECL_COMMAND(command_gc9a01_start_write, "gc9a01_start_write oid=%c");

void
command_gc9a01_set_window(uint32_t *args)
{
    struct gc9a01 *g = oid_lookup(args[0], command_config_gc9a01);
    uint16_t x0 = args[1];
    uint16_t y0 = args[2];
    uint16_t x1 = args[3];
    uint16_t y1 = args[4];

    // Set column address
    uint8_t caset_data[] = {
        (x0 >> 8) & 0xFF, x0 & 0xFF,
        (x1 >> 8) & 0xFF, x1 & 0xFF
    };
    gc9a01_cmd_with_data(g, GC9A01_CASET, 4, caset_data);

    // Set row address
    uint8_t raset_data[] = {
        (y0 >> 8) & 0xFF, y0 & 0xFF,
        (y1 >> 8) & 0xFF, y1 & 0xFF
    };
    gc9a01_cmd_with_data(g, GC9A01_RASET, 4, raset_data);
}
DECL_COMMAND(command_gc9a01_set_window,
             "gc9a01_set_window oid=%c x0=%hu y0=%hu x1=%hu y1=%hu");

void
command_gc9a01_set_palette(uint32_t *args)
{
    oid_lookup(args[0], command_config_gc9a01);
    palette[1] = args[1];
    palette[2] = args[2];
    palette[3] = args[3];
    palette[4] = args[4];
    palette[5] = args[5];
    palette[6] = args[6];
    palette[7] = args[7];
    palette[8] = args[8];
    palette[9] = args[9];
    palette[10] = args[10];
    palette[11] = args[11];
    palette[12] = args[12];
    palette[13] = args[13];
    palette[14] = args[14];
}
DECL_COMMAND(command_gc9a01_set_palette,
             "gc9a01_set_palette oid=%c c1=%hu c2=%hu c3=%hu c4=%hu "
             "c5=%hu c6=%hu c7=%hu c8=%hu c9=%hu c10=%hu c11=%hu "
             "c12=%hu c13=%hu c14=%hu");

void
gc9a01_shutdown(void)
{
    uint8_t i;
    struct gc9a01 *g;
    foreach_oid(i, g, command_config_gc9a01) {
        gpio_out_write(g->dc, 0);
        gpio_out_write(g->reset, 0);
    }
}
DECL_SHUTDOWN(gc9a01_shutdown);
