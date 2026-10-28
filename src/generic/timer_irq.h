#ifndef __GENERIC_TIMER_IRQ_H
#define __GENERIC_TIMER_IRQ_H

uint32_t timer_dispatch_many(void);
uint32_t timer_dispatch_many_polling(void);

void timer_dispatch_irq_poll(void);
void timer_dispatch_task_poll(void);

#endif // timer_irq.h
