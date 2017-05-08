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

#ifndef _PRU_CFG_H_
#define _PRU_CFG_H_

/* PRU_CFG register set */
typedef struct {

	/* PRU_CFG_REVID register bit field */
	union {
		volatile uint32_t REVID;

		volatile struct {
			unsigned REVID : 32;
		} REVID_bit;
	};	// 0x0


	/* PRU_CFG_SYSCFG register bit field */
	union {
		volatile uint32_t SYSCFG;

		volatile struct {
			unsigned IDLE_MODE : 2;
			unsigned STANDBY_MODE : 2;
			unsigned STANDBY_INIT : 1;
			unsigned SUB_MWAIT : 1;
			unsigned rsvd6 : 26;
		} SYSCFG_bit;
	};	// 0x4


	/* PRU_CFG_GPCFG0 register bit field */
	union {
		volatile uint32_t GPCFG0;

		volatile struct {
			unsigned PRU0_GPI_MODE : 2;	// 1:0
			unsigned PRU0_GPI_CLK_MODE : 1;	// 2
			unsigned PRU0_GPI_DIV0 : 5;	// 7:3
			unsigned PRU0_GPI_DIV1 : 5;	// 12:8
			unsigned PRU0_GPI_SB : 1;	// 13
			unsigned PRU0_GPO_MODE : 1;	// 14
			unsigned PRU0_GPO_DIV0 : 5;	// 19:15
			unsigned PRU0_GPO_DIV1 : 5;	// 24:20
			unsigned PRU0_GPO_SH_SEL : 1;	// 25
			unsigned rsvd26 : 6;		// 31:26
		} GPCFG0_bit;
	};	// 0x8


	/* PRU_CFG_GPCFG1 register bit field */
	union {
		volatile uint32_t GPCFG1;

		volatile struct {
			unsigned PRU1_GPI_MODE : 2;	// 1:0
			unsigned PRU1_GPI_CLK_MODE : 1;	// 2
			unsigned PRU1_GPI_DIV0 : 5;	// 7:3
			unsigned PRU1_GPI_DIV1 : 5;	// 12:8
			unsigned PRU1_GPI_SB : 1;	// 13
			unsigned PRU1_GPO_MODE : 1;	// 14
			unsigned PRU1_GPO_DIV0 : 5;	// 19:15
			unsigned PRU1_GPO_DIV1 : 5;	// 24:20
			unsigned PRU1_GPO_SH_SEL : 1;	// 25
			unsigned rsvd26 : 6;		// 31:26
		} GPCFG1_bit;
	};	// 0xC


	/* PRU_CFG_CGR register bit field */
	union {
		volatile uint32_t CGR;

		volatile struct {
			unsigned PRU0_CLK_STOP_REQ : 1;	// 0
			unsigned PRU0_CLK_STOP_ACK : 1;	// 1
			unsigned PRU0_CLK_EN : 1;	// 2
			unsigned PRU1_CLK_STOP_REQ : 1;	// 3
			unsigned PRU1_CLK_STOP_ACK : 1;	// 4
			unsigned PRU1_CLK_EN : 1;	// 5
			unsigned INTC_CLK_STOP_REQ : 1;	// 6
			unsigned INTC_CLK_STOP_ACK : 1;	// 7
			unsigned INTC_CLK_EN : 1;	// 8
			unsigned UART_CLK_STOP_REQ : 1;	// 9
			unsigned UART_CLK_STOP_ACK : 1;	// 10
			unsigned UART_CLK_EN : 1;	// 11
			unsigned ECAP_CLK_STOP_REQ : 1;	// 12
			unsigned ECAP_CLK_STOP_ACK : 1;	// 13
			unsigned ECAP_CLK_EN : 1;	// 14
			unsigned IEP_CLK_STOP_REQ : 1;	// 15
			unsigned IEP_CLK_STOP_ACK : 1;	// 16
			unsigned IEP_CLK_EN : 1;	// 17
			unsigned rsvd18 : 14;		// 31:18
		} CGR_bit;
	};	// 0x10


	/* PRU_CFG_ISRP register bit field */
	union {
		volatile uint32_t ISRP;

		volatile  struct {
			unsigned PRU0_IMEM_PE_RAW : 4;	// 3:0
			unsigned PRU0_DMEM_PE_RAW : 4;	// 7:4
			unsigned PRU1_IMEM_PE_RAW : 4;	// 11:8
			unsigned PRU1_DMEM_PE_RAW : 4;	// 15:12
			unsigned RAM_PE_RAW : 4;	// 19:16
			unsigned rsvd20 : 12;		// 31:20
		} ISRP_bit;
	};	// 0x14


	/* PRU_CFG_ISP register bit field */
	union {
		volatile uint32_t ISP;

		volatile  struct {
			unsigned PRU0_IMEM_PE : 4;	// 3:0
			unsigned PRU0_DMEM_PE : 4;	// 7:4
			unsigned PRU1_IMEM_PE : 4;	// 11:8
			unsigned PRU1_DMEM_PE : 4;	// 15:12
			unsigned RAM_PE : 4;		// 19:16
			unsigned rsvd20 : 12;		// 31:20
		} ISP_bit;
	};	// 0x18

	/* PRU_CFG_IESP register bit field */
	union {
		volatile uint32_t IESP;

		volatile struct {
			unsigned PRU0_IMEM_PE_SET : 4;	// 3:0
			unsigned PRU0_DMEM_PE_SET : 4;	// 7:4
			unsigned PRU1_IMEM_PE_SET : 4;	// 11:8
			unsigned PRU1_DMEM_PE_SET : 4;	// 15:12
			unsigned RAM_PE_SET : 4;	// 19:16
			unsigned rsvd20 : 12;		// 31:20
		} IESP_bit;
	};	// 0x1C


	/* PRU_CFG_IECP register bit field */
	union {
		volatile uint32_t IECP;

		volatile struct {
			unsigned PRU0_IMEM_PE_CLR : 4;	// 3:0
			unsigned PRU0_DMEM_PE_CLR : 4;	// 7:4
			unsigned PRU1_IMEM_PE_CLR : 4;	// 11:8
			unsigned PRU1_DMEM_PE_CLR : 4;	// 15:12
			unsigned rsvd16 : 16;		// 31:16
		} IECP_bit;
	};	// 0x20


	uint32_t rsvd24;	// 0x24


	/* PRU_CFG_PMAO register bit field */
	union {
		volatile uint32_t PMAO;

		volatile struct {
			unsigned PMAO_PRU0 : 1;		// 0
			unsigned PMAO_PRU1 : 1;		// 1
			unsigned rsvd2 : 30;		// 31:2
		} PMAO_bit;
	};	// 0x28


	uint32_t rsvd2c[1];     // 0x2C


	/* PRU_CFG_IEPCLK register bit field */
	union {
		volatile uint32_t IEPCLK;

		volatile struct {
			unsigned OCP_EN : 1;		// 0
			unsigned rsvd1 : 31;		// 31:1
		} IEPCLK_bit;
	};	// 0x30


	/* PRU_CFG_SPP register bit field */
	union {
		volatile uint32_t SPP;

		volatile struct {
			unsigned PRU1_PAD_HP_EN : 1;	// 0
			unsigned XFR_SHIFT_EN : 1;	// 1
			unsigned rsvd2 : 30;		// 31:2
		} SPP_bit;
	};	// 0x34


	uint32_t rsvd38[2];	// 0x38 - 0x3C


	union {
		volatile uint32_t PIN_MX;

		volatile struct {
			unsigned PIN_MUX_SEL : 8;	// 7:0
			unsigned rsvd2 : 24;		// 31:8
		} PIN_MX_bit;
	};	//0x40
} pruCfg;

#ifdef __GNUC__
static volatile pruCfg *__CT_CFG = (void *)0x00026000;
#define CT_CFG	(*__CT_CFG)
#else
volatile __far pruCfg CT_CFG __attribute__((cregister("PRU_CFG", near), peripheral));
#endif

#endif /* _PRU_CFG_H_ */
