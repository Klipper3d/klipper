// Code to handle IO on PRU0 and pass the messages to PRU1
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <setjmp.h> // setjmp
#include <stdint.h> // uint32_t
#include <string.h> // memset
#include <pru/io.h> // write_r31
#include <pru_cfg.h> // CT_CFG
#include <pru_iep.h> // CT_IEP
#include <pru_intc.h> // CT_INTC
#include <pru_rpmsg.h> // pru_rpmsg_send
#include <pru_virtio_ids.h> // VIRTIO_ID_RPMSG
#include <rsc_types.h> // resource_table
#include "board/io.h" // readl
#include "command.h" // command_add_frame
#include "compiler.h" // __section
#include "internal.h" // SHARED_MEM
#include "sched.h" // sched_shutdown

struct pru_rpmsg_transport transport;
static uint16_t transport_dst;


/****************************************************************
 * IO
 ****************************************************************/

#define CHAN_NAME                                       "rpmsg-pru"
#define CHAN_DESC                                       "Channel 30"
#define CHAN_PORT                                       30

// Check if there is data to be sent from PRU1 to the host
static void
check_can_send(void)
{
    for (;;) {
        uint32_t send_pop_pos = SHARED_MEM->send_pop_pos;
        struct shared_response_buffer *s = &SHARED_MEM->send_data[send_pop_pos];
        uint32_t count = readl(&s->count);
        if (!count)
            // Queue empty
            break;
        command_add_frame(s->data, count);
        pru_rpmsg_send(&transport, CHAN_PORT, transport_dst, &s->data, count);
        writel(&s->count, 0);
        SHARED_MEM->send_pop_pos = (
            (send_pop_pos + 1) % ARRAY_SIZE(SHARED_MEM->send_data));
    }
}

// Wait for PRU1 to finish processing a command
static void
wait_pru1_command(void)
{
    while (readl(&SHARED_MEM->next_command))
        check_can_send();
    check_can_send();
}

// Signal PRU1 that a new command is ready
static void
send_pru1_command(const struct command_parser *cp)
{
    barrier();
    SHARED_MEM->next_command = cp;
    barrier();
    write_r31(R31_WRITE_IRQ_SELECT | (KICK_PRU1_EVENT - R31_WRITE_IRQ_OFFSET));
}

// Instruct PRU1 to shutdown
static void
send_pru1_shutdown(void)
{
    wait_pru1_command();
    send_pru1_command(SHARED_MEM->shutdown_handler);
    wait_pru1_command();
}

// Dispatch all the commands in a message block
static void
do_dispatch(char *buf, uint32_t msglen)
{
    char *p = &buf[MESSAGE_HEADER_SIZE];
    char *msgend = &buf[msglen-MESSAGE_TRAILER_SIZE];
    while (p < msgend) {
        // Parse command
        uint8_t cmdid = *p++;
        const struct command_parser *cp = &SHARED_MEM->command_index[cmdid];
        if (!cmdid || cmdid >= SHARED_MEM->command_index_size
            || cp->num_args > ARRAY_SIZE(SHARED_MEM->next_command_args)) {
            send_pru1_shutdown();
            return;
        }
        p = command_parsef(p, msgend, cp, SHARED_MEM->next_command_args);

        send_pru1_command(ALT_PRU_PTR(cp));
        wait_pru1_command();
    }
}

// See if there are commands from the host ready to be processed
static void
check_can_read(void)
{
    // Read data
    uint16_t dst, len;
    char *p = SHARED_MEM->read_data;
    int16_t ret = pru_rpmsg_receive(&transport, &transport_dst, &dst, p, &len);
    if (ret)
        return;

    // Check for force shutdown request
    if (len == 15 && p[14] == '\n' && memcmp(p, "FORCE_SHUTDOWN\n", 15) == 0) {
        send_pru1_shutdown();
        return;
    }

    // Parse data into message blocks
    for (;;) {
        uint8_t pop_count, msglen = len > 64 ? 64 : len;
        int8_t ret = command_find_block(p, msglen, &pop_count);
        if (!ret)
            break;
        if (ret > 0)
            do_dispatch(p, pop_count);
        p += pop_count;
        len -= pop_count;
    }
}

