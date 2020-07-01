#ifndef __PRU_INTERNAL_H
#define __PRU_INTERNAL_H
// Local definitions for PRU code

#include <stdint.h> // uint32_t
#include "command.h" // MESSAGE_MAX

#define IEP_EVENT                 7
#define KICK_ARM_EVENT            16
#define KICK_PRU0_FROM_ARM_EVENT  17
#define KICK_PRU0_EVENT           18
#define KICK_PRU1_EVENT           19

#define WAKE_PRU0_IRQ  0
#define WAKE_PRU1_IRQ  1
#define WAKE_ARM_IRQ   2

#define R31_WRITE_IRQ_SELECT (1<<5)
#define R31_WRITE_IRQ_OFFSET 16

#define ALT_PRU_PTR(ptr) ((typeof(ptr))((uint32_t)(ptr) ^ 0x2000))

// Layout of shared memory
struct shared_response_buffer {
    uint32_t count;
    uint8_t data[MESSAGE_MAX];
};
struct shared_mem {
    uint32_t signal;
    const struct command_parser *next_command;
    uint32_t next_command_args[16];
    uint32_t send_push_pos, send_pop_pos;
    struct shared_response_buffer send_data[4];
    const struct command_parser *command_index;
    uint32_t command_index_size;
    const struct command_parser *shutdown_handler;
    uint8_t read_data[512];
};

#define SIGNAL_PRU0_WAITING 0xefefefef
#define SIGNAL_PRU1_READY   0xabababab

#define SHARED_MEM ((struct shared_mem *)0x00013c00)

#endif // internal.h
