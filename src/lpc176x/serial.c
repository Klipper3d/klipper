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

#if CONFIG_LPC_SERIAL_UART0_P03_P02
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "P0.3,P0.2");
  #define GPIO_Rx GPIO(0, 3)
  #define GPIO_Tx GPIO(0, 2)
  #define GPIO_FUNCTION_UARTx 1
  #define LPC_UARTx LPC_UART0
  #define UARTx_IRQn UART0_IRQn
  #define PCLK_UARTx PCLK_UART0
#elif CONFIG_LPC_SERIAL_UART3_P429_P428
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "P4.29,P4.28");
  #define GPIO_Rx GPIO(4, 29)
  #define GPIO_Tx GPIO(4, 28)
  #define GPIO_FUNCTION_UARTx 3
  #define LPC_UARTx LPC_UART3
  #define UARTx_IRQn UART3_IRQn
  #define PCLK_UARTx PCLK_UART3
#endif

// Write tx bytes to the serial port
static void
kick_tx(void)
{
    for (;;) {
        if (!(LPC_UARTx->LSR & (1<<5))) {
            // Output fifo full - enable tx irq
            LPC_UARTx->IER = 0x03;
            break;
        }
        uint8_t data;
        int ret = serial_get_tx_byte(&data);
        if (ret) {
            // No more data to send - disable tx irq
            LPC_UARTx->IER = 0x01;
            break;
        }
        LPC_UARTx->THR = data;
    }
}

void
UARTx_IRQHandler(void)
{
    uint32_t iir = LPC_UARTx->IIR, status = iir & 0x0f;
    if (status == 0x04)
        serial_rx_byte(LPC_UARTx->RBR);
    else if (status == 0x02)
        kick_tx();
}

void
serial_enable_tx_irq(void)
{
    if (LPC_UARTx->LSR & (1<<5)) {
        irqstatus_t flag = irq_save();
        kick_tx();
        irq_restore(flag);
    }
}

void
serial_init(void)
{
    // Setup baud
    enable_pclock(PCLK_UARTx);
    LPC_UARTx->LCR = (1<<7); // set DLAB bit
    uint32_t pclk = get_pclock_frequency(PCLK_UARTx);
    uint32_t div = pclk / (CONFIG_SERIAL_BAUD * 16);
    LPC_UARTx->DLL = div & 0xff;
    LPC_UARTx->DLM = (div >> 8) & 0xff;
    LPC_UARTx->FDR = 0x10;
    LPC_UARTx->LCR = 3; // 8N1 ; clear DLAB bit

    // Enable fifo
    LPC_UARTx->FCR = 0x01;

    // Setup pins
    gpio_peripheral(GPIO_Rx, GPIO_FUNCTION_UARTx, 0);
    gpio_peripheral(GPIO_Tx, GPIO_FUNCTION_UARTx, 0);

    // Enable receive irq
    armcm_enable_irq(UARTx_IRQHandler, UARTx_IRQn, 0);
    LPC_UARTx->IER = 0x01;
}
DECL_INIT(serial_init);
