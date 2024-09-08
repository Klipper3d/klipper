// Code to handle IO on PRU0 and pass the messages to PRU1
//
// Copyright (C) 2017-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <setjmp.h> // setjmp
#include <stdint.h> // uint32_t
#include <string.h> // memset
#include <pru/io.h> // write_r31
#include <pru_rpmsg.h> // pru_rpmsg_send
#include <pru_virtio_ids.h> // VIRTIO_ID_RPMSG
#include "board/io.h" // readl
#include "board/misc.h" // console_sendf
#include "command.h" // command_encode_add_frame
#include "compiler.h" // __section
#include "internal.h" // SHARED_MEM
#include "sched.h" // sched_shutdown
#include "intc_map_0.h"
#include "resource_table.h"

struct pru_rpmsg_transport transport;
static uint16_t transport_dst;


/****************************************************************
 * IO
 ****************************************************************/

#define CHAN_NAME                                       "rpmsg-pru"
#define CHAN_DESC                                       "Channel 30"
#define CHAN_PORT                                       30

static uint8_t transmit_buf[RPMSG_MESSAGE_SIZE];
static int transmit_pos;

// Transmit all pending message blocks
static void
flush_messages(void)
{
    if (!transmit_pos)
        return;
    pru_rpmsg_send(&transport, CHAN_PORT, transport_dst
                   , transmit_buf, transmit_pos);
    transmit_pos = 0;
}

// Verify space for a message block
static uint8_t *
get_transmit_ptr(const struct command_encoder *ce)
{
    if (transmit_pos + ce->max_size > sizeof(transmit_buf))
        flush_messages();
    return &transmit_buf[transmit_pos];
}

// Finalize a message block and queue it for transmission
static void
finalize_transmit(int msglen)
{
    transmit_pos += msglen;
}

// Check if there is data to be sent from PRU1 to the host
static void
check_can_send(void)
{
    for (;;) {
        uint32_t rce = readl(&SHARED_MEM->next_encoder);
        if (!rce)
            break;
        // Copy va_args on pru1 for use on pru0
        void *pru1_args = ALT_PRU_PTR(SHARED_MEM->next_encoder_args);
        uint32_t local_args[16];
        memcpy(local_args, pru1_args, sizeof(local_args));
        const struct command_encoder *pru1_ce = ALT_PRU_PTR((void*)rce);
        struct command_encoder ce;
        memcpy(&ce, pru1_ce, sizeof(ce));
        if (readl(&SHARED_MEM->next_encoder) != rce)
            continue;
        // Fixup any pointers in va_args
        ce.param_types = ALT_PRU_PTR(ce.param_types);
        uint32_t pos=0, v, i, count = ce.num_params;
        for (i=0; i<count; i++) {
            switch (ce.param_types[i]) {
            case PT_progmem_buffer:
            case PT_buffer:
                pos++;
                // NO BREAK
            case PT_string:
                v = local_args[pos];
                if (v < 0x2000)
                    // Translate pointer
                    local_args[pos] = (uint32_t)ALT_PRU_PTR((void*)v);
            }
            pos++;
        }
        // Encode and build message
        uint8_t *data = get_transmit_ptr(&ce);
        int msglen = command_encode_and_frame(data, &ce, (void*)local_args);
        if (readl(&SHARED_MEM->next_encoder) != rce)
            continue;
        writel(&SHARED_MEM->next_encoder, 0);
        finalize_transmit(msglen);
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
do_dispatch(uint8_t *buf, uint32_t msglen)
{
    uint8_t *p = &buf[MESSAGE_HEADER_SIZE];
    uint8_t *msgend = &buf[msglen-MESSAGE_TRAILER_SIZE];
    while (p < msgend) {
        // Parse command
        uint_fast16_t cmdid = command_parse_msgid(&p);
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
static int
check_can_read(void)
{
    // Read data
    uint16_t dst, len;
    uint8_t *p = SHARED_MEM->read_data;
    int16_t ret = pru_rpmsg_receive(&transport, &transport_dst, &dst, p, &len);
    if (ret)
        return ret == PRU_RPMSG_NO_BUF_AVAILABLE;

    // Check for force shutdown request
    if (len == 15 && p[14] == '\n' && memcmp(p, "FORCE_SHUTDOWN\n", 15) == 0) {
        send_pru1_shutdown();
        return 0;
    }

    // Parse data into message blocks
    for (;;) {
        uint_fast8_t pop_count, msglen = len > MESSAGE_MAX ? MESSAGE_MAX : len;
        int_fast8_t ret = command_find_block(p, msglen, &pop_count);
        if (!ret)
            break;
        if (ret > 0) {
            do_dispatch(p, pop_count);
            command_send_ack();
        }
        p += pop_count;
        len -= pop_count;
    }
    return 0;
}

// Main processing loop
static void
process_io(void)
{
    for (;;) {
        PRU_INTC.SECR0 = ((1 << KICK_PRU0_FROM_ARM_EVENT)
                         | (1 << KICK_PRU0_EVENT));
        check_can_send();
        int can_sleep = check_can_read();
        if (can_sleep) {
            flush_messages();
            while (!(read_r31() & (1 << (WAKE_PRU0_IRQ + R31_IRQ_OFFSET)))) {
                //asm("slp 1");
            }
        }
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
    uint8_t *data = get_transmit_ptr(ce);
    int msglen = command_encode_and_frame(data, ce, args);
    finalize_transmit(msglen);
}



/****************************************************************
 * Startup
 ****************************************************************/

#define VIRTIO_CONFIG_S_DRIVER_OK       4

int
main(void)
{
    // allow access to external memory
#if defined(__AM335X__)
    /* AM335x must enable OCP master port access in order for the PRU to
     * read external memories.*/
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
#endif

    /* Clear the status of the PRU-ICSS system event that the ARM
    will use to 'kick' us */
#if defined(__AM335X__)
    PRU_INTC.SICR_bit.STS_CLR_IDX = KICK_PRU0_FROM_ARM_EVENT;
#elif defined(__TDA4VM__) || defined(__AM62X__)
    PRU_INTC.STATUS_CLR_INDEX_REG_bit.STATUS_CLR_INDEX = \
    KICK_PRU0_FROM_ARM_EVENT;
#else
  #error "Unsupported SoC."
#endif

    // clear all irqs
    PRU_INTC.SECR0 = 0xffffffff;
    PRU_INTC.SECR1 = 0xffffffff;

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

    // Allow PRU0 and PRU1 to wake from sleep
    PRU0_CTRL.WAKEUP_EN = 1 << (WAKE_PRU0_IRQ + R31_IRQ_OFFSET);
    PRU1_CTRL.WAKEUP_EN = 1 << (WAKE_PRU1_IRQ + R31_IRQ_OFFSET);

    // Wait for PRU1 to be ready
    memset(SHARED_MEM, 0, sizeof(*SHARED_MEM));
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
