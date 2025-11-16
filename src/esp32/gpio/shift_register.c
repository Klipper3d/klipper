#include "gpio/shift_register.h"
#include "autoconf.h"
#include "command.h"
#include "esp_clk_tree.h"
#include "esp_log.h"
#include "esp_private/periph_ctrl.h"
#include "esp_private/spi_common_internal.h"
#include "hal/clk_tree_hal.h"
#include "hal/spi_flash_ll.h"
#include "hal/spi_hal.h"
#include "hal/spi_ll.h"

#if CONFIG_HAVE_GPIO_SR

// @todo clause

#define SR_SPI_HOST (SPI2_HOST)
#define SR_BIT_NO   (CONFIG_SR_BYTE_NO * 8)

// @todo -1?
// @todo check max
DECL_ENUMERATION_RANGE("pin", "SR_0", (1u << 7), SR_BIT_NO);

spi_dev_t* spi = NULL;
volatile static uint8_t sr_data[CONFIG_SR_BYTE_NO];

void sr_init()
{
    spi = SPI_LL_GET_HW(SR_SPI_HOST);

    /**
     * Enable state machine clocks and reset everything
     */
    PERIPH_RCC_ATOMIC() {
        spi_ll_enable_bus_clock(SR_SPI_HOST, true);
        spi_ll_reset_register(SR_SPI_HOST);
    }

    /**
     * Configure IO
     */
    ESP_ERROR_CHECK(spicommon_bus_initialize_io(
        SR_SPI_HOST,
        &(spi_bus_config_t) {
            .mosi_io_num = CONFIG_SR_PIN_DATA,
            .sclk_io_num = CONFIG_SR_PIN_CLK,
            .miso_io_num = -1,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
        },
        SPICOMMON_BUSFLAG_MASTER,
        NULL
    ));
    spicommon_cs_initialize(SR_SPI_HOST, CONFIG_SR_PIN_LATCH, 0, 1);

    /**
     * Set up the whoooole thing
     */
    spi_ll_enable_clock(SR_SPI_HOST, true);
    spi_ll_set_clk_source(spi, SPI_CLK_SRC_APB);
    spi_ll_master_init(spi);
    spi_ll_master_set_line_mode(spi, (spi_line_mode_t) { 1, 1, 1});
    spi_ll_master_set_clock(spi, (int)clk_hal_apb_get_freq_hz(), (CONFIG_SR_CLOCK_FREQ_MHZ * 1000 * 1000), 128);
    spi_ll_master_set_mode(spi, 0);
    spi_ll_master_select_cs(spi, 0);
    spi_ll_master_set_cs_setup(spi, 0);
    spi_ll_master_set_cs_hold(spi, 1);
    // Set LATCH polarity (SPI CS)
    spi_ll_master_set_pos_cs(spi, 0, 0);
    spi_ll_master_keep_cs(spi, 0);
    spi_ll_set_half_duplex(spi, 0);
    spi_ll_set_sio_mode(spi, 0);
    spi_ll_set_mosi_delay(spi, 0, 0);
    spi_ll_set_mosi_free_level(spi, false);
    spi_ll_set_miso_delay(spi, 2, 0);
    spi_ll_enable_miso(spi, false);
    spi_ll_set_dummy(spi, 0);
    spi_ll_set_command_bitlen(spi, 0);
    spi_ll_set_addr_bitlen(spi, 0);
    spi_ll_set_miso_bitlen(spi, 0);
    spi_ll_clear_int_stat(spi);
    // Send LSB last
    spi_ll_set_tx_lsbfirst(spi, 0);
    spi_ll_set_mosi_bitlen(spi, SR_BIT_NO);
    spi_ll_enable_mosi(spi, true);
    spi_ll_apply_config(spi);

    // Initial write to the SR to set it to a known state
    for (uint8_t i = 0; i < CONFIG_SR_BYTE_NO; i++) {
        sr_data[i] = 0;
    }
    sr_write();


    struct gpio_out gp = { .pin = 128 };
    struct gpio_out gp2 = { .pin = 141 };

    while (1) {
        gpio_sr_write(gp, 1);
        esp_rom_delay_us(5);
        gpio_sr_write(gp2, 0);
        esp_rom_delay_us(5);
        gpio_sr_write(gp, 0);
        esp_rom_delay_us(5);
        gpio_sr_write(gp2, 1);
        esp_rom_delay_us(5);
    }
}
DECL_INIT(sr_init);

// @todo Critical section? noirq?
// @todo check if transfer finished
void sr_write()
{
    uint8_t local_buffer[CONFIG_SR_BYTE_NO];
    for (uint8_t i = 0; i < CONFIG_SR_BYTE_NO; i++) {
        local_buffer[i] = sr_data[CONFIG_SR_BYTE_NO - 1 - i];
    }
    spi_ll_write_buffer(spi, local_buffer, SR_BIT_NO);
    spi_ll_user_start(spi);
}

void gpio_sr_write(struct gpio gpio, bool val)
{
    uint8_t bit_index = gpio.pin & 0b111;
    uint8_t byte_index = (gpio.pin & 0b01111000) >> 3;

    if (val) {
        sr_data[byte_index] |= 1 << bit_index;
    } else {
        sr_data[byte_index] &= ~(1 << bit_index);
    }

    sr_write();
}

#endif