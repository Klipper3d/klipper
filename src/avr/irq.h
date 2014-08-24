#ifndef __AVR_IRQ_H
#define __AVR_IRQ_H
// Definitions for irq enable/disable on AVR

#include <avr/interrupt.h> // cli
#include "compiler.h" // barrier

static inline void irq_disable(void) {
    cli();
    barrier();
}

static inline void irq_enable(void) {
    barrier();
    sei();
}

static inline uint8_t irq_save(void) {
    uint8_t flag = SREG;
    irq_disable();
    return flag;
}

static inline void irq_restore(uint8_t flag) {
    barrier();
    SREG = flag;
}

#endif // irq.h
