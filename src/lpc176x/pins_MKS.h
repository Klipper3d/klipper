#ifndef LPC176X_PINS_H_
#define LPC176X_PINS_H_

#include "gpio.h"

typedef enum {
    pin_SBASE_LED0 = GPIO('B', 18), // P1_18
    pin_SBASE_LED1 = GPIO('B', 19), // P1_19
    pin_SBASE_LED2 = GPIO('B', 20), // P1_20
    pin_SBASE_LED3 = GPIO('B', 21), // P1_21
    pin_SBASE_LED4 = GPIO('E', 28), // P4_28
} MKS_SBASE_LED_pins;

extern struct gpio_out SBASE_LED0; //
extern struct gpio_out SBASE_LED1; // scheduler started
extern struct gpio_out SBASE_LED2; // scheduler run (blink)
extern struct gpio_out SBASE_LED3; //
extern struct gpio_out SBASE_LED4; //

/*
#ifdef __LPC176x__
#include "pins_MKS.h"
#endif

#ifdef __LPC176x__
#include "lpc176x/pins_MKS.h"
#endif

#ifdef __LPC176x__
serial_uart_printf();
#endif

#ifdef __LPC176x__
gpio_out_write(SBASE_LED0, 0);
#endif

*/

// Debug UART0
extern void serial_uart_init(void);
extern void serial_uart_put(char const c);
extern void serial_uart_puts(char const * const str);
extern void serial_uart_put_num(uint32_t n, uint8_t const base);
extern void serial_uart_printf(char const * const format,...);

#define DEBUG_OUT(_x) serial_uart_puts(_x)
#define DEBUG_OUTF(...) serial_uart_printf(__VA_ARGS__)


#define FORCE_INLINE __attribute__((always_inline)) inline

#endif // LPC176X_PINS_H_
