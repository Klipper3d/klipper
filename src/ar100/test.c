

#include "timer.h"
#include "test.h"
#include "gpio.h"

void delay_cycles(uint32_t cycles){
  uint32_t end = timer_read_time() + cycles;
  while(end > timer_read_time());
}

void test_timer(void){
  uint32_t before, after;
	before = timer_read_time();
	timer_kick();
	while(!timer_interrupt_pending()){
	}
	after = timer_read_time();
	uart_puts("Timer kick took: ");
	uart_puti(after-before);
	uart_puts(" cycles\n");
  timer_set(0xFFFFFF);
}

// Blink PB7 (user led) indefinitely
void test_gpio(void){
  struct gpio_out pb7 = gpio_out_setup(32+7, 0);
  while(1){
    gpio_out_write(pb7, 0);
    delay_cycles(10000000);
    gpio_out_write(pb7, 1);
    delay_cycles(10000000);
  }
}
