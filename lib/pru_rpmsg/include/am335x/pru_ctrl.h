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

#ifndef _PRU_CTRL_H_
#define _PRU_CTRL_H_

/* PRU_CTRL register set */
typedef struct {

	/* PRU_CTRL_CTRL register bit field */
	union {
		volatile uint32_t CTRL;

		volatile struct {
			unsigned SOFT_RST_N : 1;
			unsigned EN : 1;
			unsigned SLEEPING : 1;
			unsigned CTR_EN : 1;
			unsigned rsvd4 : 4;
			unsigned SINGLE_STEP : 1;
			unsigned rsvd9 : 6;
			unsigned RUNSTATE : 1;
			unsigned PCTR_RST_VAL : 16;
		} CTRL_bit;
	};	// 0x0


	/* PRU_CTRL_STS register bit field */
	union {
		volatile uint32_t STS;

		volatile struct {
			unsigned PCTR : 16;
			unsigned rsvd16 : 16;
		} STS_bit;
	};	// 0x4


	/* PRU_CTRL_WAKEUP_EN register bit field */
	union {
		volatile uint32_t WAKEUP_EN;

		volatile struct {
			unsigned BITWISE_ENS : 32;
		} WAKEUP_EN_bit;
	};	// 0x8


	/* PRU_CTRL_CYCLE register bit field */
	union {
		volatile uint32_t CYCLE;

		volatile struct {
			unsigned CYCLECOUNT : 32;
		} CYCLE_bit;
	};	// 0xC


	/* PRU_CTRL_STALL register bit field */
	union {
		volatile uint32_t STALL;

		volatile  struct {
			unsigned STALLCOUNT : 32;
		} STALL_bit;
	};	// 0x10


	uint32_t rsvd14[3];	// 0x14 - 0x1C


	/* PRU_CTRL_CTBIR0 register bit field */
	union {
		volatile uint32_t CTBIR0;

		volatile struct {
			unsigned C24_BLK_IDX : 8;
			unsigned rsvd8 : 8;
			unsigned C25_BLK_IDX : 8;
			unsigned rsvd24 : 8;
		} CTBIR0_bit;
	};	// 0x20


	/* PRU_CTRL_CTBIR1 register bit field */
	union {
		volatile uint32_t CTBIR1;

		volatile struct {
			unsigned C26_BLK_IDX : 8;
			unsigned rsvd8 : 8;
			unsigned C27_BLK_IDX : 8;
			unsigned rsvd24 : 8;
		} CTBIR1_bit;
	};	// 0x24


	/* PRU_CTRL_CTPPR0 register bit field */
	union {
		volatile uint32_t CTPPR0;

		volatile struct {
			unsigned C28_BLK_POINTER : 16;
			unsigned C29_BLK_POINTER : 16;
		} CTPPR0_bit;
	};	// 0x28


	/* PRU_CTRL_CTPPR1 register bit field */
	union {
		volatile uint32_t CTPPR1;

		volatile struct {
			unsigned C30_BLK_POINTER : 16;
			unsigned C31_BLK_POINTER : 16;
		} CTPPR1_bit;
	};	// 0x2C

} pruCtrl;

/* Definition of control register structures. */
#define PRU0_CTRL (*((volatile pruCtrl*)0x22000))
#define PRU1_CTRL (*((volatile pruCtrl*)0x24000))

#endif /* _PRU_CTRL_H_ */
