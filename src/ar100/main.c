// Main starting point for ar100 code.
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include <string.h>
#include "board/misc.h" // dynmem_start
#include "board/irq.h" // irq_disable
#include "command.h" // shutdown
#include "generic/timer_irq.h" // timer_dispatch_many
#include "util.h"
#include "sched.h" // sched_main

#include "asm/spr.h"
#include "gpio.h"
#include "serial.h"
#include "prcm.h"

DECL_CONSTANT_STR("MCU", "ar100");

static struct task_wake console_wake;
static uint8_t receive_buf[1024];
static int receive_pos;


static inline unsigned long mfspr(unsigned long add){
	unsigned long ret;

	__asm__ __volatile__ ("l.mfspr %0,r0,%1" : "=r" (ret) : "K" (add));

	return ret;
}

static inline void mtspr(unsigned long add, unsigned long val)
{
	__asm__ __volatile__ ("l.mtspr r0,%1,%0" : : "K" (add), "r" (val));
}


void irq_disable(void){}

void irq_enable(void){}

irqstatus_t irq_save(void){
    return 0;
}

void irq_restore(irqstatus_t flag){
}

void irq_wait(void){
    //uart_puts("irq_wait()\n");
    irq_poll();
}
void irq_poll(void){
  if(r_uart_fifo_rcv())
    sched_wake_task(&console_wake);
}


void
serial_enable_tx_irq(void)
{
    // Normally this would enable the hardware irq, but we just call
    // do_uart() directly in this demo code.
    uart_puts("serial_enable_tx_irq\n");
}

/****************************************************************
 * Timers
 ****************************************************************/
// Set the next timer wake up time
static void
timer_set(uint32_t value){}

// Return the current time (in absolute clock ticks).
uint32_t timer_read_time(void){
  return mfspr(SPR_TICK_TTCR_ADDR);
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    timer_set(timer_read_time() + 50);
}

void
timer_init(void)
{
  mtspr(SPR_TICK_TTMR_ADDR, 3<<30);
}
DECL_INIT(timer_init);


/****************************************************************
 * Console IO
 ****************************************************************/

 // Process any incoming commands
 void
 console_task(void)
 {
     // Read data
     char c;
		 //uart_puts("Console_task\n");
     int ret = 0;
     while(r_uart_fifo_rcv()){
       c = r_uart_getc();
       receive_buf[receive_pos + ret] = c;
       ret++;
     }
     if(!ret)
       return;

     // Find and dispatch message blocks in the input
     int len = receive_pos + ret;
     uint_fast8_t pop_count, msglen = len > MESSAGE_MAX ? MESSAGE_MAX : len;
     ret = command_find_and_dispatch(receive_buf, msglen, &pop_count);
     if (ret) {
         len -= pop_count;
         if (len) {
             memcpy(receive_buf, &receive_buf[pop_count], len);
             sched_wake_task(&console_wake);
         }
     }
     receive_pos = len;
 }
 DECL_TASK(console_task);

// Encode and transmit a "response" message
void
console_sendf(const struct command_encoder *ce, va_list args){
     // Generate message
     uint8_t buf[MESSAGE_MAX];
     uint_fast8_t msglen = command_encode_and_frame(buf, ce, args);

     // Transmit message
     for(int i=0; i<msglen; i++){
       r_uart_putc(buf[i]);
     }
 }


// Handle shutdown request from ar100
static void
shutdown_handler(uint32_t *args)
{
    shutdown("Request from ar100");
}

const struct command_parser shutdown_request = {
    .func = shutdown_handler,
};

void delay_cycles(uint32_t cycles){
  uint32_t end = timer_read_time() + cycles;
  while(end > timer_read_time());
}

__noreturn void main(uint32_t exception);
__noreturn void main(uint32_t exception){

  /* Swith CPUS to 300 MHz. This should be done in Linux eventually */
  r_prcm_set_cpus_clk_rate(PLL_PERIPH);

  r_uart_init();
  uart_puts("**Start**\n");
  timer_init();
	/*struct gpio_in in = gpio_in_setup(4, 0);
	uint8_t val;
	while(1){
		val = gpio_in_read(in);
		uart_puti(val);
		uart_puts("\n");
	}*/
  sched_main();
	while(1){} // Stop complaining aout noreturn
}
