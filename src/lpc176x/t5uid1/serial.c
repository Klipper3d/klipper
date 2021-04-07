// lpc176x serial port
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/armcm_boot.h" // armcm_enable_irq
#include "autoconf.h" // CONFIG_T5UID1_SERIAL_PORT
#include "board/irq.h" // irq_save
#include "board/t5uid1/serial_irq.h" // t5uid1_rx_byte
#include "command.h" // DECL_CONSTANT_STR
#include "../internal.h" // gpio_peripheral

#if CONFIG_SERIAL && CONFIG_T5UID1_SERIAL_PORT == 0
  #error "The serial port selected for the T5UID1 screen is already used"
#endif

// Select the configured serial port
#if CONFIG_T5UID1_SERIAL_PORT == 0
  DECL_CONSTANT_STR("RESERVE_PINS_t5uid1", "P0.3,P0.2");
  #define GPIO_Rx GPIO(0, 3)
  #define GPIO_Tx GPIO(0, 2)
  #define LPC_UARTx LPC_UART0
  #define PCLK_UARTx PCLK_UART0
  #define UARTx_IRQn UART0_IRQn
  #define UARTx_FUNC 1
#elif CONFIG_T5UID1_SERIAL_PORT == 1
  DECL_CONSTANT_STR("RESERVE_PINS_t5uid1", "P0.16,P0.15");
  #define GPIO_Rx GPIO(0, 16)
  #define GPIO_Tx GPIO(0, 15)
  #define LPC_UARTx LPC_UART1
  #define PCLK_UARTx PCLK_UART1
  #define UARTx_IRQn UART1_IRQn
  #define UARTx_FUNC 1
#elif CONFIG_T5UID1_SERIAL_PORT == 3
  DECL_CONSTANT_STR("RESERVE_PINS_t5uid1", "P4.29,P4.28");
  #define GPIO_Rx GPIO(4, 29)
  #define GPIO_Tx GPIO(4, 28)
  #define LPC_UARTx LPC_UART3
  #define PCLK_UARTx PCLK_UART3
  #define UARTx_IRQn UART3_IRQn
  #define UARTx_FUNC 3
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
        int ret = t5uid1_get_tx_byte(&data);
        if (ret) {
            // No more data to send - disable tx irq
            LPC_UARTx->IER = 0x01;
            break;
        }
        LPC_UARTx->THR = data;
    }
}

void
t5uid1_UARTx_IRQHandler(void)
{
    uint32_t iir = LPC_UARTx->IIR, status = iir & 0x0f;
    if (status == 0x04)
        t5uid1_rx_byte(LPC_UARTx->RBR);
    else if (status == 0x02)
        kick_tx();
}

void
t5uid1_enable_tx_irq(void)
{
    if (LPC_UARTx->LSR & (1<<5)) {
        irqstatus_t flag = irq_save();
        kick_tx();
        irq_restore(flag);
    }
}

void
t5uid1_init(uint32_t baud)
{
    // Setup baud
    LPC_UARTx->LCR = (1<<7); // set DLAB bit
    enable_pclock(PCLK_UARTx);
    uint32_t pclk = get_pclock_frequency(PCLK_UARTx);
    uint32_t div = pclk / (baud * 16);
    LPC_UARTx->DLL = div & 0xff;
    LPC_UARTx->DLM = (div >> 8) & 0xff;
    LPC_UARTx->FDR = 0x10;
    LPC_UARTx->LCR = 3; // 8N1 ; clear DLAB bit

    // Enable fifo
    LPC_UARTx->FCR = 0x01;

    // Setup pins
    gpio_peripheral(GPIO_Rx, UARTx_FUNC, 0);
    gpio_peripheral(GPIO_Tx, UARTx_FUNC, 0);

    // Enable receive irq
    armcm_enable_irq(t5uid1_UARTx_IRQHandler, UARTx_IRQn, 0);
    LPC_UARTx->IER = 0x01;
}
