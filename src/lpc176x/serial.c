// lpc176x serial port
//
// Copyright (C) 2018-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/armcm_boot.h" // armcm_enable_irq
#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/irq.h" // irq_save
#include "board/serial_irq.h" // serial_rx_data
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // gpio_peripheral
#include "sched.h" // DECL_INIT

// Write tx bytes to the serial port
static void
kick_tx(void)
{
    for (;;) {
        if (!(LPC_UART0->LSR & (1<<5))) {
            // Output fifo full - enable tx irq
            LPC_UART0->IER = 0x03;
            break;
        }
        uint8_t data;
        int ret = serial_get_tx_byte(&data);
        if (ret) {
            // No more data to send - disable tx irq
            LPC_UART0->IER = 0x01;
            break;
        }
        LPC_UART0->THR = data;
    }
}

void
UART0_IRQHandler(void)
{
    uint32_t iir = LPC_UART0->IIR, status = iir & 0x0f;
    if (status == 0x04)
        serial_rx_byte(LPC_UART0->RBR);
    else if (status == 0x02)
        kick_tx();
}

void
serial_enable_tx_irq(void)
{
    if (LPC_UART0->LSR & (1<<5)) {
        irqstatus_t flag = irq_save();
        kick_tx();
        irq_restore(flag);
    }
}

DECL_CONSTANT_STR("RESERVE_PINS_serial", "P0.3,P0.2");

void
serial_init(void)
{
    // Setup baud
    LPC_UART0->LCR = (1<<7); // set DLAB bit
    enable_pclock(PCLK_UART0);
    uint32_t pclk = get_pclock_frequency(PCLK_UART0);
    uint32_t div = pclk / (CONFIG_SERIAL_BAUD * 16);
    LPC_UART0->DLL = div & 0xff;
    LPC_UART0->DLM = (div >> 8) & 0xff;
    LPC_UART0->FDR = 0x10;
    LPC_UART0->LCR = 3; // 8N1 ; clear DLAB bit

    // Enable fifo
    LPC_UART0->FCR = 0x01;

    // Setup pins
    gpio_peripheral(GPIO(0, 3), 1, 0);
    gpio_peripheral(GPIO(0, 2), 1, 0);

    // Enable receive irq
    armcm_enable_irq(UART0_IRQHandler, UART0_IRQn, 0);
    LPC_UART0->IER = 0x01;
}
DECL_INIT(serial_init);
