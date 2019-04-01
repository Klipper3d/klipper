// sam3/sam4 serial port
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/serial_irq.h" // serial_rx_data
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // gpio_peripheral
#include "sched.h" // DECL_INIT

// Serial port pins
#if CONFIG_MACH_SAM3X
#define Serial_IRQ_Handler UART_Handler
static Uart * const Port = UART;
static const uint32_t Pmc_id = ID_UART, Irq_id = UART_IRQn;
static const uint32_t rx_pin = GPIO('A', 8);
static const uint32_t tx_pin = GPIO('A', 9);
DECL_CONSTANT_STR("RESERVE_PINS_serial", "PA8,PA9");
#elif CONFIG_MACH_SAM4S
#define Serial_IRQ_Handler UART1_Handler
static Uart * const Port = UART1;
static const uint32_t Pmc_id = ID_UART1, Irq_id = UART1_IRQn;
static const uint32_t rx_pin = GPIO('B', 2);
static const uint32_t tx_pin = GPIO('B', 3);
DECL_CONSTANT_STR("RESERVE_PINS_serial", "PB2,PB3");
#elif CONFIG_MACH_SAM4E
#define Serial_IRQ_Handler UART0_Handler
static Uart * const Port = UART0;
static const uint32_t Pmc_id = ID_UART0, Irq_id = UART0_IRQn;
static const uint32_t rx_pin = GPIO('A', 9);
static const uint32_t tx_pin = GPIO('A', 10);
DECL_CONSTANT_STR("RESERVE_PINS_serial", "PA9,PA10");
#endif

void
serial_init(void)
{
    gpio_peripheral(rx_pin, 'A', 1);
    gpio_peripheral(tx_pin, 'A', 0);

    // Reset uart
    enable_pclock(Pmc_id);
    Port->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
    Port->UART_CR = (UART_CR_RSTRX | UART_CR_RSTTX
                     | UART_CR_RXDIS | UART_CR_TXDIS);
    Port->UART_IDR = 0xFFFFFFFF;

    // Enable uart
    Port->UART_MR = (US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT | UART_MR_PAR_NO
                     | UART_MR_CHMODE_NORMAL);
    Port->UART_BRGR = SystemCoreClock / (16 * CONFIG_SERIAL_BAUD);
    Port->UART_IER = UART_IER_RXRDY;
    NVIC_EnableIRQ(Irq_id);
    NVIC_SetPriority(Irq_id, 0);
    Port->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}
DECL_INIT(serial_init);

void __visible
Serial_IRQ_Handler(void)
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
