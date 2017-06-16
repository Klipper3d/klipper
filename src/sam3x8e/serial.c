// sam3x8e serial port
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memmove
#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/gpio.h" // gpio_peripheral
#include "board/io.h" // readl
#include "board/irq.h" // irq_save
#include "board/misc.h" // console_get_input
#include "command.h" // DECL_CONSTANT
#include "sam3x8e.h" // UART
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
    gpio_peripheral('A', PIO_PA8A_URXD, 'A', 1);
    gpio_peripheral('A', PIO_PA9A_UTXD, 'A', 0);

    // Reset uart
    PMC->PMC_PCER0 = 1 << ID_UART;
    UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
    UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
    UART->UART_IDR = 0xFFFFFFFF;

    // Enable uart
    UART->UART_MR = (US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT | UART_MR_PAR_NO
                     | UART_MR_CHMODE_NORMAL);
    UART->UART_BRGR = SystemCoreClock / (16 * CONFIG_SERIAL_BAUD);
    UART->UART_IER = UART_IER_RXRDY;
    NVIC_EnableIRQ(UART_IRQn);
    NVIC_SetPriority(UART_IRQn, 0);
    UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}
DECL_INIT(serial_init);

void __visible
UART_Handler(void)
{
    uint32_t status = UART->UART_SR;
    if (status & UART_SR_RXRDY) {
        uint8_t data = UART->UART_RHR;
        if (receive_pos >= sizeof(receive_buf))
            // Serial overflow - ignore it as crc error will force retransmit
            return;
        receive_buf[receive_pos++] = data;
        return;
    }
    if (status & UART_SR_TXRDY) {
        if (transmit_pos >= transmit_max)
            UART->UART_IDR = UART_IDR_TXRDY;
        else
            UART->UART_THR = transmit_buf[transmit_pos++];
    }
}

// Enable tx interrupts
static void
enable_tx_irq(void)
{
    UART->UART_IER = UART_IDR_TXRDY;
}


/****************************************************************
 * Console access functions
 ****************************************************************/

// Return a buffer (and length) containing any incoming messages
static char *
console_get_input(uint8_t *plen)
{
    *plen = readl(&receive_pos);
    return receive_buf;
}

// Remove from the receive buffer the given number of bytes
static void
console_pop_input(uint8_t len)
{
    uint32_t copied = 0;
    for (;;) {
        uint32_t rpos = readl(&receive_pos);
        uint32_t needcopy = rpos - len;
        if (needcopy) {
            memmove(&receive_buf[copied], &receive_buf[copied + len]
                    , needcopy - copied);
            copied = needcopy;
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
    uint8_t buf_len, pop_count;
    char *buf = console_get_input(&buf_len);
    int8_t ret = command_find_block(buf, buf_len, &pop_count);
    if (ret > 0)
        command_dispatch(buf, pop_count);
    if (ret)
        console_pop_input(pop_count);
}
DECL_TASK(console_task);

// Return an output buffer that the caller may fill with transmit messages
static char *
console_get_output(uint8_t len)
{
    uint32_t tpos = readl(&transmit_pos), tmax = readl(&transmit_max);
    if (tpos >= tmax) {
        tpos = tmax = 0;
        writel(&transmit_max, 0);
        writel(&transmit_pos, 0);
    }
    if (tmax + len <= sizeof(transmit_buf))
        return &transmit_buf[tmax];
    if (tmax - tpos + len > sizeof(transmit_buf))
        return NULL;
    // Disable TX irq and move buffer
    writel(&transmit_max, 0);
    tpos = readl(&transmit_pos);
    tmax -= tpos;
    memmove(&transmit_buf[0], &transmit_buf[tpos], tmax);
    writel(&transmit_pos, 0);
    writel(&transmit_max, tmax);
    enable_tx_irq();
    return &transmit_buf[tmax];
}

// Accept the given number of bytes added to the transmit buffer
static void
console_push_output(uint8_t len)
{
    writel(&transmit_max, readl(&transmit_max) + len);
    enable_tx_irq();
}

// Encode and transmit a "response" message
void
console_sendf(const struct command_encoder *ce, va_list args)
{
    uint8_t buf_len = ce->max_size;
    char *buf = console_get_output(buf_len);
    if (!buf)
        return;
    uint8_t msglen = command_encodef(buf, buf_len, ce, args);
    command_add_frame(buf, msglen);
    console_push_output(msglen);
}
