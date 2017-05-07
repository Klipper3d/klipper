#ifndef __PRU_INTERNAL_H
#define __PRU_INTERNAL_H
// Local definitions for PRU code

#define IEP_IRQ        0
#define GOT_ARM_IRQ    1
#define WAKE_ARM_IRQ   2

#define IEP_EVENT      7
#define GOT_ARM_EVENT  17
#define WAKE_ARM_EVENT 16

#define R31_IRQ_OFFSET 30

// console.c
void console_init(void);

#endif // internal.h
