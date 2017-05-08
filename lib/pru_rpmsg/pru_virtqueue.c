/*
 * Copyright (C) 2016 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the
 *	  distribution.
 *
 *	* Neither the name of Texas Instruments Incorporated nor the names of
 *	  its contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *  File	:	pru_virtqueue.c
 *
 *  Summary	:	A virtual queue implementation to simplify vring usage.
 *
 *  Notes	:
 *  - Implementaion of the interface described in "pru_virtqueue.h"
 */
#include <pru_virtqueue.h>

#ifndef __GNUC__
volatile register uint32_t __R31;
#else
#include <pru/io.h>
#endif

/* bit 5 is the valid strobe to generate system events with __R31 */
#define INT_ENABLE	(1 << 5)

/* __R31[3:0] can generate 15-0 which maps to system events 31-16 
 * e.g. to generate PRU-ICSS System Event 17 (pru_mst_intr[1])
 * __R31 = (INT_ENABLE | (17 - INT_OFFSET));
 */
#define INT_OFFSET	16

void pru_virtqueue_init(
	struct pru_virtqueue 		*vq,
	struct fw_rsc_vdev_vring 	*vring,
	uint32_t 			to_arm_event,
	uint32_t 			from_arm_event
)
{
	vq->id = vring->notifyid;
	vq->to_arm_event = to_arm_event;
	vq->from_arm_event = from_arm_event;
	vq->last_avail_idx = 0;

	vring_init(&vq->vring, vring->num, (void*)vring->da, vring->align);
}

int16_t pru_virtqueue_get_avail_buf(
    struct pru_virtqueue	*vq,
    void			**buf,
    uint32_t			*len
)
{
	int16_t			head;
	struct vring_desc	desc;
	struct vring_avail	*avail;

	avail = vq->vring.avail;

	/* There's nothing available */
	if (vq->last_avail_idx == avail->idx)
		return PRU_VIRTQUEUE_NO_BUF_AVAILABLE;

	/*
	 * Grab the next descriptor number the ARM host is advertising, and
	 * increment the last available index we've seen.
	 */
	head = avail->ring[vq->last_avail_idx++ & (vq->vring.num - 1)];

	desc = vq->vring.desc[head];
	*buf = (void *)(uint32_t)desc.addr;
	*len = desc.len;

	return (head);
}

int16_t pru_virtqueue_add_used_buf(
    struct pru_virtqueue	*vq,
    int16_t			head,
    uint32_t			len
)
{
	struct vring_used_elem	*used_elem;
	uint32_t		num;
	struct vring_used	*used;

	num = vq->vring.num;
	used = vq->vring.used;

	if (head > (int)num)
		return PRU_VIRTQUEUE_INVALID_HEAD;

	/*
	 * The virtqueue's vring contains a ring of used buffers.  Get a pointer to
	 * the next entry in that used ring.
	 */
	used_elem = &used->ring[used->idx++ & (num - 1)];
	used_elem->id = head;
	used_elem->len = len;

	return PRU_VIRTQUEUE_SUCCESS;
}

int16_t pru_virtqueue_kick(
    struct pru_virtqueue	*vq
)
{
	unsigned int r31;

	/* If requested, do not kick the ARM host */
	if (vq->vring.avail->flags & VRING_AVAIL_F_NO_INTERRUPT)
		return PRU_VIRTQUEUE_NO_KICK;

	/* Generate a system event to kick the ARM */
	r31 = (INT_ENABLE | (vq->to_arm_event - INT_OFFSET)); 
#ifdef __GNUC__
	write_r31(r31);
#else
	__R31 = r31;
#endif

	return PRU_VIRTQUEUE_SUCCESS;
}
