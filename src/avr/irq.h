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

typedef uint8_t irqstatus_t;

static inline irqstatus_t irq_save(void) {
    uint8_t flag = SREG;
    irq_disable();
    return flag;
}

static inline void irq_restore(irqstatus_t flag) {
    barrier();
    SREG = flag;
}

static inline void irq_wait(void) {
    asm("sei\n    nop\n    cli" : : : "memory");
}

static inline void irq_poll(void) {
}

#endif // irq.h
