#ifndef __LINUX_INTERNAL_H
#define __LINUX_INTERNAL_H
// Local definitions for micro-controllers running on linux

#include <time.h> // struct timespec

// console.c
int console_setup(char *name);
void console_sleep(struct timespec ts);

// timer.c
int timer_check_periodic(struct timespec *ts);

// watchdog.c
int watchdog_setup(void);

#endif // internal.h
