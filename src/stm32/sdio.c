// SDIO functions on STM32
//
// Copyright (C) 2022  H. Gregor Molter <gregor.molter@secretlab.de>
// Copyright (C) 2024 Bevan Weiss <bevan.weiss@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/io.h" // readb, writeb
#include "command.h" // shutdown
#include "sdio.h" // sdio_setup
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown
#include "generic/armcm_timer.h" // udelay

#define _CAT(a,V...) a##V
// STM32F1 / STM32F2 / STM32F4 all use SDIO (and have only 1)
// STM32F7 / STM32H7 / STM32L4 all use SDMMC (and might have 1 or 2 interfaces)
#ifdef SDMMC1
#define KLIPPER_SDTYPE SDMMC_TypeDef
#define SDIO_OR_SDMMC(N) _CAT(SDMMC_, N)
#ifndef SDMMC_CLKCR_CLKEN
#define SDMMC_CLKCR_CLKEN 0
#endif /* SDMMC_CLKCR_CLKEN */
#ifndef SDMMC_CLKCR_BYPASS
#define SDMMC_CLKCR_BYPASS 0
#endif /* SDMMC_CLKCR_BYPASS */
#ifndef SDMMC_CMD_SDIOSUSPEND
#define CMD_SDIOSUSPEND CMD_CMDSUSPEND
#endif /* SDMMC_CMD_SDIOSUSPEND */
#ifndef SDMMC_STA_CMDACT
#define STA_CMDACT STA_CPSMACT
#endif /* SDMMC_STA_CMDACT */
#else
#define KLIPPER_SDTYPE SDIO_TypeDef
#define SDIO_OR_SDMMC(N) _CAT(SDIO_, N)
#endif /* SDMMC1 */
#define SDIO_MAX_CLKDIV SDIO_OR_SDMMC(CLKCR_CLKDIV_Msk)

struct sdio_info {
    KLIPPER_SDTYPE *sdio;
    uint8_t clk_pin, cmd_pin, dat0_pin, dat1_pin, dat2_pin, dat3_pin, function;
};

enum {
    SDIO_OK = 0,
    SDIO_ERROR = 1,
    SDIO_TIMEOUT = 2,
    SDIO_CMD_RESPONSE_TIMEOUT = 3,
    SDIO_CRC_FAIL = 4,
    SDIO_WRONG_CMD_RESPONSE = 5,
    SDIO_WRONG_BLOCKSIZE = 6,
    SDIO_DATA_TIMEOUT = 7,
    SDIO_READ_OVERRUN = 8,
    SDIO_WRITE_UNDERRUN = 9,
    SDIO_NO_DATA_MEMORY = 10,
};

enum {
    SDIO_CARDVER_UNKNOWN = 0,
    SDIO_CARDVER_1X = 1,
    SDIO_CARDVER_2X = 2
};

enum {
    SDIO_CARDTYPE_UNKNOWN = 0,
    SDIO_CARDTYPE_SDHC_SDXC = 1,
    SDIO_CARDTYPE_SDSC = 2
};

enum {
    SDIO_WAIT_NO_RESPONSE = 0,
    SDIO_WAIT_SHORT_RESPONSE = 1,
    SDIO_WAIT_LONG_RESPONSE = 2
};


#ifdef SDMMC1
// PINS: CLK -> PC12 , CMD -> PD2,
//       DAT0 -> PC8, DAT1 -> PC9, DAT2 -> PC10, DAT3 -> PC11
DECL_ENUMERATION("sdio_bus", "sdio1a", 0);
DECL_CONSTANT_STR("BUS_PINS_sdio1a", "PC12,PD2,PC8,PC9,PC10,PC11");
#ifdef SDMMC2
// PINS: CLK -> PD6 , CMD -> PD7,
//       DAT0 -> PG9, DAT1 -> PG10, DAT2 -> PG11, DAT3 -> PG12
DECL_ENUMERATION("sdio_bus", "sdio2a", 1);
DECL_CONSTANT_STR("BUS_PINS_sdio2a", "PD6,PD7,PG9,PG10,PG11,PG12");
// PINS: CLK -> PC1 , CMD -> PA0,
//       DAT0 -> PB14, DAT1 -> PB15, DAT2 -> PB3, DAT3 -> PB4
DECL_ENUMERATION("sdio_bus", "sdio2b", 2);
DECL_CONSTANT_STR("BUS_PINS_sdio2b", "PC1,PA0,PB14,PB15,PB3,PB4");
#endif /* SDMMC2 */
#else
// PINS: CLK -> PC12 , CMD -> PD2,
//       DAT0 -> PC8, DAT1 -> PC9, DAT2 -> PC10, DAT3 -> PC11
DECL_ENUMERATION("sdio_bus", "sdio", 0);
DECL_CONSTANT_STR("BUS_PINS_sdio", "PC12,PD2,PC8,PC9,PC10,PC11");
#endif /* SDMMC1 */


