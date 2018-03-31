// STM32F1 serial port
//
// Copyright (C) 2018 Grigori Goronzy <greg@kinoho.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memmove
#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "command.h" // DECL_CONSTANT
#include "stm32f1xx.h" // UART
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"
#include "board/irq.h" 
#include "board/io.h"
#include "sched.h" // DECL_INIT

#define SERIAL_BUFFER_SIZE 96
static char receive_buf[SERIAL_BUFFER_SIZE];
static uint32_t receive_pos;
static char transmit_buf[SERIAL_BUFFER_SIZE];
static uint32_t transmit_pos, transmit_max;


/****************************************************************
 * Serial hardware
 ****************************************************************/

DECL_CONSTANT(SERIAL_BAUD, CONFIG_SERIAL_BAUD);

void
serial_init(void)
{
    const uint32_t pclk = __LL_RCC_CALC_PCLK2_FREQ(SystemCoreClock, LL_RCC_GetAPB2Prescaler());

    LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_USART1);
    LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_USART1);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
    LL_USART_SetBaudRate(USART1, pclk, CONFIG_SERIAL_BAUD);
    LL_USART_SetDataWidth(USART1, LL_USART_DATAWIDTH_8B);
    LL_USART_SetParity(USART1, LL_USART_PARITY_NONE);
    LL_USART_SetStopBitsLength(USART1, LL_USART_STOPBITS_1);
    LL_USART_SetHWFlowCtrl(USART1, LL_USART_HWCONTROL_NONE);
    LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_TX_RX);
    LL_USART_EnableIT_RXNE(USART1);
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(USART1_IRQn, 1);
    LL_USART_Enable(USART1);

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    LL_GPIO_AF_DisableRemap_USART1();
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_9, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_10, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_INPUT);
}
DECL_INIT(serial_init);

void __visible
USART1_IRQHandler(void)
{
    if (LL_USART_IsActiveFlag_RXNE(USART1) || LL_USART_IsActiveFlag_ORE(USART1)) {
        uint8_t data = LL_USART_ReceiveData8(USART1);
        if (data == MESSAGE_SYNC)
            sched_wake_tasks();
        if (receive_pos >= sizeof(receive_buf))
            // Serial overflow - ignore it as crc error will force retransmit
            return;
        receive_buf[receive_pos++] = data;
        return;
    }
    if (LL_USART_IsActiveFlag_TXE(USART1)) {
        if (transmit_pos >= transmit_max)
            LL_USART_DisableIT_TXE(USART1);
        else
            LL_USART_TransmitData8(USART1, transmit_buf[transmit_pos++]);
    }
}

// Enable tx interrupts
static void
enable_tx_irq(void)
{
    LL_USART_EnableIT_TXE(USART1);
}

/****************************************************************
 * Console access functions
 ****************************************************************/

// Remove from the receive buffer the given number of bytes
static void
console_pop_input(uint32_t len)
{
    uint32_t copied = 0;
    for (;;) {
        uint32_t rpos = readl(&receive_pos);
        uint32_t needcopy = rpos - len;
        if (needcopy) {
            memmove(&receive_buf[copied], &receive_buf[copied + len]
                    , needcopy - copied);
            copied = needcopy;
            sched_wake_tasks();
        }
        irqstatus_t flag = irq_save();
        if (rpos != readl(&receive_pos)) {
            // Raced with irq handler - retry
            irq_restore(flag);
            continue;
        }
        receive_pos = needcopy;
        irq_restore(flag);
        break;
    }
}

// Process any incoming commands
void
console_task(void)
{
    uint8_t pop_count;
    uint32_t rpos = readl(&receive_pos);
    int8_t ret = command_find_block(receive_buf, rpos, &pop_count);
    if (ret > 0)
        command_dispatch(receive_buf, pop_count);
    if (ret)
        console_pop_input(pop_count);
}
DECL_TASK(console_task);

// Encode and transmit a "response" message
void
console_sendf(const struct command_encoder *ce, va_list args)
{
    // Verify space for message
    uint32_t tpos = readl(&transmit_pos), tmax = readl(&transmit_max);
    if (tpos >= tmax) {
        tpos = tmax = 0;
        writel(&transmit_max, 0);
        writel(&transmit_pos, 0);
    }
    uint32_t max_size = ce->max_size;
    if (tmax + max_size > sizeof(transmit_buf)) {
        if (tmax + max_size - tpos > sizeof(transmit_buf))
            // Not enough space for message
            return;
        // Disable TX irq and move buffer
        writel(&transmit_max, 0);
        tpos = readl(&transmit_pos);
        tmax -= tpos;
        memmove(&transmit_buf[0], &transmit_buf[tpos], tmax);
        writel(&transmit_pos, 0);
        writel(&transmit_max, tmax);
        enable_tx_irq();
    }

    // Generate message
    char *buf = &transmit_buf[tmax];
    uint32_t msglen = command_encodef(buf, ce, args);
    command_add_frame(buf, msglen);

    // Start message transmit
    writel(&transmit_max, tmax + msglen);
    enable_tx_irq();
}
