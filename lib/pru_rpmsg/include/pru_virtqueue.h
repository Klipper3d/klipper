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
*  File	:	pru_virtqueue.h
*
*  Summary	:	A virtual queue interface to simplify vring usage.
*
*  Notes	:
*  - This file implements the vring functions needed by the PRU core
*  - The PRU core is considered the slave and the ARM core is considered the
*    host
*  - The ARM host always adds *available* buffers to send/receive, while the
*    PRU slave always adds *used* buffers to send/receive.
*  - The logic for the PRU side is summarized below:
*
*    PRU Slave:
*    - To receive buffer from the ARM host:
*          virtqueue_getAvailBuf(vq_slave);
*              >> empty data from buf <<
*          virtqueue_addUsedBuf(vq_slave);
*          virtqueue_kick(vq_slave);
*    - To send buffer to the host:
*          virtqueue_getAvailBuf(vq_host);
*              >> copy data into buf <<
*          virtqueue_addUsedBuf(vq_host);
*          virtqueue_kick(vq_host);
*/

#ifndef _PRU_VIRTQUEUE_H_
#define _PRU_VIRTQUEUE_H_

#include <rsc_types.h>
#include <pru_virtio_ring.h>

/* Return value indicating no kick was sent */
#define PRU_VIRTQUEUE_NO_KICK			1
/* Return value indicating success */
#define PRU_VIRTQUEUE_SUCCESS			0
/* Return value indicating there were no available buffers */
#define PRU_VIRTQUEUE_NO_BUF_AVAILABLE		-1
/* Return value indicating that an invalid head index was given */
#define PRU_VIRTQUEUE_INVALID_HEAD		-2

/**
 * Summary	:	pru_virtqueue is a structure that encapsulates everything
 *			needed for a 'virtual queue'. This structure wraps a vring
 *			with extra information that is needed by the application
 *			in order to use the vring.
 *
 * Variables	:	id: The notification ID of the vring. 
 *			to_arm_event: The PRU-ICSS system event that signals the ARM.
 *			from_arm_event: The PRU-ICSS system event that the ARM uses to
 *					signal the PRU.
 *			last_avail_idx: A local running counter that is used by the
 *					PRU to determine whether or not a new
 *					available buffer has been added to the
 *					vring.
 *			vring: The underlying virtio structure that is being used
 *			       to pass buffers back and forth between the ARM and
 *			       PRU. See pru_virtio_ring.h.
 */
struct pru_virtqueue {
	uint32_t	id;
	uint32_t	to_arm_event;
	uint32_t	from_arm_event;
	uint16_t 	last_avail_idx;
	struct vring 	vring;
};

/**
* Summary	:	pru_virtqueue_init initializes the pru_virtqueue structure
*			with values from the resource table.
*
* Parameters	:	vq: a pointer to a pru_virtqueue structure that will be
*			    initialized
*			vring: a pointer to a vring that is populated and returned
*			       by the ARM host through the resource table (the id,
*			       number of descriptors, address of the vring, and
*			       alignment information are contained in this vring
*			       pointer's structure
*			to_arm_event: the PRU-ICSS system event to trigger in order to
*				      'kick' the ARM host when sending data
*			from_arm_event: the PRU-ICSS system event to check
*					for data arriving from the ARM host
*
* Description	:	This function initializes the pru_virtqueue (vq) with input
*			values from the vring in the resource table. This function
*			should be called once for each virtqueue/vring. After
*			initialization the pru_virtqueue pointer, vq, should be
*			passed to the other functions in this header file.
*
* Return Value	:	No return value.
*/
void pru_virtqueue_init(
	struct pru_virtqueue 		*vq,
	struct fw_rsc_vdev_vring 	*vring,
	uint32_t 			to_arm_event,
	uint32_t 			from_arm_event
);

/**
* Summary	:	pru_virtqueue_get_avail_buf - gets the next available
*			buffer from the pru_virtqueue specified in vq.
*
* Parameters	:	vq: pointer to the pru_virtqueue from which the available
*			    buffer should be retrieved
*			buf: pointer to be filled with the address of the available
*			     buffer
*			len: pointer to be filled with the length of the available
*			     buffer
*
* Description	:	This function compares our last_avail_idx running counter
*			against the vring.avail->idx value to see if there is a
*			buffer available that we have not used. If our last
*			available index running counter matches the vring.avail->idx
*			value then there have been no new available buffers added
*			by the host. If the two indices do not match then the host
*			has added new buffers and and we can set @buf to point to
*			the available buffer and @len to match the available buffers
*			length. If an available buffer is found we increment out
*			last_avail_idx to show that we used another buffer.
*
* Return Value	:	PRU_VIRTQUEUE_NO_BUF_AVAILABLE if no buffer available.
*			Returns the vring.desc index of the available buffer
*			otherwise.
*/
int16_t pru_virtqueue_get_avail_buf(
    struct pru_virtqueue	*vq,
    void			**buf,
    uint32_t			*len
);

/**
* Summary	:	pru_virtqueue_add_used_buf adds a used buffer to the
*			pru_virtqueue specified in vq.
*
* Parameters	:	vq: pointer to the pru_virtqueue where the used buffer
*			    should be added
*			head: vring.desc[] index of the used buffer
*			len: length of the used buffer being added
*
* Description	:	This function makes sure that the head vring.desc index
*			(head) is a valid index. If the index is valid, then the
*			buffer is added to the used list in the vring contained by
*			the pru_virtqueue (vq).
*
* Return Value	:	PRU_VIRTQUEUE_INVALID_HEAD if head is an invalid index for
*			the vring.desc array. Returns PRU_VIRTQUEUE_SUCCESS
*			otherwise.
*/
int16_t pru_virtqueue_add_used_buf(
    struct pru_virtqueue	*vq,
    int16_t			head,
    uint32_t			len
);

/**
* Summary	:	pru_virtqueue_kick sends a notification to the remote
*			processor that the PRU has added a buffer to the
*			pru_virtqueue.
*
* Parameters	:	vq: pointer to the pru_virtqueue that is to be kicked
*
* Description	:	This function is used by the PRU to notify the ARM host in
*			two situations:
*				1.	That the PRU has consumed a buffer that the ARM
*					host sent through the slave pru_virtqueue
*				2.	That the PRU has sent a buffer to the ARM through
*					the host pru_virtqueue
*			If the pru_virtqueue's VRING_AVAIL_F_NO_INTERRUPT flag is
*			set then the pru does not kick the pru_virtqueue.
*
* Return Value	:	PRU_VIRTQUEUE_NO_KICK if the VRING_AVAIL_F_NO_INTERRUPT
*			flag is set or PRU_VIRTQUEUE_SUCCESS otherwise.
*/
int16_t pru_virtqueue_kick(
    struct pru_virtqueue	*vq
);

#endif /* _PRU_VIRTQUEUE_H_ */
