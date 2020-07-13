
#include "timer.h"
#include "board/timer_irq.h"

volatile static uint32_t timer_compare;
static uint8_t interrupt_seen;

uint8_t timer_interrupt_pending(void){
  if(interrupt_seen){
    return 0;
  }
  if(timer_is_before(mfspr(SPR_TICK_TTCR_ADDR), timer_compare)){
    return 0;
  }

  return 1;
}
void timer_clear_interrupt(void){
  interrupt_seen = 1;
}
// Set the next timer wake up time
void timer_set(uint32_t value){
  timer_compare = value;
  interrupt_seen = 0;
}

// Return the current time (in absolute clock ticks).
uint32_t timer_read_time(void){
  return mfspr(SPR_TICK_TTCR_ADDR);
}

// Activate timer dispatch as soon as possible
void timer_kick(void){
    timer_set(timer_read_time() + 50);
}

void timer_init(void){
  interrupt_seen = 1;
  mtspr(SPR_TICK_TTMR_ADDR, 3<<30); // continous
  timer_kick();
}
DECL_INIT(timer_init);