static const struct sdio_info sdio_bus[] = {
#ifdef SDMMC1
    { SDMMC1, GPIO('C', 12), GPIO('D', 2), GPIO('C', 8),
      GPIO('C', 9), GPIO('C', 10), GPIO('C', 11), GPIO_FUNCTION(12) },
#ifdef SDMMC2
    { SDMMC2, GPIO('D', 6), GPIO('D', 7), GPIO('G', 9),
      GPIO('G', 10), GPIO('G', 11), GPIO('G', 12), GPIO_FUNCTION(11) },
    { SDMMC2, GPIO('C', 1), GPIO('A', 0), GPIO('B', 14),
      GPIO('B', 15), GPIO('B', 3), GPIO('B', 4), GPIO_FUNCTION(9) },
#endif /* SDMMC2 */
#else
    { SDIO, GPIO('C', 12), GPIO('D', 2), GPIO('C', 8),
      GPIO('C', 9), GPIO('C', 10), GPIO('C', 11), GPIO_FUNCTION(12) },
#endif /* SDMMC1 */
};

#define SDIO_INIT_CLK 400000
#define SDIO_MAX_TIMEOUT 500 // Wait for at least 500ms before a timeout occurs
#define CLKCR_CLEAR_MASK (  SDIO_OR_SDMMC(CLKCR_CLKDIV) | \
                            SDIO_OR_SDMMC(CLKCR_PWRSAV) | \
                            SDIO_OR_SDMMC(CLKCR_BYPASS) | \
                            SDIO_OR_SDMMC(CLKCR_WIDBUS) | \
                            SDIO_OR_SDMMC(CLKCR_NEGEDGE) | \
                            SDIO_OR_SDMMC(CLKCR_HWFC_EN) )
#define DCTRL_CLEAR_MASK (SDIO_OR_SDMMC(DCTRL_DTEN) | \
                          SDIO_OR_SDMMC(DCTRL_DTDIR) | \
                          SDIO_OR_SDMMC(DCTRL_DTMODE) | \
                          SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE))
#define CMD_CLEAR_MASK (SDIO_OR_SDMMC(CMD_CMDINDEX) | \
                        SDIO_OR_SDMMC(CMD_WAITRESP) | \
                        SDIO_OR_SDMMC(CMD_WAITINT) | \
                        SDIO_OR_SDMMC(CMD_WAITPEND) | \
                        SDIO_OR_SDMMC(CMD_CPSMEN) | \
                        SDIO_OR_SDMMC(CMD_SDIOSUSPEND))
#define SDIO_CLOCK_BYPASS_DISABLE 0
#define SDIO_CLOCK_EDGE_RISING 0
#define SDIO_CLOCK_POWER_SAVE_DISABLE 0
#define SDIO_BUS_WIDE_1B 0
#define SDIO_BUS_WIDE_4B SDIO_OR_SDMMC(CLKCR_WIDBUS_0)
#define SDIO_HARDWARE_FLOW_CONTROL_DISABLE 0
#define SDIO_HARDWARE_FLOW_CONTROL_ENABLE SDIO_OR_SDMMC(CLKCR_HWFC_EN)
#define SDIO_CMD_FLAGS (SDIO_OR_SDMMC(STA_CCRCFAIL) | \
                        SDIO_OR_SDMMC(STA_CTIMEOUT) | \
                        SDIO_OR_SDMMC(STA_CMDREND) | \
                        SDIO_OR_SDMMC(STA_CMDSENT))
