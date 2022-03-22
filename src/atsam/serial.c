// sam3/sam4 serial port
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/serial_irq.h" // serial_rx_data
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // gpio_peripheral
#include "sched.h" // DECL_INIT

// Serial port pins
#if CONFIG_MACH_SAM3X
#define UARTx_IRQn UART_IRQn
static Uart * const Port = UART;
static const uint32_t Pmc_id = ID_UART;
static const uint32_t rx_pin = GPIO('A', 8), tx_pin = GPIO('A', 9);
static const char uart_periph = 'A';
DECL_CONSTANT_STR("RESERVE_PINS_serial", "PA8,PA9");
#elif CONFIG_MACH_SAM4S
#define UARTx_IRQn UART1_IRQn
static Uart * const Port = UART1;
static const uint32_t Pmc_id = ID_UART1;
static const uint32_t rx_pin = GPIO('B', 2), tx_pin = GPIO('B', 3);
static const char uart_periph = 'A';
DECL_CONSTANT_STR("RESERVE_PINS_serial", "PB2,PB3");
#elif CONFIG_MACH_SAM4E
#define UARTx_IRQn UART0_IRQn
static Uart * const Port = UART0;
static const uint32_t Pmc_id = ID_UART0;
static const uint32_t rx_pin = GPIO('A', 9), tx_pin = GPIO('A', 10);
static const char uart_periph = 'A';
DECL_CONSTANT_STR("RESERVE_PINS_serial", "PA9,PA10");
#elif CONFIG_MACH_SAME70
#define UARTx_IRQn UART2_IRQn
static Uart * const Port = UART2;
static const uint32_t Pmc_id = ID_UART2;
static const uint32_t rx_pin = GPIO('D', 25), tx_pin = GPIO('D', 26);
static const char uart_periph = 'C';
DECL_CONSTANT_STR("RESERVE_PINS_serial", "PD25,PD26");
#endif

void
UARTx_Handler(void)
{
    uint32_t status = Port->UART_SR;
    if (status & UART_SR_RXRDY)
        serial_rx_byte(Port->UART_RHR);
    if (status & UART_SR_TXRDY) {
        uint8_t data;
        int ret = serial_get_tx_byte(&data);
        if (ret)
            Port->UART_IDR = UART_IDR_TXRDY;
        else
            Port->UART_THR = data;
    }
}

void
serial_enable_tx_irq(void)
{
    Port->UART_IER = UART_IDR_TXRDY;
}

void
serial_init(void)
{
    gpio_peripheral(rx_pin, uart_periph, 1);
    gpio_peripheral(tx_pin, uart_periph, 0);

    // Reset uart
    enable_pclock(Pmc_id);
    Port->UART_CR = (UART_CR_RSTRX | UART_CR_RSTTX
                     | UART_CR_RXDIS | UART_CR_TXDIS);
    Port->UART_IDR = 0xFFFFFFFF;

    // Enable uart
    Port->UART_MR = (UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL);
    Port->UART_BRGR = get_pclock_frequency(Pmc_id) / (16 * CONFIG_SERIAL_BAUD);
    Port->UART_IER = UART_IER_RXRDY;
    armcm_enable_irq(UARTx_Handler, UARTx_IRQn, 0);
    Port->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}
DECL_INIT(serial_init);
