#include "timer.h"
#include "test.h"
#include "gpio.h"
#include "serial.h"

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
  timer_init();
  struct gpio_out pe9;
  while(1){
    pe9 = gpio_out_setup((4*32)+9, 1);
    gpio_out_write(pe9, 0);
    gpio_out_toggle_noirq(pe9);
    gpio_out_write(pe9, 1);
    gpio_out_toggle_noirq(pe9);
  }
}

void test_r_pio(void){
  gpio_out_setup(10, 0); // setup pl10
  while(1){
    *((volatile uint32_t *)(0x01F02C00+0x10)) = 1<<10;
    *((volatile uint32_t *)(0x01F02C00+0x10)) = 0<<10;
  }
}

void test_pio(void){
  gpio_out_setup((4*32)+6, 0); // setup pe6
  while(1){
    *((volatile uint32_t *)(0x01C20800+(4*0x24)+0x10)) = 0xFFF;
    *((volatile uint32_t *)(0x01C20800+(4*0x24)+0x10)) = 0<<6;
  }
}
