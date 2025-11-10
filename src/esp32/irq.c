#include "irq.h"
#include "timer.h"

/**
 * As we're in a single core context we use critical sections to only disable
 * interrupts although ESP's FreeRTOS fork uses spinlocks for this.
 */
//static portMUX_TYPE klipper_spinlock = portMUX_INITIALIZER_UNLOCKED;

void irq_disable(void)
{
    //taskENTER_CRITICAL(&klipper_spinlock);
    // @todo implementar algún tipo de spinlock o delayed triggers en el timer, cosa de evitar disparar timers cuando estamos haciendo acciones críticas.
    // Y en irq_enable disparar lo que sea necesario
}

void irq_enable(void)
{
    //taskEXIT_CRITICAL(&klipper_spinlock);
}

irqstatus_t irq_save(void)
{
    irq_disable();
    return 0;
}

void irq_restore(irqstatus_t flag)
{
    irq_enable();
}

void irq_poll(void)
{
    //timer_dispatch_if_pending();
}

void irq_wait(void)
{
    irq_poll();
}