// Main processing loop
static void
process_io(void)
{
    for (;;) {
        if (!(read_r31() & (1 << (WAKE_PRU0_IRQ + R31_IRQ_OFFSET))))
            continue;
        CT_INTC.SECR0 = (1 << KICK_PRU0_FROM_ARM_EVENT) | (1 << KICK_PRU0_EVENT);
        check_can_send();
        check_can_read();
    }
}

// Startup initialization
static void
setup_io(void)
{
    // Fixup pointers in command_parsers
    SHARED_MEM->command_index = ALT_PRU_PTR(SHARED_MEM->command_index);
    struct command_parser *p = (void*)SHARED_MEM->command_index;
    int i;
    for (i=0; i<SHARED_MEM->command_index_size; i++, p++)
        if (p->param_types)
            p->param_types = ALT_PRU_PTR(p->param_types);
}


/****************************************************************
 * Compatibility wrappers
 ****************************************************************/

// shutdown() compatibility code
uint8_t ctr_lookup_static_string(const char *str)
{
    return 0;
}

static jmp_buf shutdown_jmp;

// Handle shutdown()
void
sched_shutdown(uint_fast8_t reason)
{
    longjmp(shutdown_jmp, 1);
}

// Generate messages - only used for ack/nak messages
void
console_sendf(const struct command_encoder *ce, va_list args)
{
    char buf[MESSAGE_MIN];
    command_add_frame(buf, sizeof(buf));
    pru_rpmsg_send(&transport, CHAN_PORT, transport_dst, buf, sizeof(buf));
}


/****************************************************************
 * Peripheral reset
 ****************************************************************/

static void
timer_reset(void)
{
    CT_IEP.TMR_CMP_CFG = 0x01 << 1;
    CT_IEP.TMR_GLB_CFG = 0x11;
}

static void
adc_reset(void)
{
    // Disable ADC
    ADC->ctrl = (1<<2);
    barrier();
    // Clear registers
    ADC->irqstatus = 0xffffffff;
    ADC->irqenable_clr = 0xffffffff;
    ADC->dmaenable_clr = 0xffffffff;
    ADC->adc_clkdiv = 0;
    ADC->stepenable = 0;
    ADC->idleconfig = 0;
    int i;
    for (i=0; i<8; i++) {
        ADC->step[i].config = i<<19;
        ADC->step[i].delay = 0;
    }
    // Enable ADC
    writel(&ADC->ctrl, 0x07);
    // Drain fifo
    while (readl(&ADC->fifo0count))
        readl(&ADC->fifo0data);
}


/****************************************************************
 * Resource table
 ****************************************************************/

/*
 * Sizes of the virtqueues (expressed in number of buffers supported,
 * and must be power of 2)
 */
#define PRU_RPMSG_VQ0_SIZE      16
#define PRU_RPMSG_VQ1_SIZE      16

/*
 * The feature bitmap for virtio rpmsg
 */
#define VIRTIO_RPMSG_F_NS       0               //name service notifications

/* This firmware supports name service notifications as one of its features */
#define RPMSG_PRU_C0_FEATURES   (1 << VIRTIO_RPMSG_F_NS)

/* Definition for unused interrupts */
#define HOST_UNUSED             255

/* Mapping sysevts to a channel. Each pair contains a sysevt, channel. */
static struct ch_map pru_intc_map[] = {
    {IEP_EVENT, WAKE_PRU1_IRQ},
    {KICK_ARM_EVENT, WAKE_ARM_IRQ},
    {KICK_PRU0_FROM_ARM_EVENT, WAKE_PRU0_IRQ},
    {KICK_PRU0_EVENT, WAKE_PRU0_IRQ},
    {KICK_PRU1_EVENT, WAKE_PRU1_IRQ},
};

struct my_resource_table {
    struct resource_table base;

    uint32_t offset[2]; /* Should match 'num' in actual definition */

    /* rpmsg vdev entry */
    struct fw_rsc_vdev rpmsg_vdev;
    struct fw_rsc_vdev_vring rpmsg_vring0;
    struct fw_rsc_vdev_vring rpmsg_vring1;

