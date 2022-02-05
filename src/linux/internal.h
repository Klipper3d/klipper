#ifndef __LINUX_INTERNAL_H
#define __LINUX_INTERNAL_H
// Local definitions for micro-controllers running on linux

#include <signal.h> // sigset_t
#include <stdint.h> // uint32_t
#include "autoconf.h" // CONFIG_CLOCK_FREQ

#define MAX_GPIO_LINES    256
#define GPIO(PORT, NUM) ((PORT) * MAX_GPIO_LINES + (NUM))
#define GPIO2PORT(PIN) ((PIN) / MAX_GPIO_LINES)
#define GPIO2PIN(PIN) ((PIN) % MAX_GPIO_LINES)

#define NSECS 1000000000
#define NSECS_PER_TICK (NSECS / CONFIG_CLOCK_FREQ)

// console.c
void report_errno(char *where, int rc);
int set_non_blocking(int fd);
int set_close_on_exec(int fd);
int console_setup(char *name);
void console_sleep(sigset_t *sigset);

// timer.c
int timer_check_periodic(uint32_t *ts);
void timer_disable_signals(void);
void timer_enable_signals(void);

// watchdog.c
int watchdog_setup(void);

#endif // internal.h