#define SDIO_STATIC_FLAGS (SDIO_OR_SDMMC(STA_CCRCFAIL) | \
                            SDIO_OR_SDMMC(STA_DCRCFAIL) | \
                            SDIO_OR_SDMMC(STA_CTIMEOUT) | \
                            SDIO_OR_SDMMC(STA_DTIMEOUT) | \
                            SDIO_OR_SDMMC(STA_TXUNDERR) | \
                            SDIO_OR_SDMMC(STA_RXOVERR) | \
                            SDIO_OR_SDMMC(STA_CMDREND) | \
                            SDIO_OR_SDMMC(STA_CMDSENT) | \
                            SDIO_OR_SDMMC(STA_DATAEND) | \
                            SDIO_OR_SDMMC(STA_DBCKEND) | \
                            SDIO_OR_SDMMC(STA_SDIOIT))

struct sdio_config
sdio_setup(uint32_t bus)
{
    if (bus >= ARRAY_SIZE(sdio_bus))
        shutdown("Invalid sdio bus");

    // Enable SDIO
    KLIPPER_SDTYPE *sdio = sdio_bus[bus].sdio;
    if (!is_enabled_pclock((uint32_t)sdio)) {
        // Enable clock
        enable_pclock((uint32_t)sdio);
        // Initialize pins
        gpio_peripheral(
            sdio_bus[bus].dat0_pin, sdio_bus[bus].function|GPIO_HIGH_SPEED, 1);
        gpio_peripheral(
            sdio_bus[bus].dat1_pin, sdio_bus[bus].function|GPIO_HIGH_SPEED, 1);
        gpio_peripheral(
            sdio_bus[bus].dat2_pin, sdio_bus[bus].function|GPIO_HIGH_SPEED, 1);
        gpio_peripheral(
            sdio_bus[bus].dat3_pin, sdio_bus[bus].function|GPIO_HIGH_SPEED, 1);
        gpio_peripheral(
            sdio_bus[bus].cmd_pin, sdio_bus[bus].function|GPIO_HIGH_SPEED, 1);
        gpio_peripheral(
            sdio_bus[bus].clk_pin, sdio_bus[bus].function|GPIO_HIGH_SPEED, 1);
    }

    struct sdio_config sdio_config = { .sdio = sdio };

    // Setup SDIO with 1 bit width first and slow clock ~400 kHz
    sdio_set_speed(sdio_config, SDIO_INIT_CLK);

    // Disable clk
    CLEAR_BIT(sdio->CLKCR, SDIO_OR_SDMMC(CLKCR_CLKEN));
    // Set power state to _on_
    sdio->POWER = SDIO_OR_SDMMC(POWER_PWRCTRL);
    // Wait for 2ms (standard: at least 1ms) to settle
    udelay(2000);
    // Enable Clk
    SET_BIT(sdio->CLKCR, SDIO_OR_SDMMC(CLKCR_CLKEN));

    return sdio_config;
}

uint32_t
sdio_get_cmd_error(struct sdio_config sdio, uint32_t flags)
{
    KLIPPER_SDTYPE *regs = sdio.sdio;

    // wait for a timeout (max. SDIO_MAX_TIMEOUT) in msec.
    // 8 cycles is the instruction cycles for the loop below.
    uint32_t sta;
    uint32_t count = SDIO_MAX_TIMEOUT * (SystemCoreClock / 8U / 1000U);
    do {
        if (count-- == 0) {
            return SDIO_TIMEOUT;
        }
        sta = regs->STA;
    } while ((sta & flags) == 0 || (sta & SDIO_OR_SDMMC(STA_CMDACT)) != 0);

    regs->ICR = SDIO_CMD_FLAGS;

    if (sta & SDIO_OR_SDMMC(STA_CTIMEOUT)) {
        return SDIO_CMD_RESPONSE_TIMEOUT;
    }
    if (sta & SDIO_OR_SDMMC(STA_CCRCFAIL)) {
        return SDIO_CRC_FAIL;
    }

    return SDIO_OK;
}

