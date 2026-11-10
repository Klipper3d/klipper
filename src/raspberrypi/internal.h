#ifndef RASPBERRYPI_INTERNAL_H_
#define RASPBERRYPI_INTERNAL_H_
// Local definitions for micro-controllers running on linux

#include <signal.h> // sigset_t
#include <stdint.h> // uint32_t

// console.c
void report_errno(char *where, int rc);
int set_non_blocking(int fd);
int set_close_on_exec(int fd);
int console_setup(char *name);
void console_sleep();

// timer.c
int timer_check_periodic(uint32_t *ts);

#endif // RASPBERRYPI_INTERNAL_H_
