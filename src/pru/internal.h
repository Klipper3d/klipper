#ifndef __PRU_INTERNAL_H
#define __PRU_INTERNAL_H
// Local definitions for PRU code

#include <stdint.h> // uint32_t

#define IEP_EVENT                 7
#define KICK_ARM_EVENT            16
#define KICK_PRU0_FROM_ARM_EVENT  17
#define KICK_PRU0_EVENT           18
#define KICK_PRU1_EVENT           19

#define WAKE_PRU0_IRQ  0
#define WAKE_PRU1_IRQ  1
#define WAKE_ARM_IRQ   2

#define R31_IRQ_OFFSET 30

#define R31_WRITE_IRQ_SELECT (1<<5)
#define R31_WRITE_IRQ_OFFSET 16

// Layout of shared memory
struct shared_mem {
    uint32_t signal;
    uint32_t read_pos, read_count;
    char read_data[512];
    uint32_t send_push_pos, send_pop_pos;
    struct {
        uint32_t count;
        char data[64];
    } send_data[4];
};

#define SIGNAL_PRU0_WAITING 0xefefefef
#define SIGNAL_PRU1_READY   0xabababab

static struct shared_mem *SHARED_MEM = (void*)0x10000;

#endif // internal.h