    /* intc definition */
    struct fw_rsc_custom pru_ints;
} resourceTable __section(".resource_table") = {
    {
        1,              /* Resource table version: only version 1 is
                         * supported by the current driver */
        2,              /* number of entries in the table */
        { 0, 0 },       /* reserved, must be zero */
    },
    /* offsets to entries */
    {
        offsetof(struct my_resource_table, rpmsg_vdev),
        offsetof(struct my_resource_table, pru_ints),
    },

    /* rpmsg vdev entry */
    {
        (uint32_t)TYPE_VDEV,                    //type
        (uint32_t)VIRTIO_ID_RPMSG,              //id
        (uint32_t)0,                            //notifyid
        (uint32_t)RPMSG_PRU_C0_FEATURES,        //dfeatures
        (uint32_t)0,                            //gfeatures
        (uint32_t)0,                            //config_len
        (uint8_t)0,                             //status
        (uint8_t)2,                             //num_of_vrings, only two is supported
        {(uint8_t)0, (uint8_t)0 },              //reserved
        /* no config data */
    },
    /* the two vrings */
    {
        0,                                      //da, will be populated by host, can't pass it in
        16,                                     //align (bytes),
        PRU_RPMSG_VQ0_SIZE,                     //num of descriptors
        0,                                      //notifyid, will be populated, can't pass right now
        0                                       //reserved
    },
    {
        0,                                      //da, will be populated by host, can't pass it in
        16,                                     //align (bytes),
        PRU_RPMSG_VQ1_SIZE,                     //num of descriptors
        0,                                      //notifyid, will be populated, can't pass right now
        0                                       //reserved
    },

    {
        TYPE_CUSTOM, TYPE_PRU_INTS,
        sizeof(struct fw_rsc_custom_ints),
        { /* PRU_INTS version */
            {
                0x0000,
                /* Channel-to-host mapping, 255 for unused */
                {
                    WAKE_PRU0_IRQ, WAKE_PRU1_IRQ, WAKE_ARM_IRQ,
                    HOST_UNUSED, HOST_UNUSED, HOST_UNUSED,
                    HOST_UNUSED, HOST_UNUSED, HOST_UNUSED, HOST_UNUSED
                },
                /* Number of evts being mapped to channels */
                (sizeof(pru_intc_map) / sizeof(struct ch_map)),
                /* Pointer to the structure containing mapped events */
                pru_intc_map,
            },
        },
    },
};


/****************************************************************
 * Startup
 ****************************************************************/

#define VIRTIO_CONFIG_S_DRIVER_OK       4

int
main(void)
{
    // allow access to external memory
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    // clear all irqs
    CT_INTC.SECR0 = 0xffffffff;
    CT_INTC.SECR1 = 0xffffffff;

    /* Make sure the Linux drivers are ready for RPMsg communication */
    volatile uint8_t *status = &resourceTable.rpmsg_vdev.status;
    while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK))
        ;

    /* Initialize the RPMsg transport structure */
    pru_rpmsg_init(&transport,
                   &resourceTable.rpmsg_vring0,
                   &resourceTable.rpmsg_vring1,
                   KICK_ARM_EVENT,
                   KICK_PRU0_FROM_ARM_EVENT);

    /* Create the RPMsg channel between the PRU and ARM user space
     * using the transport structure. */
    while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME
                             , CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS)
        ;

    // Reset peripherals
    memset(SHARED_MEM, 0, sizeof(*SHARED_MEM));
    timer_reset();
    adc_reset();

    // Wait for PRU1 to be ready
    writel(&SHARED_MEM->signal, SIGNAL_PRU0_WAITING);
    while (readl(&SHARED_MEM->signal) != SIGNAL_PRU1_READY)
        ;
    writel(&SHARED_MEM->signal, 0);

    // Setup incoming message parser
    setup_io();

    // Support shutdown
    int ret = setjmp(shutdown_jmp);
    if (ret)
        send_pru1_shutdown();

    // Main loop
    process_io();
}
