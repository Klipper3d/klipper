// STM32 serial
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/serial_bridge_irq.h" // serial_rx_byte
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT
#include "board/gpio.h"
#include "board/serial_bridge.h"

#define CR1_FLAGS (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE   \
                   | USART_CR1_RXNEIE)

typedef struct serial_bridge_config {
    uint8_t number;
    USART_TypeDef* usart;
    uint8_t rx_pin;
    uint8_t tx_pin;
    uint8_t rx_alt_function;
    uint8_t tx_alt_function;
    uint32_t baud;
    uint8_t active;
    uint8_t usart_index;
} serial_bridge_config_t;


#if CONFIG_ENABLE_SERIAL_BRIDGE_USART1
DECL_CONSTANT("SERIAL_BRIDGE_CONFIG_USART1_PA3,PA2", 0);
DECL_CONSTANT("SERIAL_BRIDGE_CONFIG_USART1_PB7,PB6", 1);
#endif
#if CONFIG_ENABLE_SERIAL_BRIDGE_USART2
DECL_CONSTANT("SERIAL_BRIDGE_CONFIG_USART2_PD6,PD5", 2);
DECL_CONSTANT("SERIAL_BRIDGE_CONFIG_USART2_PA3,PA2", 3);
#endif
#if CONFIG_ENABLE_SERIAL_BRIDGE_USART6
DECL_CONSTANT("SERIAL_BRIDGE_CONFIG_USART6_PA12,PA11", 4);
#endif

USART_TypeDef* usarts[] = {
    #if CONFIG_ENABLE_SERIAL_BRIDGE_USART1
    USART1,
    #endif
    #if CONFIG_ENABLE_SERIAL_BRIDGE_USART2
    USART2,
    #endif
    #if CONFIG_ENABLE_SERIAL_BRIDGE_USART6
    USART6,
    #endif
};

serial_bridge_config_t configs[] = {
    #if CONFIG_ENABLE_SERIAL_BRIDGE_USART1
    {
        0,
        USART1,
        GPIO('A', 3),
        GPIO('A', 2),
        7,
        7
    },
    {
        1,
        USART1,
        GPIO('B', 7),
        GPIO('B', 6),
        7,
        7
    },
    #endif
    #if CONFIG_ENABLE_SERIAL_BRIDGE_USART2
    {
        2,
        USART2,
        GPIO('D', 6),
        GPIO('D', 5),
        7,
        7
    },
    {
        3,
        USART2,
        GPIO('A', 3),
        GPIO('A', 2),
        7,
        7
    },
    #endif
    #if CONFIG_ENABLE_SERIAL_BRIDGE_USART6
    {
        4,
        USART6,
        GPIO('A', 12),
        GPIO('A', 11),
        8,
        8
    },
    #endif
};

uint8_t* serial_bridge_get_uart_index_from_config(uint8_t config){
    for(int8_t i = 0; i <  (sizeof(configs) / sizeof(configs[0])); i++){
        if(configs[i].number == config){
            return &(configs[i].usart_index);
        }
    }
    return NULL;
}

serial_bridge_config_t* serial_bridge_get_config_by_number(uint8_t number){
    for(int8_t i = 0; i <  (sizeof(configs) / sizeof(configs[0])); i++){
        if(configs[i].number == number){
            return &configs[i];
        }
    }
    return NULL;
}

serial_bridge_config_t*
serial_bridge_get_active_config_for_usart(USART_TypeDef* usart){
    for(int8_t i = 0; i <  (sizeof(configs) / sizeof(configs[0])); i++){
        if(configs[i].usart == usart && configs[i].active){
            return &configs[i];
        }
    }
    return NULL;
}

void serial_bridge_handle_uart_irq(serial_bridge_config_t* config){
    uint32_t sr = config->usart->SR;
    if (sr & (USART_SR_RXNE | USART_SR_ORE)) {
        // The ORE flag is automatically cleared by reading SR, followed
        // by reading DR.
        serial_bridge_rx_byte(config->usart->DR, config->usart_index);
    }
    if (sr & USART_SR_TXE && config->usart->CR1 & USART_CR1_TXEIE) {
        uint8_t data;
        int ret = serial_bridge_get_tx_byte(&data, config->usart_index);
        if (ret)
            config->usart->CR1 = CR1_FLAGS;
        else
            config->usart->DR = data;
    }
}

#if CONFIG_ENABLE_SERIAL_BRIDGE_USART1
void
USART1_serial_bridge_IRQHandler(void)
{
   serial_bridge_handle_uart_irq(
        serial_bridge_get_active_config_for_usart(USART1));
}
#endif

#if CONFIG_ENABLE_SERIAL_BRIDGE_USART2
void
USART2_serial_bridge_IRQHandler(void)
{
    serial_bridge_handle_uart_irq(
        serial_bridge_get_active_config_for_usart(USART2));
}
#endif


#if CONFIG_ENABLE_SERIAL_BRIDGE_USART6
void
USART6_serial_bridge_IRQHandler(void)
{
    serial_bridge_handle_uart_irq(
        serial_bridge_get_active_config_for_usart(USART6));
}
#endif

void
serial_bridge_enable_tx_irq(int8_t usart_index)
{
    for(int8_t i = 0; i < (sizeof(configs) / sizeof(configs[0])); i++){
        if(configs[i].usart_index == usart_index && configs[i].active){
            configs[i].usart->CR1 = CR1_FLAGS | USART_CR1_TXEIE;
        }
    }
}

int8_t
serial_bridge_configure(uint8_t* config, uint32_t* baud)
{
    serial_bridge_config_t* s_config =
        serial_bridge_get_config_by_number(*config);
    if (config == NULL) {
        return -1;
    }
    s_config->baud = *baud;
    s_config->active = 1;

    enable_pclock((uint32_t)s_config->usart);

    uint32_t pclk = get_pclock_frequency((uint32_t)s_config->usart);
    uint32_t div = DIV_ROUND_CLOSEST(pclk, *baud);
    s_config->usart->BRR = (((div / 16) << USART_BRR_DIV_Mantissa_Pos)
                   | ((div % 16) << USART_BRR_DIV_Fraction_Pos));
    s_config->usart->CR1 = CR1_FLAGS;

    gpio_peripheral(s_config->rx_pin,
        GPIO_FUNCTION(s_config->rx_alt_function), 1);
    gpio_peripheral(s_config->tx_pin,
        GPIO_FUNCTION(s_config->tx_alt_function), 0);

    return 0;
}


void
serial_bridge_init(void)
{
    #if CONFIG_ENABLE_SERIAL_BRIDGE_USART1
    armcm_enable_irq(USART1_serial_bridge_IRQHandler, USART1_IRQn, 0);
    #endif
    #if CONFIG_ENABLE_SERIAL_BRIDGE_USART2
    armcm_enable_irq(USART2_serial_bridge_IRQHandler, USART2_IRQn, 0);
    #endif
    #if CONFIG_ENABLE_SERIAL_BRIDGE_USART6
    armcm_enable_irq(USART6_serial_bridge_IRQHandler, USART6_IRQn, 0);
    #endif

    //assign indexes for the uart buffers that are in use
    for(int8_t i = 0; i < sizeof(configs)/sizeof(configs[0]); i++){
        for(int8_t j = 0; j < sizeof(usarts)/sizeof(usarts[0]); j++){
            if(usarts[j] == configs[i].usart){
                configs[i].usart_index = j;
            }
        }
    }
}
DECL_INIT(serial_bridge_init);