uint8_t
sdio_send_command(struct sdio_config sdio_config, uint8_t cmd,
    uint32_t argument, uint8_t wait, uint8_t *response_data,
    uint8_t *response_data_len)
{
    KLIPPER_SDTYPE *sdio = sdio_config.sdio;
    uint32_t wait_flags = 0; // valid for SDIO_WAIT_NO_RESPONSE
    uint32_t sta_flags = (wait == SDIO_WAIT_NO_RESPONSE) ?
        SDIO_OR_SDMMC(STA_CMDSENT) : \
        SDIO_OR_SDMMC(STA_CCRCFAIL) | SDIO_OR_SDMMC(STA_CMDREND) | \
        SDIO_OR_SDMMC(STA_CTIMEOUT);

    if (wait == SDIO_WAIT_SHORT_RESPONSE) {
        wait_flags = SDIO_OR_SDMMC(CMD_WAITRESP_0);
    } else if (wait == SDIO_WAIT_LONG_RESPONSE) {
        wait_flags = SDIO_OR_SDMMC(CMD_WAITRESP);
    }

    // Step 1: Send command and argument
    // CMD and State Machine enabled.
    // Wait for response like specified by wait_flags.
    uint32_t cmdreg = (cmd & 0x3F) | wait_flags | SDIO_OR_SDMMC(CMD_CPSMEN);
    sdio->ARG = argument;
    MODIFY_REG(sdio->CMD, CMD_CLEAR_MASK, cmdreg);

    // Step 2: Wait until response
    // wait for a timeout (max. SDIO_MAX_TIMEOUT) in msec.
    // 8 cycles is the instruction cycles for the loop below.
    uint32_t sta;
    uint32_t count = SDIO_MAX_TIMEOUT * (SystemCoreClock / 8U / 1000U);
    do {
        if (count-- == 0) {
            return SDIO_TIMEOUT;
        }
        sta = sdio->STA;
    } while ((sta & sta_flags) == 0 || (sta & SDIO_OR_SDMMC(STA_CMDACT)) != 0);

    sdio->ICR = SDIO_CMD_FLAGS;

    // Step 3: Store response_data and check for short and long responses
    // timeout and crc.
    if (response_data != NULL) {
        if (wait == SDIO_WAIT_SHORT_RESPONSE) {
            response_data[0] = (uint8_t) ((sdio->RESP1 >> 24) & 0xFF);
            response_data[1] = (uint8_t) ((sdio->RESP1 >> 16) & 0xFF);
            response_data[2] = (uint8_t) ((sdio->RESP1 >> 8) & 0xFF);
            response_data[3] = (uint8_t) ((sdio->RESP1) & 0xFF);
            *response_data_len = 4;
        } else if (wait == SDIO_WAIT_LONG_RESPONSE) {
            // TODO Inverse?
            response_data[0] =  (uint8_t) ((sdio->RESP1 >> 24) & 0xFF);
            response_data[1] =  (uint8_t) ((sdio->RESP1 >> 16) & 0xFF);
            response_data[2] =  (uint8_t) ((sdio->RESP1 >> 8) & 0xFF);
            response_data[3] =  (uint8_t) ((sdio->RESP1) & 0xFF);
            response_data[4] =  (uint8_t) ((sdio->RESP2 >> 24) & 0xFF);
            response_data[5] =  (uint8_t) ((sdio->RESP2 >> 16) & 0xFF);
            response_data[6] =  (uint8_t) ((sdio->RESP2 >> 8) & 0xFF);
            response_data[7] =  (uint8_t) ((sdio->RESP2) & 0xFF);
            response_data[8] =  (uint8_t) ((sdio->RESP3 >> 24) & 0xFF);
            response_data[9] =  (uint8_t) ((sdio->RESP3 >> 16) & 0xFF);
            response_data[10] = (uint8_t) ((sdio->RESP3 >> 8) & 0xFF);
            response_data[11] = (uint8_t) ((sdio->RESP3) & 0xFF);
            response_data[12] = (uint8_t) ((sdio->RESP4 >> 24) & 0xFF);
            response_data[13] = (uint8_t) ((sdio->RESP4 >> 16) & 0xFF);
            response_data[14] = (uint8_t) ((sdio->RESP4 >> 8) & 0xFF);
            response_data[15] = (uint8_t) ((sdio->RESP4) & 0xFF);
            *response_data_len = 16;
        }
    }

    if (wait != SDIO_WAIT_NO_RESPONSE) {
        // CTIMEOUT and CCRCFAIL check only for short or long responses.
        if (sta & SDIO_OR_SDMMC(STA_CTIMEOUT)) {
            return SDIO_CMD_RESPONSE_TIMEOUT;
        }
        if (sta & SDIO_OR_SDMMC(STA_CCRCFAIL)) {
            return SDIO_CRC_FAIL;
        }
    }

    // Step 4: For a short response check the response cmd field, too.
    if (wait == SDIO_WAIT_SHORT_RESPONSE) {
        if (sdio->RESPCMD != cmd) {
            return SDIO_WRONG_CMD_RESPONSE;
        }
    }

    return SDIO_OK;
}

