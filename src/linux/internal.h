#ifndef __LINUX_INTERNAL_H
#define __LINUX_INTERNAL_H
// Local definitions for micro-controllers running on linux

#include <time.h> // struct timespec

// console.c
void report_errno(char *where, int rc);
int set_non_blocking(int fd);
int console_setup(char *name);
void console_sleep(struct timespec ts);

#endif // internal.h
