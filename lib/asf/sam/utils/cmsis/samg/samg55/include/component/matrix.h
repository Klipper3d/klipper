/**
 * \file
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef _SAMG55_MATRIX_COMPONENT_
#define _SAMG55_MATRIX_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR AHB Bus Matrix */
/* ============================================================================= */
/** \addtogroup SAMG55_MATRIX AHB Bus Matrix */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Matrix hardware registers */
typedef struct {
  __IO uint32_t MATRIX_MCFG[3]; /**< \brief (Matrix Offset: 0x0000) Master Configuration Register */
  __I  uint32_t Reserved1[13];
  __IO uint32_t MATRIX_SCFG[4]; /**< \brief (Matrix Offset: 0x0040) Slave Configuration Register */
  __I  uint32_t Reserved2[12];
  __IO uint32_t MATRIX_PRAS0;   /**< \brief (Matrix Offset: 0x0080) Priority Register A for Slave 0 */
  __I  uint32_t Reserved3[1];
  __IO uint32_t MATRIX_PRAS1;   /**< \brief (Matrix Offset: 0x0088) Priority Register A for Slave 1 */
  __I  uint32_t Reserved4[1];
  __IO uint32_t MATRIX_PRAS2;   /**< \brief (Matrix Offset: 0x0090) Priority Register A for Slave 2 */
  __I  uint32_t Reserved5[1];
  __IO uint32_t MATRIX_PRAS3;   /**< \brief (Matrix Offset: 0x0098) Priority Register A for Slave 3 */
  __I  uint32_t Reserved6[1];
  __I  uint32_t Reserved7[28];
  __IO uint32_t CCFG_RDMMEM;    /**< \brief (Matrix Offset: 0x0110) Embedded Memories Read Margin Values Register */
  __IO uint32_t CCFG_SYSIO;     /**< \brief (Matrix Offset: 0x0114) System I/O Configuration Register */
  __IO uint32_t CCFG_DYNCKG;    /**< \brief (Matrix Offset: 0x0118) Dynamic Clock Gating Register */
  __IO uint32_t CCFG_I2SCLKSEL; /**< \brief (Matrix Offset: 0x011C) I2S Clock Source Selection Register */
  __IO uint32_t CCFG_USBMR;     /**< \brief (Matrix Offset: 0x0120) USB Management Register */
  __I  uint32_t Reserved8[48];
  __IO uint32_t MATRIX_WPMR;    /**< \brief (Matrix Offset: 0x1E4) Write Protection Mode Register */
  __I  uint32_t MATRIX_WPSR;    /**< \brief (Matrix Offset: 0x1E8) Write Protection Status Register */
} Matrix;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- MATRIX_MCFG[3] : (MATRIX Offset: 0x0000) Master Configuration Register -------- */
#define MATRIX_MCFG_ULBT_Pos 0
#define MATRIX_MCFG_ULBT_Msk (0x7u << MATRIX_MCFG_ULBT_Pos) /**< \brief (MATRIX_MCFG[3]) Undefined Length Burst Type */
#define MATRIX_MCFG_ULBT(value) ((MATRIX_MCFG_ULBT_Msk & ((value) << MATRIX_MCFG_ULBT_Pos)))
#define   MATRIX_MCFG_ULBT_INFINITE (0x0u << 0) /**< \brief (MATRIX_MCFG[3]) No predicted end of burst is generated and therefore INCR bursts coming from this master cannot be broken. */
#define   MATRIX_MCFG_ULBT_SINGLE (0x1u << 0) /**< \brief (MATRIX_MCFG[3]) The undefined length burst is treated as a succession of single access allowing rearbitration at each beat of the INCR burst. */
#define   MATRIX_MCFG_ULBT_FOUR_BEAT (0x2u << 0) /**< \brief (MATRIX_MCFG[3]) The undefined length burst is split into a 4-beat bursts allowing rearbitration at each 4-beat burst end. */
#define   MATRIX_MCFG_ULBT_EIGHT_BEAT (0x3u << 0) /**< \brief (MATRIX_MCFG[3]) The undefined length burst is split into 8-beat bursts allowing rearbitration at each 8-beat burst end. */
#define   MATRIX_MCFG_ULBT_SIXTEEN_BEAT (0x4u << 0) /**< \brief (MATRIX_MCFG[3]) The undefined length burst is split into 16-beat bursts allowing rearbitration at each 16-beat burst end. */
/* -------- MATRIX_SCFG[4] : (MATRIX Offset: 0x0040) Slave Configuration Register -------- */
#define MATRIX_SCFG_SLOT_CYCLE_Pos 0
#define MATRIX_SCFG_SLOT_CYCLE_Msk (0xffu << MATRIX_SCFG_SLOT_CYCLE_Pos) /**< \brief (MATRIX_SCFG[4]) Maximum Number of Allowed Cycles for a Burst */
#define MATRIX_SCFG_SLOT_CYCLE(value) ((MATRIX_SCFG_SLOT_CYCLE_Msk & ((value) << MATRIX_SCFG_SLOT_CYCLE_Pos)))
#define MATRIX_SCFG_DEFMSTR_TYPE_Pos 16
#define MATRIX_SCFG_DEFMSTR_TYPE_Msk (0x3u << MATRIX_SCFG_DEFMSTR_TYPE_Pos) /**< \brief (MATRIX_SCFG[4]) Default Master Type */
#define MATRIX_SCFG_DEFMSTR_TYPE(value) ((MATRIX_SCFG_DEFMSTR_TYPE_Msk & ((value) << MATRIX_SCFG_DEFMSTR_TYPE_Pos)))
#define   MATRIX_SCFG_DEFMSTR_TYPE_NO_DEFAULT (0x0u << 16) /**< \brief (MATRIX_SCFG[4]) At the end of current slave access, if no other master request is pending, the slave is disconnected from all masters.This results in having a one cycle latency for the first access of a burst transfer or for a single access. */
#define   MATRIX_SCFG_DEFMSTR_TYPE_LAST (0x1u << 16) /**< \brief (MATRIX_SCFG[4]) At the end of current slave access, if no other master request is pending, the slave stays connected to the last master having accessed it.This results in not having the one cycle latency when the last master tries to access the slave again. */
#define   MATRIX_SCFG_DEFMSTR_TYPE_FIXED (0x2u << 16) /**< \brief (MATRIX_SCFG[4]) At the end of the current slave access, if no other master request is pending, the slave connects to the fixed master the number that has been written in the FIXED_DEFMSTR field.This results in not having the one cycle latency when the fixed master tries to access the slave again. */
#define MATRIX_SCFG_FIXED_DEFMSTR_Pos 18
#define MATRIX_SCFG_FIXED_DEFMSTR_Msk (0x7u << MATRIX_SCFG_FIXED_DEFMSTR_Pos) /**< \brief (MATRIX_SCFG[4]) Fixed Default Master */
#define MATRIX_SCFG_FIXED_DEFMSTR(value) ((MATRIX_SCFG_FIXED_DEFMSTR_Msk & ((value) << MATRIX_SCFG_FIXED_DEFMSTR_Pos)))
#define MATRIX_SCFG_ARBT_Pos 24
#define MATRIX_SCFG_ARBT_Msk (0x3u << MATRIX_SCFG_ARBT_Pos) /**< \brief (MATRIX_SCFG[4]) Arbitration Type */
#define MATRIX_SCFG_ARBT(value) ((MATRIX_SCFG_ARBT_Msk & ((value) << MATRIX_SCFG_ARBT_Pos)))
#define   MATRIX_SCFG_ARBT_ROUND_ROBIN (0x0u << 24) /**< \brief (MATRIX_SCFG[4]) Round-robin arbitration */
#define   MATRIX_SCFG_ARBT_FIXED_PRIORITY (0x1u << 24) /**< \brief (MATRIX_SCFG[4]) Fixed priority arbitration */
/* -------- MATRIX_PRAS0 : (MATRIX Offset: 0x0080) Priority Register A for Slave 0 -------- */
#define MATRIX_PRAS0_M0PR_Pos 0
#define MATRIX_PRAS0_M0PR_Msk (0x3u << MATRIX_PRAS0_M0PR_Pos) /**< \brief (MATRIX_PRAS0) Master 0 Priority */
#define MATRIX_PRAS0_M0PR(value) ((MATRIX_PRAS0_M0PR_Msk & ((value) << MATRIX_PRAS0_M0PR_Pos)))
#define MATRIX_PRAS0_M1PR_Pos 4
#define MATRIX_PRAS0_M1PR_Msk (0x3u << MATRIX_PRAS0_M1PR_Pos) /**< \brief (MATRIX_PRAS0) Master 1 Priority */
#define MATRIX_PRAS0_M1PR(value) ((MATRIX_PRAS0_M1PR_Msk & ((value) << MATRIX_PRAS0_M1PR_Pos)))
#define MATRIX_PRAS0_M2PR_Pos 8
#define MATRIX_PRAS0_M2PR_Msk (0x3u << MATRIX_PRAS0_M2PR_Pos) /**< \brief (MATRIX_PRAS0) Master 2 Priority */
#define MATRIX_PRAS0_M2PR(value) ((MATRIX_PRAS0_M2PR_Msk & ((value) << MATRIX_PRAS0_M2PR_Pos)))
#define MATRIX_PRAS0_M3PR_Pos 12
#define MATRIX_PRAS0_M3PR_Msk (0x3u << MATRIX_PRAS0_M3PR_Pos) /**< \brief (MATRIX_PRAS0) Master 3 Priority */
#define MATRIX_PRAS0_M3PR(value) ((MATRIX_PRAS0_M3PR_Msk & ((value) << MATRIX_PRAS0_M3PR_Pos)))
/* -------- MATRIX_PRAS1 : (MATRIX Offset: 0x0088) Priority Register A for Slave 1 -------- */
#define MATRIX_PRAS1_M0PR_Pos 0
#define MATRIX_PRAS1_M0PR_Msk (0x3u << MATRIX_PRAS1_M0PR_Pos) /**< \brief (MATRIX_PRAS1) Master 0 Priority */
#define MATRIX_PRAS1_M0PR(value) ((MATRIX_PRAS1_M0PR_Msk & ((value) << MATRIX_PRAS1_M0PR_Pos)))
#define MATRIX_PRAS1_M1PR_Pos 4
#define MATRIX_PRAS1_M1PR_Msk (0x3u << MATRIX_PRAS1_M1PR_Pos) /**< \brief (MATRIX_PRAS1) Master 1 Priority */
#define MATRIX_PRAS1_M1PR(value) ((MATRIX_PRAS1_M1PR_Msk & ((value) << MATRIX_PRAS1_M1PR_Pos)))
#define MATRIX_PRAS1_M2PR_Pos 8
#define MATRIX_PRAS1_M2PR_Msk (0x3u << MATRIX_PRAS1_M2PR_Pos) /**< \brief (MATRIX_PRAS1) Master 2 Priority */
#define MATRIX_PRAS1_M2PR(value) ((MATRIX_PRAS1_M2PR_Msk & ((value) << MATRIX_PRAS1_M2PR_Pos)))
#define MATRIX_PRAS1_M3PR_Pos 12
#define MATRIX_PRAS1_M3PR_Msk (0x3u << MATRIX_PRAS1_M3PR_Pos) /**< \brief (MATRIX_PRAS1) Master 3 Priority */
#define MATRIX_PRAS1_M3PR(value) ((MATRIX_PRAS1_M3PR_Msk & ((value) << MATRIX_PRAS1_M3PR_Pos)))
/* -------- MATRIX_PRAS2 : (MATRIX Offset: 0x0090) Priority Register A for Slave 2 -------- */
#define MATRIX_PRAS2_M0PR_Pos 0
#define MATRIX_PRAS2_M0PR_Msk (0x3u << MATRIX_PRAS2_M0PR_Pos) /**< \brief (MATRIX_PRAS2) Master 0 Priority */
#define MATRIX_PRAS2_M0PR(value) ((MATRIX_PRAS2_M0PR_Msk & ((value) << MATRIX_PRAS2_M0PR_Pos)))
#define MATRIX_PRAS2_M1PR_Pos 4
#define MATRIX_PRAS2_M1PR_Msk (0x3u << MATRIX_PRAS2_M1PR_Pos) /**< \brief (MATRIX_PRAS2) Master 1 Priority */
#define MATRIX_PRAS2_M1PR(value) ((MATRIX_PRAS2_M1PR_Msk & ((value) << MATRIX_PRAS2_M1PR_Pos)))
#define MATRIX_PRAS2_M2PR_Pos 8
#define MATRIX_PRAS2_M2PR_Msk (0x3u << MATRIX_PRAS2_M2PR_Pos) /**< \brief (MATRIX_PRAS2) Master 2 Priority */
#define MATRIX_PRAS2_M2PR(value) ((MATRIX_PRAS2_M2PR_Msk & ((value) << MATRIX_PRAS2_M2PR_Pos)))
#define MATRIX_PRAS2_M3PR_Pos 12
#define MATRIX_PRAS2_M3PR_Msk (0x3u << MATRIX_PRAS2_M3PR_Pos) /**< \brief (MATRIX_PRAS2) Master 3 Priority */
#define MATRIX_PRAS2_M3PR(value) ((MATRIX_PRAS2_M3PR_Msk & ((value) << MATRIX_PRAS2_M3PR_Pos)))
/* -------- MATRIX_PRAS3 : (MATRIX Offset: 0x0098) Priority Register A for Slave 3 -------- */
#define MATRIX_PRAS3_M0PR_Pos 0
#define MATRIX_PRAS3_M0PR_Msk (0x3u << MATRIX_PRAS3_M0PR_Pos) /**< \brief (MATRIX_PRAS3) Master 0 Priority */
#define MATRIX_PRAS3_M0PR(value) ((MATRIX_PRAS3_M0PR_Msk & ((value) << MATRIX_PRAS3_M0PR_Pos)))
#define MATRIX_PRAS3_M1PR_Pos 4
#define MATRIX_PRAS3_M1PR_Msk (0x3u << MATRIX_PRAS3_M1PR_Pos) /**< \brief (MATRIX_PRAS3) Master 1 Priority */
#define MATRIX_PRAS3_M1PR(value) ((MATRIX_PRAS3_M1PR_Msk & ((value) << MATRIX_PRAS3_M1PR_Pos)))
#define MATRIX_PRAS3_M2PR_Pos 8
#define MATRIX_PRAS3_M2PR_Msk (0x3u << MATRIX_PRAS3_M2PR_Pos) /**< \brief (MATRIX_PRAS3) Master 2 Priority */
#define MATRIX_PRAS3_M2PR(value) ((MATRIX_PRAS3_M2PR_Msk & ((value) << MATRIX_PRAS3_M2PR_Pos)))
#define MATRIX_PRAS3_M3PR_Pos 12
#define MATRIX_PRAS3_M3PR_Msk (0x3u << MATRIX_PRAS3_M3PR_Pos) /**< \brief (MATRIX_PRAS3) Master 3 Priority */
#define MATRIX_PRAS3_M3PR(value) ((MATRIX_PRAS3_M3PR_Msk & ((value) << MATRIX_PRAS3_M3PR_Pos)))
/* -------- CCFG_RDMMEM : (MATRIX Offset: 0x0110) Embedded Memories Read Margin Values Register -------- */
#define CCFG_RDMMEM_READ_MARGIN_ROM_Pos 0
#define CCFG_RDMMEM_READ_MARGIN_ROM_Msk (0xfu << CCFG_RDMMEM_READ_MARGIN_ROM_Pos) /**< \brief (CCFG_RDMMEM) ROM Read Margin Value Selection */
#define CCFG_RDMMEM_READ_MARGIN_ROM(value) ((CCFG_RDMMEM_READ_MARGIN_ROM_Msk & ((value) << CCFG_RDMMEM_READ_MARGIN_ROM_Pos)))
#define CCFG_RDMMEM_READ_MARGIN_DPRAM_Pos 4
#define CCFG_RDMMEM_READ_MARGIN_DPRAM_Msk (0xfu << CCFG_RDMMEM_READ_MARGIN_DPRAM_Pos) /**< \brief (CCFG_RDMMEM) DPRAM Read Margin Value Selection */
#define CCFG_RDMMEM_READ_MARGIN_DPRAM(value) ((CCFG_RDMMEM_READ_MARGIN_DPRAM_Msk & ((value) << CCFG_RDMMEM_READ_MARGIN_DPRAM_Pos)))
#define CCFG_RDMMEM_READ_MARGIN_RAM32k_Pos 8
#define CCFG_RDMMEM_READ_MARGIN_RAM32k_Msk (0xfu << CCFG_RDMMEM_READ_MARGIN_RAM32k_Pos) /**< \brief (CCFG_RDMMEM) RAM32k Read Margin Value Selection */
#define CCFG_RDMMEM_READ_MARGIN_RAM32k(value) ((CCFG_RDMMEM_READ_MARGIN_RAM32k_Msk & ((value) << CCFG_RDMMEM_READ_MARGIN_RAM32k_Pos)))
#define CCFG_RDMMEM_READ_MARGIN_RAM16k_Pos 12
#define CCFG_RDMMEM_READ_MARGIN_RAM16k_Msk (0xfu << CCFG_RDMMEM_READ_MARGIN_RAM16k_Pos) /**< \brief (CCFG_RDMMEM) RAM16k Read Margin Value Selection */
#define CCFG_RDMMEM_READ_MARGIN_RAM16k(value) ((CCFG_RDMMEM_READ_MARGIN_RAM16k_Msk & ((value) << CCFG_RDMMEM_READ_MARGIN_RAM16k_Pos)))
#define CCFG_RDMMEM_READ_MARGIN_RAM8k_Pos 16
#define CCFG_RDMMEM_READ_MARGIN_RAM8k_Msk (0xfu << CCFG_RDMMEM_READ_MARGIN_RAM8k_Pos) /**< \brief (CCFG_RDMMEM) RAM8k Read Margin Value Selection */
#define CCFG_RDMMEM_READ_MARGIN_RAM8k(value) ((CCFG_RDMMEM_READ_MARGIN_RAM8k_Msk & ((value) << CCFG_RDMMEM_READ_MARGIN_RAM8k_Pos)))
#define CCFG_RDMMEM_READ_MARGIN_RAM4k_Pos 20
#define CCFG_RDMMEM_READ_MARGIN_RAM4k_Msk (0xfu << CCFG_RDMMEM_READ_MARGIN_RAM4k_Pos) /**< \brief (CCFG_RDMMEM) RAM4k Read Margin Value Selection */
#define CCFG_RDMMEM_READ_MARGIN_RAM4k(value) ((CCFG_RDMMEM_READ_MARGIN_RAM4k_Msk & ((value) << CCFG_RDMMEM_READ_MARGIN_RAM4k_Pos)))
#define CCFG_RDMMEM_READ_MARGIN_REGFILE0_Pos 24
#define CCFG_RDMMEM_READ_MARGIN_REGFILE0_Msk (0xfu << CCFG_RDMMEM_READ_MARGIN_REGFILE0_Pos) /**< \brief (CCFG_RDMMEM) REGFILE0 Read Margin Value Selection */
#define CCFG_RDMMEM_READ_MARGIN_REGFILE0(value) ((CCFG_RDMMEM_READ_MARGIN_REGFILE0_Msk & ((value) << CCFG_RDMMEM_READ_MARGIN_REGFILE0_Pos)))
#define CCFG_RDMMEM_READ_MARGIN_REGFILE1_Pos 28
#define CCFG_RDMMEM_READ_MARGIN_REGFILE1_Msk (0xfu << CCFG_RDMMEM_READ_MARGIN_REGFILE1_Pos) /**< \brief (CCFG_RDMMEM) REGFILE1 Read Margin Value Selection */
#define CCFG_RDMMEM_READ_MARGIN_REGFILE1(value) ((CCFG_RDMMEM_READ_MARGIN_REGFILE1_Msk & ((value) << CCFG_RDMMEM_READ_MARGIN_REGFILE1_Pos)))
/* -------- CCFG_SYSIO : (MATRIX Offset: 0x0114) System I/O Configuration Register -------- */
#define CCFG_SYSIO_SYSIO4 (0x1u << 4) /**< \brief (CCFG_SYSIO) PB4 or TDI Assignment */
#define CCFG_SYSIO_SYSIO5 (0x1u << 5) /**< \brief (CCFG_SYSIO) PB5 or TDO/TRACESWO Assignment */
#define CCFG_SYSIO_SYSIO6 (0x1u << 6) /**< \brief (CCFG_SYSIO) PB6 or TMS/SWDIO Assignment */
#define CCFG_SYSIO_SYSIO7 (0x1u << 7) /**< \brief (CCFG_SYSIO) PB7 or TCK/SWCLK Assignment */
#define CCFG_SYSIO_SYSIO10 (0x1u << 10) /**< \brief (CCFG_SYSIO) PA21 or DM Assignment */
#define CCFG_SYSIO_SYSIO11 (0x1u << 11) /**< \brief (CCFG_SYSIO) PA22 or DP Assignment */
#define CCFG_SYSIO_SYSIO12 (0x1u << 12) /**< \brief (CCFG_SYSIO) PB12 or ERASE Assignment */
/* -------- CCFG_DYNCKG : (MATRIX Offset: 0x0118) Dynamic Clock Gating Register -------- */
#define CCFG_DYNCKG_MATCKG (0x1u << 0) /**< \brief (CCFG_DYNCKG) MATRIX Dynamic Clock Gating */
#define CCFG_DYNCKG_BRIDCKG (0x1u << 1) /**< \brief (CCFG_DYNCKG) Bridge Dynamic Clock Gating Enable */
#define CCFG_DYNCKG_EFCCKG (0x1u << 2) /**< \brief (CCFG_DYNCKG) EFC Dynamic Clock Gating Enable */
/* -------- CCFG_I2SCLKSEL : (MATRIX Offset: 0x011C) I2S Clock Source Selection Register -------- */
#define CCFG_I2SCLKSEL_CLKSEL0 (0x1u << 0) /**< \brief (CCFG_I2SCLKSEL) I2S0 clock source */
#define CCFG_I2SCLKSEL_CLKSEL1 (0x1u << 1) /**< \brief (CCFG_I2SCLKSEL) I2S1 clock source */
/* -------- CCFG_USBMR : (MATRIX Offset: 0x0120) USB Management Register -------- */
#define CCFG_USBMR_DEVICE (0x1u << 0) /**< \brief (CCFG_USBMR) USB device mode */
#define CCFG_USBMR_USBHTSSC (0x1u << 1) /**< \brief (CCFG_USBMR) USB Transceiver Suspend Software Control */
#define CCFG_USBMR_USBHTSC (0x1u << 2) /**< \brief (CCFG_USBMR) USB Host Transceiver Suspend Control */
/* -------- MATRIX_WPMR : (MATRIX Offset: 0x1E4) Write Protection Mode Register -------- */
#define MATRIX_WPMR_WPEN (0x1u << 0) /**< \brief (MATRIX_WPMR) Write Protection Enable */
#define MATRIX_WPMR_WPKEY_Pos 8
#define MATRIX_WPMR_WPKEY_Msk (0xffffffu << MATRIX_WPMR_WPKEY_Pos) /**< \brief (MATRIX_WPMR) Write Protection Key */
#define   MATRIX_WPMR_WPKEY_PASSWD (0x4D4154u << 8) /**< \brief (MATRIX_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit. Always reads as 0. */
/* -------- MATRIX_WPSR : (MATRIX Offset: 0x1E8) Write Protection Status Register -------- */
#define MATRIX_WPSR_WPVS (0x1u << 0) /**< \brief (MATRIX_WPSR) Write Protection Violation Status */
#define MATRIX_WPSR_WPVSRC_Pos 8
#define MATRIX_WPSR_WPVSRC_Msk (0xffffu << MATRIX_WPSR_WPVSRC_Pos) /**< \brief (MATRIX_WPSR) Write Protection Violation Source */

/*@}*/


#endif /* _SAMG55_MATRIX_COMPONENT_ */