uint32_t
sdio_get_dctrl_blocksize(uint32_t value)
{
    switch(value) {
        case 1:     return 0U;
        case 2:     return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_0);
        case 4:     return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_1);
        case 8:     return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_1) | \
                           SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_0);
        case 16:    return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_2);
        case 32:    return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_2) | \
                           SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_0);
        case 64:    return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_2) | \
                           SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_1);
        case 128:   return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_2) | \
                           SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_1) | \
                           SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_0);
        case 256:   return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_3);
        case 512:   return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_3) | \
                           SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_0);
        case 1024:  return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_3) | \
                           SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_1);
        case 2048:  return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_3) | \
                           SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_1)| \
                           SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_0);
        case 4096:  return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_3) | \
                           SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_2);
        case 8192:  return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_3) | \
                           SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_2)| \
                           SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_0);
        case 16384: return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_3) | \
                           SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_2)| \
                           SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_1);
    }
    return SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_Msk);
}

uint8_t
sdio_prepare_data_transfer(struct sdio_config sdio_config, uint8_t read,
    uint32_t numblocks, uint32_t blocksize, uint32_t timeout)
{
    KLIPPER_SDTYPE *sdio = sdio_config.sdio;
    uint32_t dctrl_blocksize = sdio_get_dctrl_blocksize(blocksize);
    uint32_t reg = dctrl_blocksize | ((read > 0) ?
        SDIO_OR_SDMMC(DCTRL_DTDIR) : 0U) | SDIO_OR_SDMMC(DCTRL_DTEN);

    if (dctrl_blocksize == SDIO_OR_SDMMC(DCTRL_DBLOCKSIZE_Msk))
        return SDIO_WRONG_BLOCKSIZE;

    sdio->DCTRL = 0;
    sdio->DTIMER = timeout;
    sdio->DLEN = numblocks*blocksize;
    MODIFY_REG(sdio->DCTRL, DCTRL_CLEAR_MASK, reg);
    return SDIO_OK;
}

uint8_t
sdio_read_data(struct sdio_config sdio_config, uint8_t *data,
    uint32_t numblocks, uint32_t blocksize)
{
    // Read data by polling
    KLIPPER_SDTYPE *sdio = sdio_config.sdio;
    uint32_t data_remaining = numblocks*blocksize;
    uint8_t *buf = data;

    if (data == NULL) {
        return SDIO_NO_DATA_MEMORY;
    }

    while ((sdio->STA & (SDIO_OR_SDMMC(STA_RXOVERR) | \
                         SDIO_OR_SDMMC(STA_DCRCFAIL) | \
                         SDIO_OR_SDMMC(STA_DTIMEOUT) | \
                         SDIO_OR_SDMMC(STA_DATAEND))) == 0) {
        if ((sdio->STA & SDIO_OR_SDMMC(STA_RXFIFOE)) == 0) {
            uint32_t tmp = sdio->FIFO;
            for (uint8_t i=0; (i<4) && (data_remaining>0); i++) {
                *buf = (uint8_t)(tmp & 0xFF);
                buf++;
                data_remaining--;
                tmp >>= 8U;
            }
        }
    }

    uint32_t sta = sdio->STA;
    sdio->ICR = SDIO_STATIC_FLAGS;

    if (sta & SDIO_OR_SDMMC(STA_DTIMEOUT)) {
        return SDIO_DATA_TIMEOUT;
    }
    if (sta & SDIO_OR_SDMMC(STA_DCRCFAIL)) {
        return SDIO_CRC_FAIL;
    }
    if (sta & SDIO_OR_SDMMC(STA_RXOVERR)) {
        return SDIO_READ_OVERRUN;
    }

    // Empty FIFO and clear flags again
    while (((sdio->STA & SDIO_OR_SDMMC(STA_RXFIFOE)) == 0)
            && (data_remaining > 0)) {
        uint32_t tmp = sdio->FIFO;
        for (uint8_t i=0; (i<4) && (data_remaining>0); i++) {
            *buf = (uint8_t)(tmp & 0xFF);
            buf++;
            data_remaining--;
            tmp >>= 8U;
        }
    }

    sdio->ICR = SDIO_STATIC_FLAGS;
    return SDIO_OK;
}

