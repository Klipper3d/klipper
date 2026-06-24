#ifndef __GENERIC_SERIAL_BRIDGE_IRQ_H
#define __GENERIC_SERIAL_BRIDGE_IRQ_H

#include <stdint.h> // uint32_t

#define SERIAL_BRIDGE_RX_BUFF_SIZE 192
#define SERIAL_BRIDGE_NUMBER_OF_CONFIGS = 5

// callback provided by board specific code
void serial_bridge_enable_tx_irq(int8_t usart_index);

// serial_brodge_irq.c
void serial_bridge_rx_byte(uint_fast8_t data, uint8_t usart_index);
int serial_bridge_get_tx_byte(uint8_t *pdata, uint8_t usart_index);

// serial_bridge.c
void serial_bridge_send(uint8_t* data, uint_fast8_t size, uint8_t config);

// serial_bridge.c
uint8_t serial_bridge_get_data(uint8_t* data, uint8_t config);

int8_t serial_bridge_configure(uint8_t* usart_index, uint32_t* baud);

#endif // serial_bridge_irq.h