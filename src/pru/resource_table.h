/****************************************************************
 * Resource table
 ****************************************************************/

#ifndef _RESOURCE_TABLE_H_
#define _RESOURCE_TABLE_H_

#include <stddef.h>
#include "pru_virtio_ids.h"

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

struct my_resource_table {
    struct resource_table base;

    uint32_t offset[1]; /* Should match 'num' in actual definition */

    /* rpmsg vdev entry */
    struct fw_rsc_vdev rpmsg_vdev;
    struct fw_rsc_vdev_vring rpmsg_vring0;
    struct fw_rsc_vdev_vring rpmsg_vring1;

};

#if !defined(__GNUC__)
  #pragma DATA_SECTION(resourceTable, ".resource_table")
  #pragma RETAIN(resourceTable)
  #define __resource_table      /* */
#else
  #define __resource_table __attribute__((section(".resource_table")))
#endif

struct my_resource_table resourceTable __resource_table = {
    {
        1,              /* Resource table version: only version 1 is
                         * supported by the current driver */
        1,              /* number of entries in the table */
        {0, 0},       /* reserved, must be zero */
    },
    /* offsets to entries */
    {
        offsetof(struct my_resource_table, rpmsg_vdev),
    },

    /* rpmsg vdev entry */
    {
        (uint32_t)TYPE_VDEV,            //type
        (uint32_t)VIRTIO_ID_RPMSG,      //id
        (uint32_t)0,                    //notifyid
        (uint32_t)RPMSG_PRU_C0_FEATURES,//dfeatures
        (uint32_t)0,                    //gfeatures
        (uint32_t)0,                    //config_len
        (uint8_t)0,                     //status
        (uint8_t)2,                     //num_of_vrings, only two is supported
        {(uint8_t)0, (uint8_t)0 },      //reserved
        /* no config data */
    },
    /* the two vrings */
    {
        FW_RSC_ADDR_ANY,    //da, will be populated by host, can't pass it in
        16,                 //align (bytes),
        PRU_RPMSG_VQ0_SIZE, //num of descriptors
        0,                  //notifyid, will be populated, can't pass right now
        0                   //reserved
    },
    {
        FW_RSC_ADDR_ANY,    //da, will be populated by host, can't pass it in
        16,                 //align (bytes),
        PRU_RPMSG_VQ1_SIZE, //num of descriptors
        0,                  //notifyid, will be populated, can't pass right now
        0                   //reserved
    },
};

#endif /* _RESOURCE_TABLE_H_ */
