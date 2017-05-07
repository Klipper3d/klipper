// PRU input/output via RPMsg
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include <string.h> // memcpy
#include <pru/io.h> // read_r31
#include <pru_intc.h> // CT_INTC
#include <pru_rpmsg.h> // pru_rpmsg_send
#include <pru_virtio_ids.h> // VIRTIO_ID_RPMSG
#include "board/misc.h" // console_get_input
#include "internal.h" // WAKE_ARM_EVENT
#include "sched.h" // DECL_INIT

#define CHAN_NAME                                       "rpmsg-pru"
#define CHAN_DESC                                       "Channel 30"
#define CHAN_PORT                                       30


/****************************************************************
 * Console interface
 ****************************************************************/

static void
clear_pending(void)
{
    CT_INTC.SECR0 = 1 << GOT_ARM_EVENT;
}

static struct pru_rpmsg_transport transport;
static uint16_t transport_dst;
static char input_data[64];

// XXX
struct pru_rpmsg_hdr {
    uint32_t src;
    uint32_t dst;
    uint32_t reserved;
    uint16_t len;
    uint16_t flags;
    uint8_t  data[0];
};

// Return a buffer (and length) containing any incoming messages
char *
console_get_input(uint8_t *plen)
{
    if (!(read_r31() & (1 << (GOT_ARM_IRQ + R31_IRQ_OFFSET))))
        goto nodata;
    struct pru_rpmsg_hdr *msg;
    uint32_t msg_len;
    int16_t head = pru_virtqueue_get_avail_buf(
        &transport.virtqueue1, (void**)&msg, &msg_len);
    if (head < 0) {
        clear_pending();
        goto nodata;
    }
    transport_dst = msg->src;
    int len = msg->len < sizeof(input_data) ? msg->len : sizeof(input_data);
    memcpy(input_data, msg->data, len);
    pru_virtqueue_add_used_buf(&transport.virtqueue1, head, msg_len);
    pru_virtqueue_kick(&transport.virtqueue1);
    *plen = len;
    return input_data;
nodata:
    *plen = 0;
    return input_data;
}

// Remove from the receive buffer the given number of bytes
void
console_pop_input(uint8_t len)
{
}

static char output_data[64];

// Return an output buffer that the caller may fill with transmit messages
char *
console_get_output(uint8_t len)
{
    if (len > sizeof(output_data))
        return NULL;
    return output_data;
}

// Accept the given number of bytes added to the transmit buffer
void
console_push_output(uint8_t len)
{
    pru_rpmsg_send(&transport, CHAN_PORT, transport_dst, output_data, len);
}


/****************************************************************
 * resource table
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
    {IEP_EVENT, IEP_IRQ},
    {WAKE_ARM_EVENT, WAKE_ARM_IRQ},
    {GOT_ARM_EVENT, GOT_ARM_IRQ},
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
                    IEP_IRQ, GOT_ARM_IRQ, WAKE_ARM_IRQ,
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
 * RPMsg init
 ****************************************************************/

#define VIRTIO_CONFIG_S_DRIVER_OK       4

void
console_init(void)
{
    clear_pending();

    /* Make sure the Linux drivers are ready for RPMsg communication */
    volatile uint8_t *status = &resourceTable.rpmsg_vdev.status;
    while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK))
        ;

    /* Initialize the RPMsg transport structure */
    pru_rpmsg_init(&transport,
                   &resourceTable.rpmsg_vring0,
                   &resourceTable.rpmsg_vring1,
                   WAKE_ARM_EVENT,
                   GOT_ARM_EVENT);

    /* Create the RPMsg channel between the PRU and ARM user space
     * using the transport structure. */
    while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME
                             , CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS)
        ;
}
