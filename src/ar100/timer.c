
#include "timer.h"

uint8_t timer_interrupt_pending(void){
	return !!(mfspr(SPR_TICK_TTMR_ADDR) & (1<<28));
}
void timer_clear_interrupt(void){
  uint32_t current = mfspr(SPR_TICK_TTMR_ADDR);
  mtspr(SPR_TICK_TTMR_ADDR, current | (1<<28));
}
// Set the next timer wake up time
void timer_set(uint32_t value){
	mtspr(SPR_TICK_TTMR_ADDR, 3<<30 | 1<<29 | (0x00FFFFFF & value));
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
  mtspr(SPR_TICK_TTMR_ADDR, 3<<30 | 1<<29); // Interrupt enable + continous
}
DECL_INIT(timer_init);
