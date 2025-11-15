#include "gpio/shift_register.h"
#include "command.h"
#include "esp_clk_tree.h"
#include "esp_private/periph_ctrl.h"
#include "esp_private/spi_common_internal.h"
#include "hal/clk_tree_hal.h"
#include "hal/spi_flash_ll.h"
#include "hal/spi_hal.h"
#include "hal/spi_ll.h"

// @todo move to menuconfig
#define SPI_SR_BUS          (SPI2_HOST)
// @todo test with 5 or 10MHz and check timings with logic analyzer
#define SPI_SR_CLOCK_FREQ   (1 * 1000 * 1000)
#define SPI_SR_DATA_PIN     (11)
#define SPI_SR_CLK_PIN      (12)
#define SPI_SR_LATCH_PIN    (13)

spi_dev_t* spi = NULL;
volatile static union {
    uint32_t u32;
    uint8_t u8[4];
} sr_status = { .u32 = 0x00 };

// @todo check if transfer finished
void sr_init()
{
    spi = SPI_LL_GET_HW(SPI_SR_BUS);

    /**
     * Enable state machine clocks and reset everything
     */
    PERIPH_RCC_ATOMIC() {
        spi_ll_enable_bus_clock(SPI_SR_BUS, true);
        spi_ll_reset_register(SPI_SR_BUS);
    }

    /**
     * Configure IO
     */
    ESP_ERROR_CHECK(spicommon_bus_initialize_io(
        SPI_SR_BUS,
        &(spi_bus_config_t) {
            .miso_io_num = -1,
            .mosi_io_num = 5,
            .sclk_io_num = 6,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
        },
        SPICOMMON_BUSFLAG_MASTER,
        NULL
    ));
    spicommon_cs_initialize(SPI_SR_BUS, 7, 0, 1);

    /**
     * Set up the whoooole thing
     */
    spi_ll_enable_clock(SPI_SR_BUS, true);
    spi_ll_set_clk_source(spi, SPI_CLK_SRC_APB);
    spi_ll_master_init(spi);
    spi_ll_master_set_line_mode(spi, (spi_line_mode_t) { 1, 1, 1});
    spi_ll_master_set_clock(spi, (int)clk_hal_apb_get_freq_hz(), SPI_SR_CLOCK_FREQ, 128);
    spi_ll_master_set_mode(spi, 0);
    spi_ll_master_select_cs(spi, 0);
    spi_ll_master_set_cs_setup(spi, 0);
    spi_ll_master_set_cs_hold(spi, 1);
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
    spi_ll_set_tx_lsbfirst(spi, 0);
    spi_ll_set_mosi_bitlen(spi, 32);
    spi_ll_enable_mosi(spi, true);
    spi_ll_apply_config(spi);

    while (1) {
        spi_ll_write_buffer(spi, sr_status.u8, sizeof(sr_status.u32) * sizeof(sr_status.u8));
        spi_ll_user_start(spi);

        sr_status.u32 += 1;

        esp_rom_delay_us(30);
    }
}
DECL_INIT(sr_init);
