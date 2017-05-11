// Code to handle IO on PRU0 and pass the messages to PRU1
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

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
#include "compiler.h" // __section
#include "internal.h" // SHARED_MEM


/****************************************************************
 * IO
 ****************************************************************/

#define CHAN_NAME                                       "rpmsg-pru"
#define CHAN_DESC                                       "Channel 30"
#define CHAN_PORT                                       30

static uint16_t transport_dst;

static void
check_can_send(struct pru_rpmsg_transport *transport)
{
    for (;;) {
        uint32_t send_pop_pos = SHARED_MEM->send_pop_pos;
        uint32_t count = readl(&SHARED_MEM->send_data[send_pop_pos].count);
        if (!count)
            // Queue empty
            break;
        pru_rpmsg_send(
            transport, CHAN_PORT, transport_dst
            , &SHARED_MEM->send_data[send_pop_pos].data, count);
        barrier();
        writel(&SHARED_MEM->send_data[send_pop_pos].count, 0);
        SHARED_MEM->send_pop_pos = (
            (send_pop_pos + 1) % ARRAY_SIZE(SHARED_MEM->send_data));
    }
}

static void
check_can_read(struct pru_rpmsg_transport *transport)
{
    if (readl(&SHARED_MEM->read_count))
        // main processing pru is busy
        return;
    uint16_t dst, len;
    int16_t ret = pru_rpmsg_receive(
        transport, &transport_dst, &dst, SHARED_MEM->read_data, &len);
    if (ret || !len)
        return;
    SHARED_MEM->read_pos = 0;
    barrier();
    writel(&SHARED_MEM->read_count, len);
    write_r31(R31_WRITE_IRQ_SELECT | (KICK_PRU1_EVENT - R31_WRITE_IRQ_OFFSET));
}

static void
process_io(struct pru_rpmsg_transport *transport)
{
    for (;;) {
        if (!(read_r31() & (1 << (WAKE_PRU0_IRQ + R31_IRQ_OFFSET))))
            continue;
        CT_INTC.SECR0 = (1 << KICK_PRU0_FROM_ARM_EVENT) | (1 << KICK_PRU0_EVENT);
        check_can_send(transport);
        check_can_read(transport);
    }
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
    barrier();
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
    struct pru_rpmsg_transport transport;
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

    process_io(&transport);
}