uint8_t
sdio_write_data(struct sdio_config sdio_config, uint8_t *data,
    uint32_t numblocks, uint32_t blocksize)
{
    // Write data by polling
    KLIPPER_SDTYPE *sdio = sdio_config.sdio;
    uint32_t data_remaining = numblocks*blocksize;
    uint8_t *buf = data;

    if (data == NULL) {
        return SDIO_NO_DATA_MEMORY;
    }

    while ((sdio->STA & (SDIO_OR_SDMMC(STA_TXUNDERR) | \
                        SDIO_OR_SDMMC(STA_DCRCFAIL) | \
                        SDIO_OR_SDMMC(STA_DTIMEOUT) | \
                        SDIO_OR_SDMMC(STA_DATAEND))) == 0) {
        if ((sdio->STA & SDIO_OR_SDMMC(STA_TXFIFOF)) == 0) {
            uint32_t tmp = 0;
            for (uint8_t i=0; (i<4) && (data_remaining>0); i++) {
                tmp |= ((uint32_t)(*buf) << (i<<3));
                buf++;
                data_remaining--;
            }
            sdio->FIFO = tmp;
        }
    }

    uint32_t sta = sdio->STA;
    sdio->ICR = SDIO_STATIC_FLAGS;

    if (sta & SDIO_OR_SDMMC(STA_DTIMEOUT)) {
        return SDIO_DATA_TIMEOUT;
    }
    if (sta & SDIO_OR_SDMMC(STA_DCRCFAIL)) {
        return SDIO_CRC_FAIL;
    }
    if (sta & SDIO_OR_SDMMC(STA_TXUNDERR)) {
        return SDIO_WRITE_UNDERRUN;
    }

    return SDIO_OK;
}

void
sdio_send_cmd(struct sdio_config sdio_config, uint8_t cmd, uint32_t argument,
    uint8_t wait)
{
    KLIPPER_SDTYPE *sdio = sdio_config.sdio;

    sdio->ARG = argument;
    //CMD and State Machine enabled. Wait for response like specified.
    uint32_t cmdreg =   (cmd & 0x3F) | \
                        ((wait & 0xC0)) | \
                        SDIO_OR_SDMMC(CMD_CPSMEN);

    MODIFY_REG(sdio->CMD, CMD_CLEAR_MASK, cmdreg);
}

void
sdio_set_speed(struct sdio_config sdio_config, uint32_t speed)
{
    KLIPPER_SDTYPE *sdio = sdio_config.sdio;

    uint32_t clkdiv = \
            DIV_ROUND_UP(get_pclock_frequency((uint32_t)sdio), speed)-2;
    if (clkdiv > SDIO_MAX_CLKDIV) {
        output("SDIO WARNING: CLK Divider exceeds limit of %u, \
                     clkdiv: %u, speed: %u, clamping to 255",
                     SDIO_MAX_CLKDIV, clkdiv, speed);
        clkdiv = SDIO_MAX_CLKDIV;
    }

    uint32_t sdio_confreg = SDIO_CLOCK_EDGE_RISING | \
        SDIO_CLOCK_BYPASS_DISABLE | SDIO_CLOCK_POWER_SAVE_DISABLE | \
        SDIO_BUS_WIDE_1B | SDIO_HARDWARE_FLOW_CONTROL_DISABLE | \
        (clkdiv & SDIO_MAX_CLKDIV);
    MODIFY_REG(sdio->CLKCR, CLKCR_CLEAR_MASK, sdio_confreg);
}
