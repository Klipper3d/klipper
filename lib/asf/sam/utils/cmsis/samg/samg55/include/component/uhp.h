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

#ifndef _SAMG55_UHP_COMPONENT_
#define _SAMG55_UHP_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR USB Host Port */
/* ============================================================================= */
/** \addtogroup SAMG55_UHP USB Host Port */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Uhp hardware registers */
typedef struct {
  __I  uint32_t UHP_HCREVISION;         /**< \brief (Uhp Offset: 0x00) OHCI Revision Number Register */
  __IO uint32_t UHP_HCCONTROL;          /**< \brief (Uhp Offset: 0x04) HC Operating Mode Register */
  __IO uint32_t UHP_HCCOMMANDSTATUS;    /**< \brief (Uhp Offset: 0x08) HC Command and Status Register */
  __IO uint32_t UHP_HCINTERRUPTSTATUS;  /**< \brief (Uhp Offset: 0x0C) HC Interrupt and Status Register */
  __IO uint32_t UHP_HCINTERRUPTENABLE;  /**< \brief (Uhp Offset: 0x10) HC Interrupt Enable Register */
  __IO uint32_t UHP_HCINTERRUPTDISABLE; /**< \brief (Uhp Offset: 0x14) HC Interrupt Disable Register */
  __IO uint32_t UHP_HCHCCA;             /**< \brief (Uhp Offset: 0x18) HC HCCA Address Register */
  __I  uint32_t UHP_HCPERIODCURRENTED;  /**< \brief (Uhp Offset: 0x1C) HC Current Periodic Register */
  __IO uint32_t UHP_HCCONTROLHEADED;    /**< \brief (Uhp Offset: 0x20) HC Head Control Register */
  __IO uint32_t UHP_HCCONTROLCURRENTED; /**< \brief (Uhp Offset: 0x24) HC Current Control Register */
  __IO uint32_t UHP_HCBULKHEADED;       /**< \brief (Uhp Offset: 0x28) HC Head Bulk Register */
  __IO uint32_t UHP_HCBULKCURRENTED;    /**< \brief (Uhp Offset: 0x2C) HC Current Bulk Register */
  __I  uint32_t UHP_HCDONEHEAD;         /**< \brief (Uhp Offset: 0x30) HC Head Done Register */
  __IO uint32_t UHP_HCFMINTERVAL;       /**< \brief (Uhp Offset: 0x34) HC Frame Interval Register */
  __I  uint32_t UHP_HCFMREMAINING;      /**< \brief (Uhp Offset: 0x38) HC Frame Remaining Register */
  __I  uint32_t UHP_HCFMNUMBER;         /**< \brief (Uhp Offset: 0x3C) HC Frame Number Register */
  __IO uint32_t UHP_HCPERIODICSTART;    /**< \brief (Uhp Offset: 0x40) HC Periodic Start Register */
  __IO uint32_t UHP_HCLSTHRESHOLD;      /**< \brief (Uhp Offset: 0x44) HC Low-Speed Threshold Register */
  __IO uint32_t UHP_HCRHDESCRIPTORA;    /**< \brief (Uhp Offset: 0x48) HC Root Hub A Register */
  __IO uint32_t UHP_HCRHDESCRIPTORB;    /**< \brief (Uhp Offset: 0x4C) HC Root Hub B Register */
  __IO uint32_t UHP_HCRHSTATUS;         /**< \brief (Uhp Offset: 0x50) HC Root Hub Status Register */
  __IO uint32_t UHP_HCRHPORTSTATUS[2];  /**< \brief (Uhp Offset: 0x54) HC Port 1 Status and Control Register */
} Uhp;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- UHP_HCREVISION : (UHP Offset: 0x00) OHCI Revision Number Register -------- */
#define UHP_HCREVISION_REV_Pos 0
#define UHP_HCREVISION_REV_Msk (0xffu << UHP_HCREVISION_REV_Pos) /**< \brief (UHP_HCREVISION) OHCI revision number */
/* -------- UHP_HCCONTROL : (UHP Offset: 0x04) HC Operating Mode Register -------- */
#define UHP_HCCONTROL_CBSR_Pos 0
#define UHP_HCCONTROL_CBSR_Msk (0x3u << UHP_HCCONTROL_CBSR_Pos) /**< \brief (UHP_HCCONTROL) Control/bulk service ratio */
#define UHP_HCCONTROL_CBSR(value) ((UHP_HCCONTROL_CBSR_Msk & ((value) << UHP_HCCONTROL_CBSR_Pos)))
#define UHP_HCCONTROL_PLE (0x1u << 2) /**< \brief (UHP_HCCONTROL) Periodic list enable */
#define UHP_HCCONTROL_IE (0x1u << 3) /**< \brief (UHP_HCCONTROL) Isochronous enable */
#define UHP_HCCONTROL_CLE (0x1u << 4) /**< \brief (UHP_HCCONTROL) Control list enable */
#define UHP_HCCONTROL_BLE (0x1u << 5) /**< \brief (UHP_HCCONTROL) Bulk list enable */
#define UHP_HCCONTROL_HCFS_Pos 6
#define UHP_HCCONTROL_HCFS_Msk (0x3u << UHP_HCCONTROL_HCFS_Pos) /**< \brief (UHP_HCCONTROL) Host controller functional state */
#define UHP_HCCONTROL_HCFS(value) ((UHP_HCCONTROL_HCFS_Msk & ((value) << UHP_HCCONTROL_HCFS_Pos)))
#define UHP_HCCONTROL_IR (0x1u << 8) /**< \brief (UHP_HCCONTROL) Interrupt routing */
#define UHP_HCCONTROL_RWC (0x1u << 9) /**< \brief (UHP_HCCONTROL) Remote wake-up connected */
#define UHP_HCCONTROL_RWE (0x1u << 10) /**< \brief (UHP_HCCONTROL) Remote wake-up enable */
/* -------- UHP_HCCOMMANDSTATUS : (UHP Offset: 0x08) HC Command and Status Register -------- */
#define UHP_HCCOMMANDSTATUS_HCR (0x1u << 0) /**< \brief (UHP_HCCOMMANDSTATUS) Host controller reset (read/write) */
#define UHP_HCCOMMANDSTATUS_CLF (0x1u << 1) /**< \brief (UHP_HCCOMMANDSTATUS) Control list filled (read/write) */
#define UHP_HCCOMMANDSTATUS_BLF (0x1u << 2) /**< \brief (UHP_HCCOMMANDSTATUS) Bulk list filled (read/write) */
#define UHP_HCCOMMANDSTATUS_OCR (0x1u << 3) /**< \brief (UHP_HCCOMMANDSTATUS) Ownership change request (read/write) */
#define UHP_HCCOMMANDSTATUS_SOC_Pos 16
#define UHP_HCCOMMANDSTATUS_SOC_Msk (0x3u << UHP_HCCOMMANDSTATUS_SOC_Pos) /**< \brief (UHP_HCCOMMANDSTATUS) Scheduling overrun count (read-only) */
#define UHP_HCCOMMANDSTATUS_SOC(value) ((UHP_HCCOMMANDSTATUS_SOC_Msk & ((value) << UHP_HCCOMMANDSTATUS_SOC_Pos)))
/* -------- UHP_HCINTERRUPTSTATUS : (UHP Offset: 0x0C) HC Interrupt and Status Register -------- */
#define UHP_HCINTERRUPTSTATUS_SO (0x1u << 0) /**< \brief (UHP_HCINTERRUPTSTATUS) Scheduling overrun (read/write, write '1' to clear) */
#define UHP_HCINTERRUPTSTATUS_WDH (0x1u << 1) /**< \brief (UHP_HCINTERRUPTSTATUS) Write done head (read/write, write '1' to clear) */
#define UHP_HCINTERRUPTSTATUS_SF (0x1u << 2) /**< \brief (UHP_HCINTERRUPTSTATUS) Start of frame (read/write, write '1' to clear) */
#define UHP_HCINTERRUPTSTATUS_RD (0x1u << 3) /**< \brief (UHP_HCINTERRUPTSTATUS) Resume detected (read/write, write '1' to clear) */
#define UHP_HCINTERRUPTSTATUS_UE (0x1u << 4) /**< \brief (UHP_HCINTERRUPTSTATUS) Unrecoverable error (read/write, write '1' to clear) */
#define UHP_HCINTERRUPTSTATUS_FNO (0x1u << 5) /**< \brief (UHP_HCINTERRUPTSTATUS) Frame number overflow (read/write, write '1' to clear) */
#define UHP_HCINTERRUPTSTATUS_RHSC (0x1u << 6) /**< \brief (UHP_HCINTERRUPTSTATUS) Root hub status change (read/write, write '1' to clear) */
#define UHP_HCINTERRUPTSTATUS_OC (0x1u << 30) /**< \brief (UHP_HCINTERRUPTSTATUS) Ownership change (read-only) */
/* -------- UHP_HCINTERRUPTENABLE : (UHP Offset: 0x10) HC Interrupt Enable Register -------- */
#define UHP_HCINTERRUPTENABLE_SO (0x1u << 0) /**< \brief (UHP_HCINTERRUPTENABLE) Scheduling overrun (read/write, write '1' to set) */
#define UHP_HCINTERRUPTENABLE_WDH (0x1u << 1) /**< \brief (UHP_HCINTERRUPTENABLE) Write done head (read/write, write '1' to set) */
#define UHP_HCINTERRUPTENABLE_SF (0x1u << 2) /**< \brief (UHP_HCINTERRUPTENABLE) Start of frame (read/write, write '1' to set) */
#define UHP_HCINTERRUPTENABLE_RD (0x1u << 3) /**< \brief (UHP_HCINTERRUPTENABLE) Resume detected (read/write, write '1' to set) */
#define UHP_HCINTERRUPTENABLE_UE (0x1u << 4) /**< \brief (UHP_HCINTERRUPTENABLE) Unrecoverable error (read/write, write '1' to set) */
#define UHP_HCINTERRUPTENABLE_FNO (0x1u << 5) /**< \brief (UHP_HCINTERRUPTENABLE) Frame number overflow (read/write, write '1' to set) */
#define UHP_HCINTERRUPTENABLE_RHSC (0x1u << 6) /**< \brief (UHP_HCINTERRUPTENABLE) Root hub status change (read/write, write '1' to set) */
#define UHP_HCINTERRUPTENABLE_OC (0x1u << 30) /**< \brief (UHP_HCINTERRUPTENABLE) Ownership change (read-only) */
#define UHP_HCINTERRUPTENABLE_MIE (0x1u << 31) /**< \brief (UHP_HCINTERRUPTENABLE) Master interrupt enable (read/write, write '1' to set) */
/* -------- UHP_HCINTERRUPTDISABLE : (UHP Offset: 0x14) HC Interrupt Disable Register -------- */
#define UHP_HCINTERRUPTDISABLE_SO (0x1u << 0) /**< \brief (UHP_HCINTERRUPTDISABLE) Scheduling overrun (read/write) */
#define UHP_HCINTERRUPTDISABLE_WDH (0x1u << 1) /**< \brief (UHP_HCINTERRUPTDISABLE) Write done head (read/write) */
#define UHP_HCINTERRUPTDISABLE_SF (0x1u << 2) /**< \brief (UHP_HCINTERRUPTDISABLE) Start of frame (read/write) */
#define UHP_HCINTERRUPTDISABLE_RD (0x1u << 3) /**< \brief (UHP_HCINTERRUPTDISABLE) Resume detected (read/write) */
#define UHP_HCINTERRUPTDISABLE_UE (0x1u << 4) /**< \brief (UHP_HCINTERRUPTDISABLE) Unrecoverable error (read/write) */
#define UHP_HCINTERRUPTDISABLE_FNO (0x1u << 5) /**< \brief (UHP_HCINTERRUPTDISABLE) Frame number overflow (read/write) */
#define UHP_HCINTERRUPTDISABLE_RHSC (0x1u << 6) /**< \brief (UHP_HCINTERRUPTDISABLE) Root hub status change (read/write) */
#define UHP_HCINTERRUPTDISABLE_OC (0x1u << 30) /**< \brief (UHP_HCINTERRUPTDISABLE) Ownership change (read-only) */
#define UHP_HCINTERRUPTDISABLE_MIE (0x1u << 31) /**< \brief (UHP_HCINTERRUPTDISABLE) Master interrupt enable (read/write) */
/* -------- UHP_HCHCCA : (UHP Offset: 0x18) HC HCCA Address Register -------- */
#define UHP_HCHCCA_HCCA_Pos 8
#define UHP_HCHCCA_HCCA_Msk (0xffffffu << UHP_HCHCCA_HCCA_Pos) /**< \brief (UHP_HCHCCA) Physical address of the beginning of the HCCA */
#define UHP_HCHCCA_HCCA(value) ((UHP_HCHCCA_HCCA_Msk & ((value) << UHP_HCHCCA_HCCA_Pos)))
/* -------- UHP_HCPERIODCURRENTED : (UHP Offset: 0x1C) HC Current Periodic Register -------- */
#define UHP_HCPERIODCURRENTED_PCED_Pos 4
#define UHP_HCPERIODCURRENTED_PCED_Msk (0xfffffffu << UHP_HCPERIODCURRENTED_PCED_Pos) /**< \brief (UHP_HCPERIODCURRENTED) Physical address of the current ED on the periodic ED list */
/* -------- UHP_HCCONTROLHEADED : (UHP Offset: 0x20) HC Head Control Register -------- */
#define UHP_HCCONTROLHEADED_CHED_Pos 4
#define UHP_HCCONTROLHEADED_CHED_Msk (0xfffffffu << UHP_HCCONTROLHEADED_CHED_Pos) /**< \brief (UHP_HCCONTROLHEADED) Physical address of the head ED on the control ED list */
#define UHP_HCCONTROLHEADED_CHED(value) ((UHP_HCCONTROLHEADED_CHED_Msk & ((value) << UHP_HCCONTROLHEADED_CHED_Pos)))
/* -------- UHP_HCCONTROLCURRENTED : (UHP Offset: 0x24) HC Current Control Register -------- */
#define UHP_HCCONTROLCURRENTED_CCED_Pos 4
#define UHP_HCCONTROLCURRENTED_CCED_Msk (0xfffffffu << UHP_HCCONTROLCURRENTED_CCED_Pos) /**< \brief (UHP_HCCONTROLCURRENTED) Physical address of the current ED on the control ED list */
#define UHP_HCCONTROLCURRENTED_CCED(value) ((UHP_HCCONTROLCURRENTED_CCED_Msk & ((value) << UHP_HCCONTROLCURRENTED_CCED_Pos)))
/* -------- UHP_HCBULKHEADED : (UHP Offset: 0x28) HC Head Bulk Register -------- */
#define UHP_HCBULKHEADED_BHED_Pos 4
#define UHP_HCBULKHEADED_BHED_Msk (0xfffffffu << UHP_HCBULKHEADED_BHED_Pos) /**< \brief (UHP_HCBULKHEADED) Physical address of the head ED on the bulk ED list */
#define UHP_HCBULKHEADED_BHED(value) ((UHP_HCBULKHEADED_BHED_Msk & ((value) << UHP_HCBULKHEADED_BHED_Pos)))
/* -------- UHP_HCBULKCURRENTED : (UHP Offset: 0x2C) HC Current Bulk Register -------- */
#define UHP_HCBULKCURRENTED_BCED_Pos 4
#define UHP_HCBULKCURRENTED_BCED_Msk (0xfffffffu << UHP_HCBULKCURRENTED_BCED_Pos) /**< \brief (UHP_HCBULKCURRENTED) Physical address of the current ED on the bulk ED list */
#define UHP_HCBULKCURRENTED_BCED(value) ((UHP_HCBULKCURRENTED_BCED_Msk & ((value) << UHP_HCBULKCURRENTED_BCED_Pos)))
/* -------- UHP_HCDONEHEAD : (UHP Offset: 0x30) HC Head Done Register -------- */
#define UHP_HCDONEHEAD_DH_Pos 4
#define UHP_HCDONEHEAD_DH_Msk (0xfffffffu << UHP_HCDONEHEAD_DH_Pos) /**< \brief (UHP_HCDONEHEAD) Physical address of the last TD that has added to the done queue */
/* -------- UHP_HCFMINTERVAL : (UHP Offset: 0x34) HC Frame Interval Register -------- */
#define UHP_HCFMINTERVAL_FRAMEINTERVAL_Pos 0
#define UHP_HCFMINTERVAL_FRAMEINTERVAL_Msk (0x3fffu << UHP_HCFMINTERVAL_FRAMEINTERVAL_Pos) /**< \brief (UHP_HCFMINTERVAL) Frame interval */
#define UHP_HCFMINTERVAL_FRAMEINTERVAL(value) ((UHP_HCFMINTERVAL_FRAMEINTERVAL_Msk & ((value) << UHP_HCFMINTERVAL_FRAMEINTERVAL_Pos)))
#define UHP_HCFMINTERVAL_FSMPS_Pos 16
#define UHP_HCFMINTERVAL_FSMPS_Msk (0x7fffu << UHP_HCFMINTERVAL_FSMPS_Pos) /**< \brief (UHP_HCFMINTERVAL) Largest data packet */
#define UHP_HCFMINTERVAL_FSMPS(value) ((UHP_HCFMINTERVAL_FSMPS_Msk & ((value) << UHP_HCFMINTERVAL_FSMPS_Pos)))
#define UHP_HCFMINTERVAL_FIT (0x1u << 31) /**< \brief (UHP_HCFMINTERVAL) Frame interval toggle */
/* -------- UHP_HCFMREMAINING : (UHP Offset: 0x38) HC Frame Remaining Register -------- */
#define UHP_HCFMREMAINING_FR_Pos 0
#define UHP_HCFMREMAINING_FR_Msk (0x3fffu << UHP_HCFMREMAINING_FR_Pos) /**< \brief (UHP_HCFMREMAINING) Frame remaining */
#define UHP_HCFMREMAINING_FRT (0x1u << 31) /**< \brief (UHP_HCFMREMAINING) Frame remaining toggle */
/* -------- UHP_HCFMNUMBER : (UHP Offset: 0x3C) HC Frame Number Register -------- */
#define UHP_HCFMNUMBER_FN_Pos 0
#define UHP_HCFMNUMBER_FN_Msk (0xffffu << UHP_HCFMNUMBER_FN_Pos) /**< \brief (UHP_HCFMNUMBER) Frame number */
/* -------- UHP_HCPERIODICSTART : (UHP Offset: 0x40) HC Periodic Start Register -------- */
#define UHP_HCPERIODICSTART_PS_Pos 0
#define UHP_HCPERIODICSTART_PS_Msk (0x3fffu << UHP_HCPERIODICSTART_PS_Pos) /**< \brief (UHP_HCPERIODICSTART) Periodic start */
#define UHP_HCPERIODICSTART_PS(value) ((UHP_HCPERIODICSTART_PS_Msk & ((value) << UHP_HCPERIODICSTART_PS_Pos)))
/* -------- UHP_HCLSTHRESHOLD : (UHP Offset: 0x44) HC Low-Speed Threshold Register -------- */
#define UHP_HCLSTHRESHOLD_LST_Pos 0
#define UHP_HCLSTHRESHOLD_LST_Msk (0x3fffu << UHP_HCLSTHRESHOLD_LST_Pos) /**< \brief (UHP_HCLSTHRESHOLD) Low-speed threshold */
#define UHP_HCLSTHRESHOLD_LST(value) ((UHP_HCLSTHRESHOLD_LST_Msk & ((value) << UHP_HCLSTHRESHOLD_LST_Pos)))
/* -------- UHP_HCRHDESCRIPTORA : (UHP Offset: 0x48) HC Root Hub A Register -------- */
#define UHP_HCRHDESCRIPTORA_NDP_Pos 0
#define UHP_HCRHDESCRIPTORA_NDP_Msk (0xffu << UHP_HCRHDESCRIPTORA_NDP_Pos) /**< \brief (UHP_HCRHDESCRIPTORA) Number of downstream ports (read-only) */
#define UHP_HCRHDESCRIPTORA_NDP(value) ((UHP_HCRHDESCRIPTORA_NDP_Msk & ((value) << UHP_HCRHDESCRIPTORA_NDP_Pos)))
#define UHP_HCRHDESCRIPTORA_PSM (0x1u << 8) /**< \brief (UHP_HCRHDESCRIPTORA) Power switching mode (read/write) */
#define UHP_HCRHDESCRIPTORA_NPS (0x1u << 9) /**< \brief (UHP_HCRHDESCRIPTORA) No power switching (read/write) */
#define UHP_HCRHDESCRIPTORA_DT (0x1u << 10) /**< \brief (UHP_HCRHDESCRIPTORA) Device type (read-only) */
#define UHP_HCRHDESCRIPTORA_OCPM (0x1u << 11) /**< \brief (UHP_HCRHDESCRIPTORA) Overcurrent protection mode (read/write) */
#define UHP_HCRHDESCRIPTORA_NOCP (0x1u << 12) /**< \brief (UHP_HCRHDESCRIPTORA) No overcurrent protection (read/write) */
#define UHP_HCRHDESCRIPTORA_POTPG_Pos 24
#define UHP_HCRHDESCRIPTORA_POTPG_Msk (0xffu << UHP_HCRHDESCRIPTORA_POTPG_Pos) /**< \brief (UHP_HCRHDESCRIPTORA) Power-on to power-good time (read/write) */
#define UHP_HCRHDESCRIPTORA_POTPG(value) ((UHP_HCRHDESCRIPTORA_POTPG_Msk & ((value) << UHP_HCRHDESCRIPTORA_POTPG_Pos)))
/* -------- UHP_HCRHDESCRIPTORB : (UHP Offset: 0x4C) HC Root Hub B Register -------- */
#define UHP_HCRHDESCRIPTORB_DR0 (0x1u << 0) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR1 (0x1u << 1) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR2 (0x1u << 2) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR3 (0x1u << 3) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR4 (0x1u << 4) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR5 (0x1u << 5) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR6 (0x1u << 6) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR7 (0x1u << 7) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR8 (0x1u << 8) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR9 (0x1u << 9) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR10 (0x1u << 10) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR11 (0x1u << 11) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR12 (0x1u << 12) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR13 (0x1u << 13) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR14 (0x1u << 14) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_DR15 (0x1u << 15) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM0 (0x1u << 16) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM1 (0x1u << 17) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM2 (0x1u << 18) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM3 (0x1u << 19) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM4 (0x1u << 20) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM5 (0x1u << 21) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM6 (0x1u << 22) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM7 (0x1u << 23) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM8 (0x1u << 24) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM9 (0x1u << 25) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM10 (0x1u << 26) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM11 (0x1u << 27) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM12 (0x1u << 28) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM13 (0x1u << 29) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM14 (0x1u << 30) /**< \brief (UHP_HCRHDESCRIPTORB)  */
#define UHP_HCRHDESCRIPTORB_PPCM15 (0x1u << 31) /**< \brief (UHP_HCRHDESCRIPTORB)  */
/* -------- UHP_HCRHSTATUS : (UHP Offset: 0x50) HC Root Hub Status Register -------- */
#define UHP_HCRHSTATUS_LPS (0x1u << 0) /**< \brief (UHP_HCRHSTATUS) Local power status (read/write) */
#define UHP_HCRHSTATUS_OCI (0x1u << 1) /**< \brief (UHP_HCRHSTATUS) Overcurrent indicator (read-only) */
#define UHP_HCRHSTATUS_DRWE (0x1u << 15) /**< \brief (UHP_HCRHSTATUS) Device remote wake-up enable (read/write) */
#define UHP_HCRHSTATUS_LPSC (0x1u << 16) /**< \brief (UHP_HCRHSTATUS) Local power status change (read/write) */
#define UHP_HCRHSTATUS_OCIC (0x1u << 17) /**< \brief (UHP_HCRHSTATUS) Overcurrent indication change (read/write) */
#define UHP_HCRHSTATUS_CRWE (0x1u << 31) /**< \brief (UHP_HCRHSTATUS) Clear remote wake-up enable (read/write) */
/* -------- UHP_HCRHPORTSTATUS[2] : (UHP Offset: 0x54) HC Port 1 Status and Control Register -------- */
#define UHP_HCRHPORTSTATUS_CCS_CPE (0x1u << 0) /**< \brief (UHP_HCRHPORTSTATUS[2])  */
#define UHP_HCRHPORTSTATUS_PES_SPE (0x1u << 1) /**< \brief (UHP_HCRHPORTSTATUS[2])  */
#define UHP_HCRHPORTSTATUS_PSS_SPS (0x1u << 2) /**< \brief (UHP_HCRHPORTSTATUS[2])  */
#define UHP_HCRHPORTSTATUS_POCI_CSS (0x1u << 3) /**< \brief (UHP_HCRHPORTSTATUS[2])  */
#define UHP_HCRHPORTSTATUS_PRS_SPR (0x1u << 4) /**< \brief (UHP_HCRHPORTSTATUS[2])  */
#define UHP_HCRHPORTSTATUS_PPS_SPP (0x1u << 8) /**< \brief (UHP_HCRHPORTSTATUS[2])  */
#define UHP_HCRHPORTSTATUS_LSDA_CPP (0x1u << 9) /**< \brief (UHP_HCRHPORTSTATUS[2])  */
#define UHP_HCRHPORTSTATUS_CSC (0x1u << 16) /**< \brief (UHP_HCRHPORTSTATUS[2]) Port 1 connect status change (read/write, write '1' to clear) */
#define UHP_HCRHPORTSTATUS_PESC (0x1u << 17) /**< \brief (UHP_HCRHPORTSTATUS[2]) Port 1 enable status change (read/write, write '1' to clear) */
#define UHP_HCRHPORTSTATUS_PSSC (0x1u << 18) /**< \brief (UHP_HCRHPORTSTATUS[2]) Port 1 suspend status change (read/write, write '1' to clear) */
#define UHP_HCRHPORTSTATUS_OCIC (0x1u << 19) /**< \brief (UHP_HCRHPORTSTATUS[2]) Port 1 overcurrent indicator change (read/write) */
#define UHP_HCRHPORTSTATUS_PRSC (0x1u << 20) /**< \brief (UHP_HCRHPORTSTATUS[2]) Port 1 reset status change (read/write, write '1' to clear) */

/*@}*/


#endif /* _SAMG55_UHP_COMPONENT_ */
