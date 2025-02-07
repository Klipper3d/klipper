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

/* Host-0 Interrupt sets bit 31 in register R31 */
#define HOST_INT_0 ((uint32_t) 1 << 30)

/* Host-1 Interrupt sets bit 31 in register R31 */
#define HOST_INT_1 ((uint32_t) 1 << 31)

#define R31_IRQ_OFFSET 30

#define R31_WRITE_IRQ_SELECT (1<<5)
#define R31_WRITE_IRQ_OFFSET 16

#define ALT_PRU_PTR(ptr) ((typeof(ptr))((uint32_t)(ptr) ^ 0x2000))

// Layout of shared memory
struct shared_mem {
    uint32_t signal;
    void *next_encoder_args;
    uint32_t next_encoder;
    const struct command_parser *next_command;
    uint32_t next_command_args[16];
    const struct command_parser *command_index;
    uint32_t command_index_size;
    const struct command_parser *shutdown_handler;
    uint8_t read_data[512];
};

#define SIGNAL_PRU0_WAITING 0xefefefef
#define SIGNAL_PRU1_READY   0xabababab

#define SHARED_MEM ((struct shared_mem *)0x10000)

// Hardware ADC registers
struct beaglebone_adc {
    uint32_t pad_00[10];
    uint32_t irqstatus;
    uint32_t irqenable_set;
    uint32_t irqenable_clr;
    uint32_t irqwakeup;
    uint32_t dmaenable_set;
    uint32_t dmaenable_clr;
    uint32_t ctrl;
    uint32_t adcstat;
    uint32_t adcrange;
    uint32_t adc_clkdiv;
    uint32_t adc_misc;
    uint32_t stepenable;
    uint32_t idleconfig;
    uint32_t ts_charge_stepconfig;
    uint32_t ts_charge_delay;
    struct {
        uint32_t config;
        uint32_t delay;
    } step[16];
    uint32_t fifo0count;
    uint32_t pad_e8[6];
    uint32_t fifo0data;
    uint32_t pad_104[63];
    uint32_t fifo1data;
};

#define ADC ((struct beaglebone_adc *)0x44e0d000)

#endif // internal.h
