#ifndef __GENERIC_SERIAL_BRIDGE_IRQ_H
#define __GENERIC_SERIAL_BRIDGE_IRQ_H

#include <stdint.h> // uint32_t

#define SERIAL_BRIDGE_RX_BUFFER_SIZE 192

// callback provided by board specific code
void serial_bridge_enable_tx_irq(void);

// serial_brodge_irq.c
void serial_bridge_rx_byte(uint_fast8_t data);
int serial_bridge_get_tx_byte(uint8_t *pdata);

// serial_bridge.c
void serial_bridge_send(uint8_t* data, uint_fast8_t size);

// serial_bridge.c
uint8_t serial_bridge_get_data(uint8_t* data);

#endif // serial_bridge_irq.h
