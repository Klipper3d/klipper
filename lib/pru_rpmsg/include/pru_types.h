/*
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
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

#ifndef _PRU_TYPES_H_
#define _PRU_TYPES_H_

/* Custom Resource info: Must match drivers/remoteproc/pru_rproc.h */
#define TYPE_PRU_INTS		1

/**
 * struct ch_map - sysevts-to-channel mapping
 *
 * @evt: the number of the sysevt
 * @ch: channel number assigned to a given @sysevt
 *
 * PRU system events are mapped to channels, and these channels are mapped to
 * hosts. Events can be mapped to channels in a one-to-one or many-to-one ratio
 * (multiple events per channel), and channels can be mapped to hosts in a
 * one-to-one or many-to-one ratio (multiple events per channel).
 *
 * @evt is the number of the sysevt, and @ch is the number of the channel to be
 * mapped.
 */

struct ch_map {
	uint8_t evt;
	uint8_t ch;
};

/**
 * struct fw_rsc_custom_ints - custom resource to define PRU interrupts
 * @version: revision number of the custom ints type
 * @channel_host: assignment of PRU channels to hosts
 * @num_evts: device address of INTC
 * @event_channel: mapping of sysevts to channels
 *
 * PRU system events are mapped to channels, and these channels are mapped to
 * hosts. Events can be mapped to channels in a one-to-one or many-to-one ratio
 * (multiple events per channel), and channels can be mapped to hosts in a
 * one-to-one or many-to-one ratio (multiple events per channel).
 *
 * @da is the device address of the interrupt controller, @channel_map is
 * used to specify to which channel, if any, an event is mapped, and @host_map
 * specifies to which host, if any, a channel is mapped.
 */
struct fw_rsc_custom_ints {
	uint16_t version;
	uint8_t channel_host[10];
	uint32_t num_evts;
	struct ch_map *event_channel;
};

#endif /* _PRU_TYPES_H_ */
