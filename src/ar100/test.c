

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


#define TEST_TIMES 100000
void time_gpio(void){
  timer_init();
  //uint32_t now1 = timer_read_time();
  //uart_puts("Now: ");
  //uart_puti(now);
  //uart_puts("\n");

  struct gpio_out pb7 = gpio_out_setup(32+7, 0);
  struct gpio_out pl4 = gpio_out_setup(4, 0);
  /*uint32_t before, after, overhead;
  before = timer_read_time();
  for(int i=0; i<TEST_TIMES; i++){
    __asm__ __volatile__ ("l.nop");
  }
  after = timer_read_time();
  overhead = after-before-TEST_TIMES;
  uint32_t now2 = timer_read_time();*/
  /*uart_puts("Now: ");
  uart_puti(now2);
  uart_puts("\n");
  uart_puts("overhead: ");
  uart_puti(overhead);
  uart_puts("\n");*/
  /*before = timer_read_time();
  for(int i=0; i<TEST_TIMES; i++){
    //gpio_out_toggle_noirq(pb7);
  }
  after = timer_read_time();*/
  /*uart_puts("Toggle PIO: ");
  uart_puti((after-before-overhead)/TEST_TIMES);
  uart_puts("\n");
  before = timer_read_time();
  for(int i=0; i<TEST_TIMES; i++){
    //gpio_out_toggle_noirq(pl4);
  }
  after = timer_read_time();
  uart_puts("Toggle R_PIO: ");
  uart_puti((after-before-overhead)/TEST_TIMES);
  uart_puts("\n");*/

  /*uint32_t now = timer_read_time();
  uart_puts("Now: ");
  uart_puti(now);
  uart_puts("\n");*/
  uint32_t r = 0x01C20800 + 1*0x24 + 0x10;
  r = 0x01F02C00+0x10;

  /**((volatile uint32_t *)(r)) = 0xFF;

  uint32_t now = timer_read_time();
  __asm__ __volatile__ ("l.nop 0x6");
  *((volatile uint32_t *)(r)) = 0xFF;
  __asm__ __volatile__ ("l.nop 0x6");
  __asm__ __volatile__ ("l.nop 0x6");
  __asm__ __volatile__ ("l.nop 0x6");
  //__asm__ __volatile__ ("l.nop 0x6");
  //__asm__ __volatile__ ("l.nop 0x6");

  before = timer_read_time();
  after = timer_read_time();
  uint32_t overhead = after-before;*/
  /*uart_puts("overhead: ");
  uart_puti(overhead);
  uart_puts("\n");*/
  //*((volatile uint32_t *)(r)) = 0xFF;
  //*((volatile uint32_t *)(r)) = 0xFF;
  //*((volatile uint32_t *)(r)) = 0xFF;
  uint32_t times[10];
  //*((volatile uint32_t *)(r)) = 0x00;
  __asm__ __volatile__ ("l.nop 0x6");

  /**((volatile uint32_t *)(r)) = 0xFF;
  *((volatile uint32_t *)(r)) = 0x00;
  *((volatile uint32_t *)(r)) = 0xFF;
  *((volatile uint32_t *)(r)) = 0x00;
  *((volatile uint32_t *)(r)) = 0xFF;
  *((volatile uint32_t *)(r)) = 0x00;*/
  static volatile uint32_t dummy;
__asm__ __volatile__ ("l.nop 0x6");
*((volatile uint32_t *)(r)) = 0xF0;
  while(1){
    *((volatile uint32_t *)(r)) = 0xFF;
    *((volatile uint32_t *)(r)) = 0x00;
    *((volatile uint32_t *)(r)) = 0x00;
    *((volatile uint32_t *)(r)) = 0x00;
    *((volatile uint32_t *)(r)) = 0x00;
    *((volatile uint32_t *)(r)) = 0x00;
    *((volatile uint32_t *)(r)) = 0x00;
    *((volatile uint32_t *)(r)) = 0x00;
    *((volatile uint32_t *)(r)) = 0x00;
    *((volatile uint32_t *)(r)) = 0x00;

    //*((volatile uint32_t *)(r)) = 0x00;
  }
  //*((volatile uint32_t *)(r)) = 0xFF;
  uart_puts("Times \n");
  for(int i=1; i<10; i++){
    uart_puti((times[i]-times[i-1]));
    uart_puts("\n");
  }

  /*after = timer_read_time();
  uart_puts("Write R_PIO: ");
  uart_puti((after-before));
  uart_puts("\n");*/
  //uart_puts("Now: ");
  //uart_puti(now1);
  //uart_puts("\n");
}


void test_r_pio(void){
  struct gpio_out pl4 = gpio_out_setup(4, 0);
  while(1){
    *((volatile uint32_t *)(0x01F02C00+0x10)) = 0xFF;
    *((volatile uint32_t *)(0x01F02C00+0x10)) = 0x00;
    *((volatile uint32_t *)(0x01F02C00+0x10)) = 0xFF;
    *((volatile uint32_t *)(0x01F02C00+0x10)) = 0x00;
    *((volatile uint32_t *)(0x01F02C00+0x10)) = 0xFF;
    *((volatile uint32_t *)(0x01F02C00+0x10)) = 0x00;
    *((volatile uint32_t *)(0x01F02C00+0x10)) = 0xFF;
    *((volatile uint32_t *)(0x01F02C00+0x10)) = 0x00;
  }
}