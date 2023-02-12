// Uart and r_uart functions for ar100
//
// Copyright (C) 2020-2021  Elias Bakken <elias@iagent.no>
//
// This file may be distributed under the terms of the GNU GPLv3 license.


#include "serial.h"
#include "util.h"
#include "internal.h"
#include "gpio.h"

void r_uart_init(void){
  // Setup Pins PL2, PL3 as UART IO
  gpio_mux_setup(2, PIO_ALT1);
  gpio_mux_setup(3, PIO_ALT1);

  // Enable clock and assert reset
  clear_bit(APB0_CLK_GATING_REG, 4);
  set_bit(APB0_SOFT_RST_REG, 4);
  set_bit(APB0_CLK_GATING_REG, 4);

  // Setup baud rate
  set_bit(R_UART_LCR, 7); // Enable setting DLH, DLL
  write_reg(R_UART_DLH, 0x0);
  write_reg(R_UART_DLL, 0xD); // 1 500 000
  write_reg(R_UART_LCR, 0x3); // 8 bit data length

  write_reg(R_UART_FCR, 0<<0); // Disable fifo
  r_uart_getc(); // flush input
  write_reg(R_UART_FCR, 1<<0); // Enable fifo
}

char r_uart_getc(void){
  char c = (char) read_reg(R_UART_RBR);
  return c;
}

uint32_t r_uart_fifo_rcv(void){
  return read_reg(R_UART_RFL);
}

void r_uart_putc(char c){
  while(!(read_reg(R_UART_LSR) & 1<<5))
    ;
  write_reg(R_UART_THR, c);
}

void r_uart_puts(char *s){
  while(*s){
    r_uart_putc(*s++);
  }
}
