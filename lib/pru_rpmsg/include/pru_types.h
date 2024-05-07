/*
 * Copyright (C) 2015-2021 Texas Instruments Incorporated - http://www.ti.com/
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

#define PRU_INTS_VER0		(0 << 16)
#define PRU_INTS_VER1		(1 << 16)

/**
 * struct pruss_int_map - sysevt to channel to host interrupt mapping
 *
 * @event: the number of the sysevt
 * @chnl: channel number assigned to a given @event
 * @host: the host interrupt assigned to a given @chnl
 *
 * PRU system events are mapped to channels, and these channels are mapped to
 * hosts. Events can be mapped to channels in a one-to-one or many-to-one ratio
 * (multiple events per channel), and channels can be mapped to hosts in a
 * one-to-one or many-to-one ratio (multiple events per channel).
 *
 * @event is the number of the sysevt, @chnl is the number of the channel to be
 * mapped, and @host is the number of the host interrupt to be mapped.
 *
 * pruss_int_map is defined in Linux at drivers/remoteproc/pru_rproc.h
 */
struct pruss_int_map {
	uint8_t event;
	uint8_t chnl;
	uint8_t host;
};

/**
 * struct pru_irq_rsc - PRU firmware section header for IRQ data
 * @type: resource type
 * @num_evts: number of described events
 * @pru_intc_map: PRU interrupt routing description
 *
 * The PRU firmware blob can contain optional .pru_irq_map ELF section, which
 * provides the PRUSS interrupt mapping description. The pru_irq_rsc struct
 * describes resource entry format.
 *
 * pru_irq_rsc is defined in Linux at drivers/remoteproc/pru_rproc.h
 */
struct pru_irq_rsc {
	uint8_t type;
	uint8_t num_evts;
	struct pruss_int_map pru_intc_map[];
};

/**
 * struct fw_rsc_custom_ints - custom resource to define PRU interrupts
 * @reserved: reserved field, value should be 0 for backward compatibility
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
	uint16_t reserved;
	uint8_t channel_host[10];
	uint32_t num_evts;
	struct ch_map *event_channel;
};

/**
 * struct fw_rsc_custom_ints_k3 - custom resource to define PRU/RTU interrupts
 * @channel_host: assignment of PRU interrupt channels to host interrupts
 * @num_evts: number of mappings defined in the @event_channel map
 * @event_channel: PRU device address of pointer to array of events to channel
 *                 mappings
 *
 * PRU system events are mapped to channels, and these channels are mapped
 * to host interrupts. Events can be mapped to channels in a one-to-one or
 * many-to-one ratio (multiple events per channel), and channels can be
 * mapped to host interrupts in a one-to-one or many-to-one ratio (multiple
 * channels per interrupt).
 *
 * This structure needs to be used using custom interrupt resource version
 * number 1. This structure is to be used with firmwares dealing with the
 * additional host interrupts on ICSSG IP instances. The firmwares for PRU
 * cores on ICSSG can get away with the standard version (if not dealing with
 * Task Manager), but the firmwares for RTU cores would definitely need this
 * for mapping to the corresponding higher host interrupts.
 */
struct fw_rsc_custom_ints_k3 {
	uint8_t channel_host[20];
	uint32_t num_evts;
	struct ch_map *event_channel;
};

#endif /* _PRU_TYPES_H_ */
