// Main starting point for ar100 code.
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include "board/misc.h" // dynmem_start
#include "board/io.h" // readl
#include "board/irq.h" // irq_disable
#include "command.h" // shutdown
#include "generic/timer_irq.h" // timer_dispatch_many
#include "internal.h" // SHARED_MEM
#include "sched.h" // sched_main

DECL_CONSTANT_STR("MCU", "ar100");

#define PRCM_BASE 0x01f01400

#define SHARED_MEM ((struct shared_mem *)0x00013c00)

#define NUM_COMMANDS 2
#define SHARED_MEM_BASE 0x00013c00
#define SHARED_MEM_LENGTH 1000
volatile unsigned int *sram = (volatile unsigned int *)SHARED_MEM_BASE;

struct command{
  uint16_t gpios;
  uint16_t padding;
  uint32_t delay;
};

/****************************************************************
 * Timers
 ****************************************************************/

void
irq_disable(void)
{
}

void
irq_enable(void)
{
}

irqstatus_t
irq_save(void)
{
    return 0;
}

void
irq_restore(irqstatus_t flag)
{
}

void
irq_wait(void)
{
    irq_poll();
}

// Set the next timer wake up time
static void
timer_set(uint32_t value)
{
    if (!value)
        value = 1;
}

// Return the current time (in absolute clock ticks).
uint32_t
timer_read_time(void)
{
    return 0;
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    timer_set(timer_read_time() + 50);
}

void
irq_poll(void)
{
}

void
timer_init(void)
{
}
DECL_INIT(timer_init);


/****************************************************************
 * Console IO
 ****************************************************************/

// Process any incoming commands
void
console_task(void)
{
    const struct command_parser *cp = SHARED_MEM->next_command;
    if (!cp)
        return;

    if (sched_is_shutdown() && !(cp->flags & HF_IN_SHUTDOWN)) {
        sched_report_shutdown();
    } else {
        void (*func)(uint32_t*) = cp->func;
        func(SHARED_MEM->next_command_args);
    }

    writel(&SHARED_MEM->next_command, 0);
}
DECL_TASK(console_task);

// Encode and transmit a "response" message
void
console_sendf(const struct command_encoder *ce, va_list args)
{
    // Verify space for message
    uint32_t send_push_pos = SHARED_MEM->send_push_pos;
    struct shared_response_buffer *s = &SHARED_MEM->send_data[send_push_pos];
    if (readl(&s->count))
        return;

    // Generate message
    uint32_t msglen = command_encodef(s->data, ce, args);

    // Signal PRU0 to transmit message
    writel(&s->count, msglen);
    SHARED_MEM->send_push_pos = (
        (send_push_pos + 1) % ARRAY_SIZE(SHARED_MEM->send_data));
}

void
console_shutdown(void)
{
    writel(&SHARED_MEM->next_command, 0);
}
DECL_SHUTDOWN(console_shutdown);

// Handle shutdown request from ar100
static void
shutdown_handler(uint32_t *args)
{
    shutdown("Request from ar100");
}
const struct command_parser shutdown_request = {
    .func = shutdown_handler,
};


/****************************************************************
 * Dynamic memory
 ****************************************************************/

static char dynmem_pool[3 * 1024];

// Return the start of memory available for dynamic allocations
void *
dynmem_start(void)
{
    return dynmem_pool;
}

// Return the end of memory available for dynamic allocations
void *
dynmem_end(void)
{
    return &dynmem_pool[sizeof(dynmem_pool)];
}


/****************************************************************
 * Startup
 ****************************************************************/

 // Support config_reset
 DECL_COMMAND_FLAGS(config_reset, HF_IN_SHUTDOWN, "config_reset");

 inline void  write_reg(uint32_t addr, uint32_t val){
   *((volatile unsigned long *)(addr)) = val;
 }


 void delay_cycles(uint32_t cycles){
   if(cycles <= 2)
     return;
   while(cycles--){
     __asm__ __volatile__ ("l.nop");
   }
 }

 /*void gpio_init(void){
   write_reg(PE_CFG0, 1 << 0 | 1 << 4 | 1 << 8 | 1 << 12 | 1 << 16 | 1 << 20 | 1 << 24 | 1 << 28);
   write_reg(PE_CFG1, 1 << 0 | 1 << 4 | 1 << 8 | 1 << 12 | 1 << 16 | 1 << 20 | 1 << 24 | 1 << 28);

   write_reg(PB_CFG0, 1 << 28);
 }*/


// Main entry point
int
main(void){
  /* Swith CPUS to 300 MHz. This should be done in Linux eventually */
  write_reg(PRCM_BASE, 2<<16 | 1<<8);

  struct command commands[NUM_COMMANDS] = {
    {
      .delay = 1000000,
      .gpios = 1 << 7
    },
    {
      .delay = 1000000,
      .gpios = 0 << 7
    },
  };

  //gpio_init();

  SHARED_MEM->command_index = command_index;
  SHARED_MEM->command_index_size = command_index_size;

  /*while(1){
    for(int i=0; i<2; i++){
      write_reg(PB_DATA, commands[i].gpios);
      //delay_cycles(commands[i].delay);
    }
  }*/

    sched_main();
    return 0;
}
