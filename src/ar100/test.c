

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
  struct gpio_out pb7 = gpio_out_setup(32+7, 1);
  gpio_out_write(pb7, 0);
  /*while(1){
    gpio_out_toggle_noirq(pb7);
    delay_cycles(300000000);
    gpio_out_toggle_noirq(pb7);
    delay_cycles(300000000);
    gpio_out_write(pb7, 0);
    delay_cycles(300000000);
    gpio_out_write(pb7, 1);
    delay_cycles(300000000);
  }*/
}

#define TEST_TIMES 100000
void time_gpio(void){
  struct gpio_out pb7 = gpio_out_setup(32+7, 0);
  struct gpio_out pl4 = gpio_out_setup(4, 0);
  uint32_t before, after, overhead;
  before = timer_read_time();
  for(int i=0; i<TEST_TIMES; i++){
    __asm__ __volatile__ ("l.nop");
  }
  after = timer_read_time();
  overhead = after-before;
  uart_puts("overhead: ");
  uart_puti(overhead);
  uart_puts("\n");
  before = timer_read_time();
  for(int i=0; i<TEST_TIMES; i++){
    gpio_out_toggle_noirq(pb7);
  }
  after = timer_read_time();
  uart_puts("Toggle PIO: ");
  uart_puti((after-before-overhead)/TEST_TIMES);
  uart_puts("\n");
  before = timer_read_time();
  for(int i=0; i<TEST_TIMES; i++){
    gpio_out_toggle_noirq(pl4);
  }
  after = timer_read_time();
  uart_puts("Toggle R_PIO: ");
  uart_puti((after-before-overhead)/TEST_TIMES);
  uart_puts("\n");
}
