// Main starting point for ar100 code.
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include <string.h>
#include "board/misc.h" // dynmem_start
#include "board/irq.h" // irq_disable
#include "command.h" // shutdown
#include "generic/timer_irq.h" // timer_dispatch_many
#include "sched.h" // sched_main

#include "asm/spr.h"
#include "util.h"
#include "gpio.h"
#include "serial.h"
#include "prcm.h"
#include "timer.h"
#include "test.h"

DECL_CONSTANT_STR("MCU", "ar100");

static struct task_wake console_wake;
static uint8_t receive_buf[1024];
static int receive_pos;


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
	if(timer_interrupt_pending()){
		timer_clear_interrupt();
		uint32_t next = timer_dispatch_many();
		timer_set(next);
	}
  if(r_uart_fifo_rcv())
    sched_wake_task(&console_wake);
}


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

__noreturn void main(uint32_t exception);
__noreturn void main(uint32_t exception){

  /* Swith CPUS to 300 MHz. This should be done in Linux eventually */
  r_prcm_set_cpus_clk_rate(PLL_PERIPH);

  r_uart_init();
  uart_puts("**Start**\n");
  timer_init();
	//test_timer();
	//test_gpio();
  sched_main();
	while(1){} // Stop complaining about noreturn
}
