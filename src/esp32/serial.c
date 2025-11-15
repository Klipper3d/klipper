#include "autoconf.h"
#include "command.h"
#include "esp_err.h"
#include "serial_irq.h"
#include "esp_intr_types.h"
#include "driver/uart.h"
#include "hal/uart_ll.h"
#include "soc/interrupts.h"
#include "soc/uart_struct.h"

#define UART_NUM    UART_NUM_2
#define UART_IRQ    ETS_UART2_INTR_SOURCE

static uart_dev_t* uart_dev = NULL;
static intr_handle_t uart_isr_handle = NULL;

/**
 * Interrupt handler for UART.
 *
 * The uart_ll_set_rxfifo_full_thr() is set to 1 so that an interrupt is
 * triggered on each received byte. Process it and pass it along to
 * klipper.
 *
 * There is a possibility of a buffer overflow (when more bytes are
 * received than klipper can handle), but this is already handled by
 * serial_rx_byte().
 */
static void IRAM_ATTR uart_isr(void *arg)
{
    if ( uart_ll_get_intsts_mask(uart_dev) & UART_INTR_RXFIFO_FULL) {
        uint32_t length = uart_ll_get_rxfifo_len(uart_dev);
        uint8_t buffer[length];
        uart_ll_read_rxfifo(uart_dev, buffer, length);

        for (uint32_t i = 0; i < length; i++) {
            serial_rx_byte(buffer[i]);
        }

        uart_ll_clr_intsts_mask(uart_dev, UART_INTR_RXFIFO_FULL);
    }
}

/**
 * Initialize serial port (UART).
 *
 * Set up UART interface without installing ESP-IDF drivers, that way we can
 * directly access the internal FIFO buffers and manage the interrupts without
 * IDF getting in between.
 *
 * @todo check that klipper's queue is smaller than ESP's TXFIFO
 */
void serial_init(void)
{
    uart_dev = UART_LL_GET_HW(UART_NUM);

    ESP_ERROR_CHECK(uart_param_config(
        UART_NUM,
        &(uart_config_t) {
            .baud_rate = CONFIG_SERIAL_BAUD,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_APB,
        }
    ));
    ESP_ERROR_CHECK(uart_set_pin(
        UART_NUM,
        CONFIG_UART_TX_PIN,
        CONFIG_UART_RX_PIN,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE
    ));

    // Clear FIFOs
    uart_ll_rxfifo_rst(uart_dev);
    uart_ll_txfifo_rst(uart_dev);

    // Disable interrupt and clear pending status
    uart_ll_disable_intr_mask(uart_dev, UART_LL_INTR_MASK);
    uart_ll_clr_intsts_mask(uart_dev, UART_LL_INTR_MASK);

    // Set TX idle time between transfers
    uart_ll_set_tx_idle_num(uart_dev, 0);

    // Set RXFIFO_FULL interrupt threshold to 1 => an interrupt is fired on every RX byte.
    uart_ll_set_rxfifo_full_thr(uart_dev, 1);

    // Allocate interrupt
    ESP_ERROR_CHECK(esp_intr_alloc(
        UART_IRQ,
        ESP_INTR_FLAG_IRAM,
        uart_isr,
        NULL,
        &uart_isr_handle
    ));

    // Enable RXFIFO_FULL interrupt
    uart_ll_ena_intr_mask(uart_dev, UART_INTR_RXFIFO_FULL);
}
DECL_INIT(serial_init);

/**
 * Send data.
 *
 * This function is called by klipper when there's data available to be sent.
 * There's a busy-waiting safeguard to make sure that the whole TX FIFO
 * buffer is available. This could theoretically be improved to send data if
 * there's enough space in the FIFO, but that level of responsiveness it's
 * not really necessary.
 */
void serial_enable_tx_irq()
{
    // @todo 10ms is a bit too much, we should implement some sort of ring buffer
    uint32_t timeout = 10000;
    while (!uart_ll_is_tx_idle(uart_dev)) {
        if (unlikely(timeout == 0)) {
            shutdown("TX transaction took too long.");
            // ReSharper disable once CppDFAUnreachableCode
            return;
        }
        esp_rom_delay_us(1);
        timeout--;
    }

    uint8_t buffer;
    while (!serial_get_tx_byte(&buffer)) {
        uart_ll_write_txfifo(uart_dev, &buffer, sizeof(buffer));
    }
}
