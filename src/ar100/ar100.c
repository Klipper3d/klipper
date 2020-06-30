#include <stdint.h>
#include <stdarg.h>
#include "board/io.h" // readl
#include "command.h" // command_add_frame
#include "compiler.h" // __section
#include "internal.h" // SHARED_MEM
#include "sched.h" // sched_shutdown

#define PRCM_BASE 0x01f01400
#define PIO_BASE 0x01C20800
#define PIOB_BASE PIO_BASE + (1*0x24)
#define PIOE_BASE PIO_BASE + (4*0x24)
#define PB_CFG0 PIOB_BASE + 0x0
#define PB_DATA PIOB_BASE + 0x10
#define PE_CFG0 PIOE_BASE + 0x0
#define PE_CFG1 PIOE_BASE + 0x4
#define PE_DATA PIOE_BASE + 0x10


#define NUM_COMMANDS 100
#define SHARED_MEM_BASE 0x00013c00
#define SHARED_MEM_LENGTH 1000
volatile unsigned int *sram = (volatile unsigned int *)SHARED_MEM_BASE;

struct command{
  uint16_t gpios;
  uint16_t padding;
  uint32_t delay;
};

// shutdown() compatibility code
uint8_t ctr_lookup_static_string(const char *str)
{
    return 0;
}

// Handle shutdown()
void
sched_shutdown(uint_fast8_t reason)
{

}

// Generate messages - only used for ack/nak messages
void
console_sendf(const struct command_encoder *ce, va_list args)
{
}

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


void gpio_init(void){
  write_reg(PE_CFG0, 1 << 0 | 1 << 4 | 1 << 8 | 1 << 12 | 1 << 16 | 1 << 20 | 1 << 24 | 1 << 28);
  write_reg(PE_CFG1, 1 << 0 | 1 << 4 | 1 << 8 | 1 << 12 | 1 << 16 | 1 << 20 | 1 << 24 | 1 << 28);

  write_reg(PB_CFG0, 1 << 28);
}

_Noreturn void
main(void){
  /* Swith CPUS to 300 MHz. This should be done in Linux eventually */
  write_reg(PRCM_BASE, 2<<16 | 1<<8);

  struct command commands[NUM_COMMANDS] = {
    {
      .delay = 100000,
      .gpios = 1 << 0
    },
    {
      .delay = 100000,
      .gpios = 0 << 0
    },
  };

  gpio_init();

  SHARED_MEM->command_index = command_index;
  SHARED_MEM->command_index_size = command_index_size;

  while(1){
    for(int i=0; i<2; i++){
      write_reg(PE_DATA, commands[i].gpios);
      delay_cycles(commands[i].delay);
    }
  }
}