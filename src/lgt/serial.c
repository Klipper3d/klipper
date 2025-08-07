// Example code for interacting with serial_irq.c
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h>  // uint8_t
#include <avr/interrupt.h> // USART_RX_vect
#include "board/serial_irq.h"  // serial_get_tx_byte
#include "pindefine.h"
#include "sched.h"  // DECL_INIT
#include "autoconf.h" // CONFIG_MCU
#include "command.h" // DECL_CONSTANT_STR

DECL_CONSTANT_STR("RESERVE_PINS_serial", "PD0,PD1");

void serial_init(void) {
  uint16_t baud_setting = ( CONFIG_CLOCK_FREQ / (CONFIG_SERIAL_BAUD * 8L) - 1 );
#ifdef CONFIG_SERIAL_BAUD_U2X
  UCSR0A = _BV(U2X0);  // Double speed mode USART0
#endif
  UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0) | _BV(UDRIE0);
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
  UBRR0H = baud_setting >> 8;
  UBRR0L = baud_setting;
}
DECL_INIT(serial_init);

// Rx interrupt - data available to be read.
ISR(USART_RX_vect) { serial_rx_byte(UDR0); }

// Tx interrupt - data can be written to serial.
ISR(USART_UDRE_vect) {
  uint8_t data;
  int ret = serial_get_tx_byte(&data);
  if (ret)
    UCSR0B &= ~(1 << UDRIE0);
  else
    UDR0 = data;
}

void serial_enable_tx_irq(void) { UCSR0B |= _BV(UDRIE0); }
