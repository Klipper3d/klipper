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

#ifndef _PRU_INTC_H_
#define _PRU_INTC_H_

/* PRU INTC register set */
typedef struct {

	/* PRU_INTC_REVID register bit field */
	union {
		volatile uint32_t REVID;

		volatile struct {
			unsigned REV_MINOR : 6;		// 5:0
			unsigned REV_CUSTOM : 2;	// 7:6
			unsigned REV_MAJOR : 3;		// 10:8
			unsigned REV_RTL : 5;		// 15:11
			unsigned REV_MODULE : 12;	// 27:16
			unsigned rsvd28 : 2;		// 29:28
			unsigned REV_SCHEME : 2;	// 31:30
		} REVID_bit;
	};	// 0x0


	/* PRU_INTC_CR register bit field */
	union {
		volatile uint32_t CR;

		volatile struct {
			unsigned rsvd0 : 2;		// 1:0
			unsigned NEST_MODE : 2;		// 3:2
			unsigned rsvd4 : 28;		// 31:4
		} CR_bit;
	};	// 0x4


	uint32_t rsvd8[2];		// 0x8 - 0xC


	/* PRU_INTC_GER register bit field */
	union {
		volatile uint32_t GER;

		volatile struct {
			unsigned EN_HINT_ANY : 1;	// 0
			unsigned rsvd1 : 31;		// 31:1
		} GER_bit;
	};	// 0x10


	uint32_t rsvd14[2];	// 0x14 - 0x18


	/* PRU_INTC_GNLR register bit field */
	union {
		volatile uint32_t GNLR;

		volatile struct {
			unsigned GLB_NEST_LEVEL : 9;	// 8:0
			unsigned rsvd9 : 22;		// 30:9
			unsigned AUTO_OVERRIDE : 1;	// 31
		} GNLR_bit;
	};	// 0x1C


	/* PRU_INTC_SISR register bit field */
	union {
		volatile uint32_t SISR;

		volatile struct {
			unsigned STS_SET_IDX : 10;	// 9:0
			unsigned rsvd10 : 22;		// 31:10
		} SISR_bit;
	};	// 0x20


	/* PRU_INTC_SICR register bit field */
	union {
		volatile uint32_t SICR;

		volatile struct {
			unsigned STS_CLR_IDX : 10;	// 9:0
			unsigned rsvd10 : 22;		// 31:10
		} SICR_bit;
	};	// 0x24


	/* PRU_INTC_EISR register bit field */
	union {
		volatile uint32_t EISR;

		volatile struct {
			unsigned EN_SET_IDX : 10;	// 9:0
			unsigned rsvd10 : 22;		// 31:10
		} EISR_bit;
	};	// 0x28


	/* PRU_INTC_EICR register bit field */
	union {
		volatile uint32_t EICR;

		volatile struct {
			unsigned EN_CLR_IDX : 10;	// 9:0
			unsigned rsvd10 : 22;		// 31:10
		} EICR_bit;
	};	// 0x2C


	uint32_t rsvd30;	// 0x30


	/* PRU_INTC_HIEISR register bit field */
	union {
		volatile uint32_t HIEISR;

		volatile struct {
			unsigned HINT_EN_SET_IDX : 4;	// 3:0
			unsigned rsvd4 : 28;		// 31:4
		} HIEISR_bit;
	};	// 0x34


	/* PRU_INTC_HIDISR register bit field */
	union {
		volatile uint32_t HIDISR;

		volatile struct {
			unsigned HINT_EN_CLR_IDX : 4;	// 3:0
			unsigned rsvd4 : 28;		// 31:4
		} HIDISR_bit;
	};	// 0x38


	uint32_t rsvd3C[17];	// 0x3C - 0x7C


	/* PRU_INTC_GPIR register bit field */
	union {
		volatile uint32_t GPIR;

		volatile struct {
			unsigned GLB_PRI_INTR : 10;	// 9:0
			unsigned rsvd10 : 21;		// 30:10
			unsigned GLB_NONE : 1;		// 31
		} GPIR_bit;
	};	// 0x80


	uint32_t rsvd84[95];	// 0x84 - 0x1FC


	/* PRU_INTC_SRSR0 register bit field */
	union {
		volatile uint32_t SRSR0;

		volatile struct {
			unsigned RAW_STS_31_0 : 32;	// 31:0
		} SRSR0_bit;
	};	// 0x200


	/* PRU_INTC_SRSR1 register bit field */
	union {
		volatile uint32_t SRSR1;

		volatile struct {
			unsigned RAW_STS_63_32 : 32;	// 31:0
		} SRSR1_bit;
	};	// 0x204


	uint32_t rsvd208[30];	// 0x208 - 0x27C


	/* PRU_INTC_SECR0 register bit field */
	union {
		volatile uint32_t SECR0;

		volatile struct {
			unsigned ENA_STS_31_0 : 32;	// 31:0
		} SECR0_bit;
	};	// 0x280


	/* PRU_INTC_SECR1 register bit field */
	union {
		volatile uint32_t SECR1;

		volatile struct {
			unsigned ENA_STS_63_32 : 32;	// 31:0
		} SECR1_bit;
	};	// 0x284


	uint32_t rsvd288[30];	// 0x288 - 0x2FC


	/* PRU_INTC_ESR0 register bit field */
	union {
		volatile uint32_t ESR0;

		volatile struct {
			unsigned EN_SET_31_0 : 32;	// 31:0
		} ESR0_bit;
	};	// 0x300


	/* PRU_INTC_ESR1 register bit field */
	union {
		volatile uint32_t ESR1;

		volatile struct {
			unsigned EN_SET_63_32 : 32;	// 31:0
		} ESR1_bit;
	};	// 0x304


	uint32_t rsvd308[30];	// 0x308 - 0x37C


	/* PRU_INTC_ECR0 register bit field */
	union {
		volatile uint32_t ECR0;

		volatile struct {
			unsigned EN_CLR_31_0 : 32;	// 31:0
		} ECR0_bit;
	};	// 0x380


	/* PRU_INTC_ECR1 register bit field */
	union {
		volatile uint32_t ECR1;

		volatile struct {
			unsigned EN_CLR_63_32 : 32;	// 31:0
		} ECR1_bit;
	};	// 0x384


	uint32_t rsvd388[30];	// 0x388 - 0x3FC


	/* PRU_INTC_CMR0 register bit field */
	union {
		volatile uint32_t CMR0;

		volatile struct {
			unsigned CH_MAP_0 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_1 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_2 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_3 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR0_bit;
	};	// 0x400


	/* PRU_INTC_CMR1 register bit field */
	union {
		volatile uint32_t CMR1;

		volatile struct {
			unsigned CH_MAP_4 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_5 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_6 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_7 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR1_bit;
	};	// 0x404


	/* PRU_INTC_CMR2 register bit field */
	union {
		volatile uint32_t CMR2;

		volatile struct {
			unsigned CH_MAP_8 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_9 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_10 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_11 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR2_bit;
	};	// 0x408


	/* PRU_INTC_CMR3 register bit field */
	union {
		volatile uint32_t CMR3;

		volatile struct {
			unsigned CH_MAP_12 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_13 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_14 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_15 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR3_bit;
	};	// 0x40C


	/* PRU_INTC_CMR4 register bit field */
	union {
		volatile uint32_t CMR4;

		volatile struct {
			unsigned CH_MAP_16 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_17 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_18 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_19 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR4_bit;
	};	// 0x410


	/* PRU_INTC_CMR5 register bit field */
	union {
		volatile uint32_t CMR5;

		volatile struct {
			unsigned CH_MAP_20 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_21 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_22 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_23 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR5_bit;
	};	// 0x414


	/* PRU_INTC_CMR6 register bit field */
	union {
		volatile uint32_t CMR6;

		volatile struct {
			unsigned CH_MAP_24 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_25 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_26 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_27 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR6_bit;
	};	// 0x418


	/* PRU_INTC_CMR7 register bit field */
	union {
		volatile uint32_t CMR7;

		volatile struct {
			unsigned CH_MAP_28 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_29 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_30 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_31 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR7_bit;
	};	// 0x41C


	/* PRU_INTC_CMR8 register bit field */
	union {
		volatile uint32_t CMR8;

		volatile struct {
			unsigned CH_MAP_32 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_33 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_34 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_35 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR8_bit;
	};	// 0x420


	/* PRU_INTC_CMR9 register bit field */
	union {
		volatile uint32_t CMR9;

		volatile struct {
			unsigned CH_MAP_36 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_37 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_38 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_39 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR9_bit;
	};	// 0x424


	/* PRU_INTC_CMR10 register bit field */
	union {
		volatile uint32_t CMR10;

		volatile struct {
			unsigned CH_MAP_40 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_41 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_42 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_43 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR10_bit;
	};	// 0x428


	/* PRU_INTC_CMR11 register bit field */
	union {
		volatile uint32_t CMR11;

		volatile struct {
			unsigned CH_MAP_44 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_45 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_46 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_47 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR11_bit;
	};	// 0x42C


	/* PRU_INTC_CMR12 register bit field */
	union {
		volatile uint32_t CMR12;

		volatile struct {
			unsigned CH_MAP_48 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_49 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_50 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_51 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR12_bit;
	};	// 0x430


	/* PRU_INTC_CMR13 register bit field */
	union {
		volatile uint32_t CMR13;

		volatile struct {
			unsigned CH_MAP_52 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_53 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_54 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_55 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR13_bit;
	};	// 0x434


	/* PRU_INTC_CMR14 register bit field */
	union {
		volatile uint32_t CMR14;

		volatile struct {
			unsigned CH_MAP_56 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_57 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_58 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_59 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR14_bit;
	};	// 0x438


	/* PRU_INTC_CMR15 register bit field */
	union {
		volatile uint32_t CMR15;

		volatile struct {
			unsigned CH_MAP_60 : 4;		// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned CH_MAP_61 : 4;		// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned CH_MAP_62 : 4;		// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned CH_MAP_63 : 4;		// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} CMR15_bit;
	};	// 0x43C


	uint32_t rsvd440[240];	// 0x440 - 0x7FC


	/* PRU_INTC_HMR0 register bit field */
	union {
		volatile uint32_t HMR0;

		volatile struct {
			unsigned HINT_MAP_0 : 4;	// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned HINT_MAP_1 : 4;	// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned HINT_MAP_2 : 4;	// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned HINT_MAP_3 : 4;	// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} HMR0_bit;
	};	// 0x800


	/* PRU_INTC_HMR1 register bit field */
	union {
		volatile uint32_t HMR1;

		volatile struct {
			unsigned HINT_MAP_4 : 4;	// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned HINT_MAP_5 : 4;	// 11:8
			unsigned rsvd12 : 4;		// 15:12
			unsigned HINT_MAP_6 : 4;	// 19:16
			unsigned rsvd20 : 4;		// 23:20
			unsigned HINT_MAP_7 : 4;	// 27:24
			unsigned rsvd28 : 4;		// 31:28
		} HMR1_bit;
	};	// 0x804


	/* PRU_INTC_HMR2 register bit field */
	union {
		volatile uint32_t HMR2;

		volatile struct {
			unsigned HINT_MAP_8 : 4;	// 3:0
			unsigned rsvd4 : 4;		// 7:4
			unsigned HINT_MAP_9 : 4;	// 11:8
			unsigned rsvd12 : 20;		// 31:12
		} HMR2_bit;
	};	// 0x808


	uint32_t rsvd80C[61];	// 0x80C - 0x8FC


	/* PRU_INTC_HIPIR0 register bit field */
	union {
		volatile uint32_t HIPIR0;

		volatile struct {
			unsigned PRI_HINT_0 : 10;	// 9:0
			unsigned rsvd10 : 21;		// 30:10
			unsigned NONE_HINT_0 : 1;	// 31
		} HIPIR0_bit;
	};	// 0x900


	/* PRU_INTC_HIPIR1 register bit field */
	union {
		volatile uint32_t HIPIR1;

		volatile struct {
			unsigned PRI_HINT_1 : 10;	// 9:0
			unsigned rsvd10 : 21;		// 30:10
			unsigned NONE_HINT_1 : 1;	// 31
		} HIPIR1_bit;
	};	// 0x904


	/* PRU_INTC_HIPIR2 register bit field */
	union {
		volatile uint32_t HIPIR2;

		volatile struct {
			unsigned PRI_HINT_2 : 10;	// 9:0
			unsigned rsvd10 : 21;		// 30:10
			unsigned NONE_HINT_2 : 1;	// 31
		} HIPIR2_bit;
	};	// 0x908


	/* PRU_INTC_HIPIR3 register bit field */
	union {
		volatile uint32_t HIPIR3;

		volatile struct {
			unsigned PRI_HINT_3 : 10;	// 9:0
			unsigned rsvd10 : 21;		// 30:10
			unsigned NONE_HINT_3 : 1;	// 31
		} HIPIR3_bit;
	};	// 0x90C


	/* PRU_INTC_HIPIR4 register bit field */
	union {
		volatile uint32_t HIPIR4;

		volatile struct {
			unsigned PRI_HINT_4 : 10;	// 9:0
			unsigned rsvd10 : 21;		// 30:10
			unsigned NONE_HINT_4 : 1;	// 31
		} HIPIR4_bit;
	};	// 0x910


	/* PRU_INTC_HIPIR5 register bit field */
	union {
		volatile uint32_t HIPIR5;

		volatile struct {
			unsigned PRI_HINT_5 : 10;	// 9:0
			unsigned rsvd10 : 21;		// 30:10
			unsigned NONE_HINT_5 : 1;	// 31
		} HIPIR5_bit;
	};	// 0x914


	/* PRU_INTC_HIPIR6 register bit field */
	union {
		volatile uint32_t HIPIR6;

		volatile struct {
			unsigned PRI_HINT_6 : 10;	// 9:0
			unsigned rsvd10 : 21;		// 30:10
			unsigned NONE_HINT_6 : 1;	// 31
		} HIPIR6_bit;
	};	// 0x918


	/* PRU_INTC_HIPIR7 register bit field */
	union {
		volatile uint32_t HIPIR7;

		volatile struct {
			unsigned PRI_HINT_7 : 10;	// 9:0
			unsigned rsvd10 : 21;		// 30:10
			unsigned NONE_HINT_7 : 1;	// 31
		} HIPIR7_bit;
	};	// 0x91C


	/* PRU_INTC_HIPIR8 register bit field */
	union {
		volatile uint32_t HIPIR8;

		volatile struct {
			unsigned PRI_HINT_8 : 10;	// 9:0
			unsigned rsvd10 : 21;		// 30:10
			unsigned NONE_HINT_8 : 1;	// 31
		} HIPIR8_bit;
	};	// 0x920


	/* PRU_INTC_HIPIR9 register bit field */
	union {
		volatile uint32_t HIPIR9;

		volatile struct {
			unsigned PRI_HINT_9 : 10;	// 9:0
			unsigned rsvd10 : 21;		// 30:10
			unsigned NONE_HINT_9 : 1;	// 31
		} HIPIR9_bit;
	};	// 0x924


	uint32_t rsvd928[246];	// 0x928 - 0xCFC


	/* PRU_INTC_SIPR0 register bit field */
	union {
		volatile uint32_t SIPR0;

		volatile struct {
			unsigned POLARITY_31_0 : 32;	// 31:0
		} SIPR0_bit;
	};	// 0xD00


	/* PRU_INTC_SIPR1 register bit field */
	union {
		volatile uint32_t SIPR1;

		volatile struct {
			unsigned POLARITY_63_32 : 32;	// 31:0
		} SIPR1_bit;
	};	// 0xD04


	uint32_t rsvdD08[30];	// 0xD08 - 0xD7C


	/* PRU_INTC_SITR0 register bit field */
	union {
		volatile uint32_t SITR0;

		volatile struct {
			unsigned TYPE_31_0 : 32;	// 31:0
		} SITR0_bit;
	};	// 0xD80


	/* PRU_INTC_SITR1 register bit field */
	union {
		volatile uint32_t SITR1;

		volatile struct {
			unsigned TYPE_63_32 : 32;	// 31:0
		} SITR1_bit;
	};	// 0xD84


	uint32_t rsvdD84[222];	// 0xD88 - 0x10FC


	/* PRU_INTC_HINLR0 register bit field */
	union {
		volatile uint32_t HINLR0;

		volatile struct {
			unsigned NEST_HINT_0 : 9;	// 8:0
			unsigned rsvd9 : 22;		// 30:9
			unsigned AUTO_OVERRIDE : 1;	// 31
		} HINLR0_bit;
	};	// 0x1100


	/* PRU_INTC_HINLR1 register bit field */
	union {
		volatile uint32_t HINLR1;

		volatile struct {
			unsigned NEST_HINT_1 : 9;	// 8:0
			unsigned rsvd9 : 22;		// 30:9
			unsigned AUTO_OVERRIDE : 1;	// 31
		} HINLR1_bit;
	};	// 0x1104


	/* PRU_INTC_HINLR2 register bit field */
	union {
		volatile uint32_t HINLR2;

		volatile struct {
			unsigned NEST_HINT_2 : 9;	// 8:0
			unsigned rsvd9 : 22;		// 30:9
			unsigned AUTO_OVERRIDE : 1;	// 31
		} HINLR2_bit;
	};	// 0x1108


	/* PRU_INTC_HINLR3 register bit field */
	union {
		volatile uint32_t HINLR3;

		volatile struct {
			unsigned NEST_HINT_3 : 9;	// 8:0
			unsigned rsvd9 : 22;		// 30:9
			unsigned AUTO_OVERRIDE : 1;	// 31
		} HINLR3_bit;
	};	// 0x110C


	/* PRU_INTC_HINLR4 register bit field */
	union {
		volatile uint32_t HINLR4;

		volatile struct {
			unsigned NEST_HINT_4 : 9;	// 8:0
			unsigned rsvd9 : 22;		// 30:9
			unsigned AUTO_OVERRIDE : 1;	// 31
		} HINLR4_bit;
	};	// 0x1110


	/* PRU_INTC_HINLR5 register bit field */
	union {
		volatile uint32_t HINLR5;

		volatile struct {
			unsigned NEST_HINT_5 : 9;	// 8:0
			unsigned rsvd9 : 22;		// 30:9
			unsigned AUTO_OVERRIDE : 1;	// 31
		} HINLR5_bit;
	};	// 0x1114


	/* PRU_INTC_HINLR6 register bit field */
	union {
		volatile uint32_t HINLR6;

		volatile struct {
			unsigned NEST_HINT_6 : 9;	// 8:0
			unsigned rsvd9 : 22;		// 30:9
			unsigned AUTO_OVERRIDE : 1;	// 31
		} HINLR6_bit;
	};	// 0x1118


	/* PRU_INTC_HINLR7 register bit field */
	union {
		volatile uint32_t HINLR7;

		volatile struct {
			unsigned NEST_HINT_7 : 9;	// 8:0
			unsigned rsvd9 : 22;		// 30:9
			unsigned AUTO_OVERRIDE : 1;	// 31
		} HINLR7_bit;
	};	// 0x111C


	/* PRU_INTC_HINLR8 register bit field */
	union {
		volatile uint32_t HINLR8;

		volatile struct {
			unsigned NEST_HINT_8 : 9;	// 8:0
			unsigned rsvd9 : 22;		// 30:9
			unsigned AUTO_OVERRIDE : 1;	// 31
		} HINLR8_bit;
	};	// 0x1120


	/* PRU_INTC_HINLR9 register bit field */
	union {
		volatile uint32_t HINLR9;

		volatile struct {
			unsigned NEST_HINT_9 : 9;	// 8:0
			unsigned rsvd9 : 22;		// 30:9
			unsigned AUTO_OVERRIDE : 1;	// 31
		} HINLR9_bit;
	};	// 0x1124


	uint32_t rsvd1128[246];	// 0x1128 - 0x14FC


	/* PRU_INTC_HIER register bit field */
	union {
		volatile uint32_t HIER;

		volatile struct {
			unsigned EN_HINT : 10;	// 9:0
			unsigned rsvd9 : 22;		// 31:10
		} HIER_bit;
	};	// 0x1500

} pruIntc;

#ifdef __GNUC__
static volatile pruIntc *__CT_INTC = (void *)0x00020000;
#define CT_INTC	(*__CT_INTC)
#else
volatile __far pruIntc CT_INTC __attribute__((cregister("PRU_INTC", far), peripheral));
#endif

#endif /* _PRU_INTC_H_ */
