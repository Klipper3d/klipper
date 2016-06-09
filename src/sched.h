#ifndef __SCHED_H
#define __SCHED_H

#include <stdint.h>
#include "board/pgm.h" // PSTR
#include "compiler.h" // __section

// Declare an init function (called at firmware startup)
#define DECL_INIT(FUNC) _DECL_CALLBACK(initfuncs, FUNC)
// Declare a task function (called periodically during normal runtime)
#define DECL_TASK(FUNC) _DECL_CALLBACK(taskfuncs, FUNC)
// Declare a shutdown function (called on an emergency stop)
#define DECL_SHUTDOWN(FUNC) _DECL_CALLBACK(shutdownfuncs, FUNC)

// Timer structure for scheduling timed events (see sched_timer() )
struct timer {
    struct timer *next;
    uint_fast8_t (*func)(struct timer*);
    uint32_t waketime;
};

enum { SF_DONE=0, SF_RESCHEDULE=1 };

// sched.c
uint8_t sched_check_periodic(uint16_t time, uint16_t *pnext);
uint32_t sched_from_us(uint32_t us);
uint32_t sched_read_time(void);
uint8_t sched_is_before(uint32_t time1, uint32_t time2);
void sched_timer(struct timer*);
void sched_del_timer(struct timer *del);
void sched_timer_kick(void);
uint8_t sched_is_shutdown(void);
uint16_t sched_shutdown_reason(void);
void sched_clear_shutdown(void);
void sched_try_shutdown(uint16_t reason);
void sched_shutdown(uint16_t reason) __noreturn;
void sched_main(void);

// Compiler glue for DECL_X macros above.
struct callback_handler {
    void (*func)(void);
};
#define _DECL_CALLBACK(NAME, FUNC)                                      \
    const struct callback_handler _DECL_ ## NAME ## _ ## FUNC __visible \
    __section(".progmem.data." __stringify(NAME) ) = { .func = FUNC }

#define foreachdecl(ITER, NAME)                                 \
    extern typeof(*ITER) NAME ## _start[], NAME ## _end[];      \
    for (ITER = NAME ## _start ; ITER < NAME ## _end ; ITER ++)

#endif // sched.h
