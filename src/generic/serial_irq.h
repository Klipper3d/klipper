#ifndef __GENERIC_SERIAL_IRQ_H
#define __GENERIC_SERIAL_IRQ_H

#include <stdint.h> // uint32_t

// callback provided by board specific code
void serial_enable_tx_irq(void);

// serial_irq.c
void serial_rx_byte(uint_fast8_t data);
int serial_get_tx_byte(uint8_t *pdata);

// keep console alive when time is frozen
void console_frozen_idle(void);

#endif // serial_irq.h
