#include <stdint.h>

#define R_UART_BASE 0x01F02800
#define R_UART_RBR  R_UART_BASE + 0x00 // UART Receive Buffer Register
#define R_UART_THR  R_UART_BASE + 0x00 // UART Transmit Holding Register
#define R_UART_DLL  R_UART_BASE + 0x00 // UART Divisor Latch Low Register
#define R_UART_DLH  R_UART_BASE + 0x04 // UART Divisor Latch High Register
#define R_UART_IER  R_UART_BASE + 0x04 // UART Interrupt Enable Register
#define R_UART_IIR  R_UART_BASE + 0x08 // UART Interrupt Identity Register
#define R_UART_FCR  R_UART_BASE + 0x08 // UART FIFO Control Register
#define R_UART_LCR  R_UART_BASE + 0x0C // UART Line Control Register
#define R_UART_MCR  R_UART_BASE + 0x10 // UART Modem Control Register
#define R_UART_LSR  R_UART_BASE + 0x14 // UART Line Status Register
#define R_UART_MSR  R_UART_BASE + 0x18 // UART Modem Status Register
#define R_UART_SCH  R_UART_BASE + 0x1C // UART Scratch Register
#define R_UART_USR  R_UART_BASE + 0x7C // UART Status Register
#define R_UART_TFL  R_UART_BASE + 0x80 // UART Transmit FIFO Level
#define R_UART_RFL  R_UART_BASE + 0x84 // UART_RFL
#define R_UART_HLT  R_UART_BASE + 0xA4 // UART Halt TX Register

#define UART0_BASE  0x01C28000
#define UART0_RBR  UART0_BASE + 0x00 // UART Receive Buffer Register
#define UART0_THR  UART0_BASE + 0x00 // UART Transmit Holding Register
#define UART0_DLL  UART0_BASE + 0x00 // UART Divisor Latch Low Register
#define UART0_DLH  UART0_BASE + 0x04 // UART Divisor Latch High Register
#define UART0_IER  UART0_BASE + 0x04 // UART Interrupt Enable Register
#define UART0_IIR  UART0_BASE + 0x08 // UART Interrupt Identity Register
#define UART0_FCR  UART0_BASE + 0x08 // UART FIFO Control Register
#define UART0_LCR  UART0_BASE + 0x0C // UART Line Control Register
#define UART0_MCR  UART0_BASE + 0x10 // UART Modem Control Register
#define UART0_LSR  UART0_BASE + 0x14 // UART Line Status Register
#define UART0_MSR  UART0_BASE + 0x18 // UART Modem Status Register
#define UART0_SCH  UART0_BASE + 0x1C // UART Scratch Register
#define UART0_USR  UART0_BASE + 0x7C // UART Status Register
#define UART0_TFL  UART0_BASE + 0x80 // UART Transmit FIFO Level
#define UART0_RFL  UART0_BASE + 0x84 // UART_RFL
#define UART0_HLT  UART0_BASE + 0xA4 // UART Halt TX Register


#define R_PRCM_BASE               0x01F01400
#define APB0_CLK_GATING_REG       R_PRCM_BASE + 0x0028 // APB0 Clock Gating Reg
#define APB0_SOFT_RST_REG         R_PRCM_BASE + 0x00B0 // APB0 SW Reset Reg

void r_uart_init(void);
void r_uart_putc(char c);
char r_uart_getc(void);
uint32_t r_uart_fifo_rcv(void);
void uart_putc(char c);
void uart_puts(char *s);
void uart_puti(uint32_t u);
