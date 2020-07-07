// Copyright (C) 2020  Elias Bakken <elias@iagent.no>
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "serial.h"
#include "util.h"
#include "prcm.h"
#include "r_pio.h"

void r_uart_init(void){
  // Setup Pins PL2, PL3 as UART IO
  uint32_t pl = (read_reg(PL_CFG0_REG) & 0xFFFF00FF);
  write_reg(PL_CFG0_REG, pl | 2 << 8 | 2 << 12);
  // Enable clock and assert reset
  r_prcm_uart_enable();
  // Setup baud rate (baud rate = (serial clock freq) / (16 * divisor))
  set_bit(R_UART_LCR, 7); // Enable setting DLH, DLL
  write_reg(R_UART_DLH, 0x0);
  write_reg(R_UART_DLL, 0xD); // 1 500 000
  write_reg(R_UART_LCR, 0x3); // 8 bit data length

  write_reg(R_UART_FCR, 0<<0); // Disable fifo
  r_uart_getc(); // flush input
  write_reg(R_UART_FCR, 1<<0); // Enable fifo
}

uint8_t r_uart_dr(void){
  return (read_reg(R_UART_LSR) & (1<<0));
}

char r_uart_getc(void){
  char c = (char) read_reg(R_UART_RBR);
  return c;
}

uint32_t r_uart_fifo_rcv(void){
  return read_reg(R_UART_RFL);
}

void uart_putc(char c){
  while(!(read_reg(UART0_LSR) & 1<<5))
    ;
  write_reg(UART0_THR, c);
}

void r_uart_putc(char c){
  while(!(read_reg(R_UART_LSR) & 1<<5))
    ;
  write_reg(R_UART_THR, c);
}

void uart_puts(char *s){
  while(*s){
    if(*s == '\n')
      uart_putc('\r');
    uart_putc(*s++);
  }
}
void r_uart_puts(char *s){
  while(*s){
    r_uart_putc(*s++);
  }
}

void uart_puti(uint32_t u){
  char s[9] = {0};
  for(int i=7; i>=0; i--){
    s[i] = '0'+u%10;
    u/=10;
  }
  uart_puts(s);
}