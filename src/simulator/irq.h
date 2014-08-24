#ifndef __SIMU_IRQ_H
#define __SIMU_IRQ_H
// Definitions for irq enable/disable on host simulator

#include <stdint.h>
#include "compiler.h" // barrier

extern uint8_t Interrupt_off;

static inline void irq_disable(void) {
    Interrupt_off = 1;
    barrier();
}

static inline void irq_enable(void) {
    barrier();
    Interrupt_off = 0;
}

static inline uint8_t irq_save(void) {
    uint8_t flag = Interrupt_off;
    irq_disable();
    return flag;
}

static inline void irq_restore(uint8_t flag) {
    barrier();
    Interrupt_off = flag;
}

#endif // irq.h
