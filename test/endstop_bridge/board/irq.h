#ifndef TEST_IRQ_H
#define TEST_IRQ_H
typedef unsigned irqstatus_t;
static irqstatus_t irq_save(void) { return 0; }
static void irq_restore(irqstatus_t flag) {}
static void irq_disable(void) {}
static void irq_enable(void) {}
#endif
