#ifndef __GENERIC_T5UID1_SERIAL_IRQ_H
#define __GENERIC_T5UID1_SERIAL_IRQ_H

#include <stdint.h> // uint32_t

void t5uid1_init(uint32_t baud);
// callback provided by board specific code
void t5uid1_enable_tx_irq(void);

// serial_irq.c
void t5uid1_rx_byte(uint_fast8_t data);
int t5uid1_get_tx_byte(uint8_t *pdata);

#endif // serial_irq.h
