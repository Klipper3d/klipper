#ifndef __SCHED_H
#define __SCHED_H

#include <stdint.h>
#include "board/pgm.h" // PROGMEM
#include "compiler.h" // __section

// Declare an init function (called at firmware startup)
#define DECL_INIT(FUNC) _DECL_CALLBACK(initfuncs, FUNC)
// Declare a task function (called periodically during normal runtime)
#define DECL_TASK(FUNC) _DECL_CALLBACK(taskfuncs, FUNC)
// Declare a shutdown function (called on an emergency stop)
#define DECL_SHUTDOWN(FUNC) _DECL_CALLBACK(shutdownfuncs, FUNC)

// Timer structure for scheduling timed events (see sched_timer() )
struct timer {
    uint32_t waketime;
    void (*func)(struct timer*);
    struct timer *next;
};

// sched.c
uint32_t sched_from_ms(uint32_t ms);
uint8_t sched_is_past(uint32_t time1, uint32_t time2);
void sched_timer(struct timer*);
void sched_timer_kick(void);
uint8_t sched_is_shutdown(void);
#define shutdown(fmt, args...) \
    shutdown_P(PSTR(fmt) , ##args )
void shutdown_P(const char *fmt_P, ...)
    __attribute__ ((format (printf, 1, 2)));
void sched_kick(void);

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
