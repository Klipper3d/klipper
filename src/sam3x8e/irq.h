#ifndef __SAM3X8E_IRQ_H
#define __SAM3X8E_IRQ_H
// Definitions for irq enable/disable on ARM Cortex-M

static inline void irq_disable(void) {
    asm volatile("cpsid i" ::: "memory");
}

static inline void irq_enable(void) {
    asm volatile("cpsie i" ::: "memory");
}

typedef unsigned long irqstatus_t;

static inline irqstatus_t irq_save(void) {
    irqstatus_t flag;
    asm volatile("mrs %0, primask" : "=r" (flag) :: "memory");
    irq_disable();
    return flag;
}

static inline void irq_restore(irqstatus_t flag) {
    asm volatile("msr primask, %0" :: "r" (flag) : "memory");
}

#endif // irq.h
