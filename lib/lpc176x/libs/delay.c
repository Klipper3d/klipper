#include "delay.h"
#include <cmsis_nvic.h>

static inline void __delayMicroseconds(uint32_t const) __attribute__((always_inline, unused));
static inline void __delayMicroseconds(uint32_t const usec){
    /*
     * Based on Paul Stoffregen's implementation
     * for Teensy 3.0 (http://www.pjrc.com/)
     */
    if (usec == 0) return;
    uint32_t n = usec * (SystemCoreClock / 3000000);
    asm volatile(
        "L_%=_delayMicroseconds:"       "\n\t"
        "subs   %0, #1"                 "\n\t"
        "bne    L_%=_delayMicroseconds" "\n"
        : "+r" (n) :
    );
}

void DELAY_s(uint32_t const delay) {
  __delayMicroseconds(1000*1000*delay);
}

void DELAY_ms(uint32_t const delay) {
  __delayMicroseconds(1000*delay);
}

void DELAY_us(uint32_t const delay) {
  __delayMicroseconds(delay);
}
