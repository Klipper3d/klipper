#include <stdint.h>

#define SERIAL_BRIDGE_CNT CONFIG_ENABLE_SERIAL_BRIDGE_USART1 + \
    CONFIG_ENABLE_SERIAL_BRIDGE_USART2 + CONFIG_ENABLE_SERIAL_BRIDGE_USART6

uint8_t* serial_bridge_get_uart_index_from_config(uint8_t config);