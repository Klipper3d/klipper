#include <stdint.h>
#include "asm/spr.h"
#include "sched.h"

static inline unsigned long mfspr(unsigned long add){
    unsigned long ret;
    __asm__ __volatile__ ("l.mfspr %0,r0,%1" : "=r" (ret) : "K" (add));
    return ret;
}

static inline void mtspr(unsigned long add, unsigned long val){
    __asm__ __volatile__ ("l.mtspr r0,%1,%0" : : "K" (add), "r" (val));
}

uint8_t timer_interrupt_pending(void);
void timer_set(uint32_t value);
uint32_t timer_read_time(void);
void timer_reset(void);
void timer_clear_interrupt(void);
void timer_kick(void);
void timer_init(void);
