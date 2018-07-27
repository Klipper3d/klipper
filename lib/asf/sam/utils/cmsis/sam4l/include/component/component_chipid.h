/**
 * \file
 *
 * \brief Component description for CHIPID
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

#ifndef _SAM4L_CHIPID_COMPONENT_
#define _SAM4L_CHIPID_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR CHIPID */
/* ========================================================================== */
/** \addtogroup SAM4L_CHIPID Chip ID Registers */
/*@{*/

/** \brief CHIPID hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
	RoReg8  Reserved1[0x340];
	RoReg   CHIPID_CIDR;        /**< \brief (CHIPID Offset: 0x340) Chip ID Register */
	RoReg   CHIPID_EXID;        /**< \brief (CHIPID Offset: 0x344) Chip ID Extension Register */
} Chipid;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define REV_CHIPID                  0x100

/* -------- CHIPID_CIDR : (CHIPID Offset: 0x340) (R/  32) Chip ID Register -------- */
#define CHIPID_CIDR_OFFSET          0x340        /**< \brief (CHIPID_CIDR offset) Chip ID Register */
#define CHIPID_CIDR_RESETVALUE      0xAB0A09E0   /**< \brief (CHIPID_CIDR reset_value) Chip ID Register */
#define CHIPID_CIDR_MASK            0xFFFFFFFFu  /**< \brief (CHIPID_CIDR) MASK Register */

/* -------- CHIPID_EXID : (CHIPID Offset: 0x344) (R/  32) Chip ID Extension Register -------- */
#define CHIPID_EXID_OFFSET          0x344        /**< \brief (CHIPID_EXID offset) Chip ID Extension Register */
#define CHIPID_EXID_RESETVALUE      0x0400000F   /**< \brief (CHIPID_EXID reset_value) Chip ID Extension Register */
#define CHIPID_EXID_MASK            0xFFFFFFFFu  /**< \brief (CHIPID_EXID) MASK Register */

/* -------- < \brief (CHIPID_CIDR) Version of the Device -------- */
#define CHIPID_CIDR_VERSION_Pos      0
#define CHIPID_CIDR_VERSION_Msk      (0x1Fu << CHIPID_CIDR_VERSION_Pos)
#define CHIPID_CIDR_VERSION(value)   ((CHIPID_CIDR_VERSION_Msk & ((value) << CHIPID_CIDR_VERSION_Pos)))
/* -------- < \brief (CHIPID_CIDR) Embedded Processor -------- */
#define CHIPID_CIDR_EPROC_Pos        5
#define CHIPID_CIDR_EPROC_Msk        (0x7u << CHIPID_CIDR_EPROC_Pos) /**< \brief (CHIPID_CIDR) Embedded Processor */
#define CHIPID_CIDR_EPROC(value)     ((CHIPID_CIDR_EPROC_Msk & ((value) << CHIPID_CIDR_EPROC_Pos)))
#define CHIPID_CIDR_EPROC_ARM946ES   CHIPID_CIDR_EPROC(0x1u) /**< \brief (CHIPID_CIDR) ARM946ES */
#define CHIPID_CIDR_EPROC_ARM7TDMI   CHIPID_CIDR_EPROC(0x2u) /**< \brief (CHIPID_CIDR) ARM7TDMI */
#define CHIPID_CIDR_EPROC_CM3        CHIPID_CIDR_EPROC(0x3u) /**< \brief (CHIPID_CIDR) Cortex-M3 */
#define CHIPID_CIDR_EPROC_ARM920T    CHIPID_CIDR_EPROC(0x4u) /**< \brief (CHIPID_CIDR) ARM920T */
#define CHIPID_CIDR_EPROC_ARM926EJS  CHIPID_CIDR_EPROC(0x5u) /**< \brief (CHIPID_CIDR) ARM926EJS */
#define CHIPID_CIDR_EPROC_CA5        CHIPID_CIDR_EPROC(0x6u) /**< \brief (CHIPID_CIDR) Cortex-A5 */
#define CHIPID_CIDR_EPROC_CM4        CHIPID_CIDR_EPROC(0x7u) /**< \brief (CHIPID_CIDR) Cortex-M4 */
/* -------- < \brief (CHIPID_CIDR) Nonvolatile Program Memory Size -------- */
#define CHIPID_CIDR_NVPSIZ_Pos       8
#define CHIPID_CIDR_NVPSIZ_Msk       (0xFu << CHIPID_CIDR_NVPSIZ_Pos) /**< \brief (CHIPID_CIDR) Nonvolatile Program Memory Size */
#define CHIPID_CIDR_NVPSIZ(value)    ((CHIPID_CIDR_NVPSIZ_Msk & ((value) << CHIPID_CIDR_NVPSIZ_Pos)))
#define CHIPID_CIDR_NVPSIZ_NONE      CHIPID_CIDR_NVPSIZ(0x0u) /**< \brief (CHIPID_CIDR) None */
#define CHIPID_CIDR_NVPSIZ_8K        CHIPID_CIDR_NVPSIZ(0x1u) /**< \brief (CHIPID_CIDR) 8 Kbytes */
#define CHIPID_CIDR_NVPSIZ_16K       CHIPID_CIDR_NVPSIZ(0x2u) /**< \brief (CHIPID_CIDR) 16 Kbytes */
#define CHIPID_CIDR_NVPSIZ_32K       CHIPID_CIDR_NVPSIZ(0x3u) /**< \brief (CHIPID_CIDR) 32 Kbytes */
#define CHIPID_CIDR_NVPSIZ_64K       CHIPID_CIDR_NVPSIZ(0x5u) /**< \brief (CHIPID_CIDR) 64 Kbytes */
#define CHIPID_CIDR_NVPSIZ_128K      CHIPID_CIDR_NVPSIZ(0x7u) /**< \brief (CHIPID_CIDR) 128 Kbytes */
#define CHIPID_CIDR_NVPSIZ_256K      CHIPID_CIDR_NVPSIZ(0x9u) /**< \brief (CHIPID_CIDR) 256 Kbytes */
#define CHIPID_CIDR_NVPSIZ_512K      CHIPID_CIDR_NVPSIZ(0xAu) /**< \brief (CHIPID_CIDR) 512 Kbytes */
#define CHIPID_CIDR_NVPSIZ_1024K     CHIPID_CIDR_NVPSIZ(0xCu) /**< \brief (CHIPID_CIDR) 1024 Kbytes */
#define CHIPID_CIDR_NVPSIZ_2048K     CHIPID_CIDR_NVPSIZ(0xEu) /**< \brief (CHIPID_CIDR) 2048 Kbytes */
/* -------- < \brief (CHIPID_CIDR) Second Nonvolatile Program Memory Size -------- */
#define CHIPID_CIDR_NVPSIZ2_Pos      12
#define CHIPID_CIDR_NVPSIZ2_Msk      (0xFu << CHIPID_CIDR_NVPSIZ2_Pos) /**< \brief (CHIPID_CIDR) Second Nonvolatile Program Memory Size */
#define CHIPID_CIDR_NVPSIZ2(value)   ((CHIPID_CIDR_NVPSIZ2_Msk & ((value) << CHIPID_CIDR_NVPSIZ2_Pos)))
#define CHIPID_CIDR_NVPSIZ2_NONE     CHIPID_CIDR_NVPSIZ2(0x0u) /**< \brief (CHIPID_CIDR) None */
#define CHIPID_CIDR_NVPSIZ2_8K       CHIPID_CIDR_NVPSIZ2(0x1u) /**< \brief (CHIPID_CIDR) 8 Kbytes */
#define CHIPID_CIDR_NVPSIZ2_16K      CHIPID_CIDR_NVPSIZ2(0x2u) /**< \brief (CHIPID_CIDR) 16 Kbytes */
#define CHIPID_CIDR_NVPSIZ2_32K      CHIPID_CIDR_NVPSIZ2(0x3u) /**< \brief (CHIPID_CIDR) 32 Kbytes */
#define CHIPID_CIDR_NVPSIZ2_64K      CHIPID_CIDR_NVPSIZ2(0x5u) /**< \brief (CHIPID_CIDR) 64 Kbytes */
#define CHIPID_CIDR_NVPSIZ2_128K     CHIPID_CIDR_NVPSIZ2(0x7u) /**< \brief (CHIPID_CIDR) 128 Kbytes */
#define CHIPID_CIDR_NVPSIZ2_256K     CHIPID_CIDR_NVPSIZ2(0x9u) /**< \brief (CHIPID_CIDR) 256 Kbytes */
#define CHIPID_CIDR_NVPSIZ2_512K     CHIPID_CIDR_NVPSIZ2(0xAu) /**< \brief (CHIPID_CIDR) 512 Kbytes */
#define CHIPID_CIDR_NVPSIZ2_1024K    CHIPID_CIDR_NVPSIZ2(0xCu) /**< \brief (CHIPID_CIDR) 1024 Kbytes */
#define CHIPID_CIDR_NVPSIZ2_2048K    CHIPID_CIDR_NVPSIZ2(0xEu) /**< \brief (CHIPID_CIDR) 2048 Kbytes */
/* -------- < \brief (CHIPID_CIDR) Internal SRAM Size -------- */
#define CHIPID_CIDR_SRAMSIZ_Pos      16
#define CHIPID_CIDR_SRAMSIZ_Msk      (0xFu << CHIPID_CIDR_SRAMSIZ_Pos) /**< \brief (CHIPID_CIDR) Internal SRAM Size */
#define CHIPID_CIDR_SRAMSIZ(value)   ((CHIPID_CIDR_SRAMSIZ_Msk & ((value << CHIPID_CIDR_SRAMSIZ_Pos))))
#define CHIPID_CIDR_SRAMSIZ_48K      CHIPID_CIDR_SRAMSIZ(0x0u) /**< \brief (CHIPID_CIDR) 48 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_1K       CHIPID_CIDR_SRAMSIZ(0x1u) /**< \brief (CHIPID_CIDR) 1 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_2K       CHIPID_CIDR_SRAMSIZ(0x2u) /**< \brief (CHIPID_CIDR) 2 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_6K       CHIPID_CIDR_SRAMSIZ(0x3u) /**< \brief (CHIPID_CIDR) 6 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_24K      CHIPID_CIDR_SRAMSIZ(0x4u) /**< \brief (CHIPID_CIDR) 24 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_4K       CHIPID_CIDR_SRAMSIZ(0x5u) /**< \brief (CHIPID_CIDR) 4 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_80K      CHIPID_CIDR_SRAMSIZ(0x6u) /**< \brief (CHIPID_CIDR) 80 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_160K     CHIPID_CIDR_SRAMSIZ(0x7u) /**< \brief (CHIPID_CIDR) 160 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_8K       CHIPID_CIDR_SRAMSIZ(0x8u) /**< \brief (CHIPID_CIDR) 8 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_16K      CHIPID_CIDR_SRAMSIZ(0x9u) /**< \brief (CHIPID_CIDR) 16 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_32K      CHIPID_CIDR_SRAMSIZ(0xAu) /**< \brief (CHIPID_CIDR) 32 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_64K      CHIPID_CIDR_SRAMSIZ(0xBu) /**< \brief (CHIPID_CIDR) 64 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_128K     CHIPID_CIDR_SRAMSIZ(0xCu) /**< \brief (CHIPID_CIDR) 128 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_256K     CHIPID_CIDR_SRAMSIZ(0xDu) /**< \brief (CHIPID_CIDR) 256 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_96K      CHIPID_CIDR_SRAMSIZ(0xEu) /**< \brief (CHIPID_CIDR) 96 Kbytes */
#define CHIPID_CIDR_SRAMSIZ_512K     CHIPID_CIDR_SRAMSIZ(0xFu) /**< \brief (CHIPID_CIDR) 512 Kbytes */
/* -------- < \brief (CHIPID_CIDR) Architecture Identifier -------- */
#define CHIPID_CIDR_ARCH_Pos         20
#define CHIPID_CIDR_ARCH_Msk         (0xFFu << CHIPID_CIDR_ARCH_Pos)
#define CHIPID_CIDR_ARCH(value)      ((CHIPID_CIDR_ARCH_Msk & ((value) << CHIPID_CIDR_ARCH_Pos)))
#define CHIPID_CIDR_ARCH_SAM4L       CHIPID_CIDR_ARCH(0xB0u) /**< \brief (CHIPID_CIDR) SAM4L Series */
/* -------- < \brief (CHIPID_CIDR) Nonvolatile Program Memory Type -------- */
#define CHIPID_CIDR_NVPTYP_Pos       28
#define CHIPID_CIDR_NVPTYP_Msk       (0x7u << CHIPID_CIDR_NVPTYP_Pos) /**< \brief (CHIPID_CIDR) Nonvolatile Program Memory Type */
#define CHIPID_CIDR_NVPTYP(value)    ((CHIPID_CIDR_NVPTYP_Msk & ((value) << CHIPID_CIDR_NVPTYP_Pos)))
#define CHIPID_CIDR_NVPTYP_ROM       CHIPID_CIDR_NVPTYP(0x0u) /**< \brief (CHIPID_CIDR) ROM */
#define CHIPID_CIDR_NVPTYP_ROMLESS   CHIPID_CIDR_NVPTYP(0x1u) /**< \brief (CHIPID_CIDR) ROMless or on-chip Flash */
#define CHIPID_CIDR_NVPTYP_FLASH     CHIPID_CIDR_NVPTYP(0x2u) /**< \brief (CHIPID_CIDR) Embedded Flash Memory */
#define CHIPID_CIDR_NVPTYP_ROM_FLASH CHIPID_CIDR_NVPTYP(0x3u) /**< \brief (CHIPID_CIDR) ROM and Embedded Flash Memory- NVPSIZ is ROM size- NVPSIZ2 is Flash size */
#define CHIPID_CIDR_NVPTYP_SRAM      CHIPID_CIDR_NVPTYP(0x4u) /**< \brief (CHIPID_CIDR) SRAM emulating ROM */
/* -------- < \brief (CHIPID_CIDR) Extension Flag -------- */
#define CHIPID_CIDR_EXT_Pos          31
#define CHIPID_CIDR_EXT (0x1u << CHIPID_CIDR_EXT_Pos) /**< \brief (CHIPID_CIDR) Extension Flag */
/** \brief CHIPID Extension hardware registers */
/* -------- < \brief (CHIPID_EXID) Chip ID Extension -------- */
/* -------- < \brief (CHIPID_EXID) Chip ID Extension AES Option -------- */
#define CHIPID_EXID_AES_Pos          0
#define CHIPID_EXID_AES_Msk          (0x1u << CHIPID_EXID_AES_Pos)
#define CHIPID_EXID_AES(value)       ((CHIPID_EXID_AES_Msk & ((value) << CHIPID_EXID_AES_Pos)))
#define CHIPID_EXID_AES_NIMP         CHIPID_EXID_AES(0x0u) /* -------- < \brief (CHIPID_EXID) AES is not implemented -------- */
#define CHIPID_EXID_AES_IMP          CHIPID_EXID_AES(0x1u) /* -------- < \brief (CHIPID_EXID) AES is implemented -------- */
/* -------- < \brief (CHIPID_EXID) Chip ID Extension USB Option -------- */
#define CHIPID_EXID_USB_Pos          1
#define CHIPID_EXID_USB_Msk          (0x1u << CHIPID_EXID_USB_Pos)
#define CHIPID_EXID_USB(value)       ((CHIPID_EXID_USB_Msk & ((value) << CHIPID_EXID_USB_Pos)))
#define CHIPID_EXID_USB_NIMP         CHIPID_EXID_USB(0x0u) /* -------- < \brief (CHIPID_EXID) USB is not implemented -------- */
#define CHIPID_EXID_USB_IMP          CHIPID_EXID_USB(0x1u) /* -------- < \brief (CHIPID_EXID) USB is implemented -------- */
/* -------- < \brief (CHIPID_EXID) Chip ID Extension USB Configuration -------- */
#define CHIPID_EXID_USBFULL_Pos      2
#define CHIPID_EXID_USBFULL_Msk      (0x1u << CHIPID_EXID_USBFULL_Pos)
#define CHIPID_EXID_USBFULL(value)   ((CHIPID_EXID_USBFULL_Msk & ((value) << CHIPID_EXID_USBFULL_Pos)))
#define CHIPID_EXID_USBFULL_DO       CHIPID_EXID_USBFULL(0x0u) /* -------- < \brief (CHIPID_EXID) USB is Device-only -------- */
#define CHIPID_EXID_USBFULL_DH       CHIPID_EXID_USBFULL(0x1u) /* -------- < \brief (CHIPID_EXID) Device and Host -------- */
/* -------- < \brief (CHIPID_EXID) Chip ID Extension LCD Option -------- */
#define CHIPID_EXID_LCD_Pos          3
#define CHIPID_EXID_LCD_Msk          (0x1u << CHIPID_EXID_LCD_Pos)
#define CHIPID_EXID_LCD(value)       ((CHIPID_EXID_LCD_Msk & ((value) << CHIPID_EXID_LCD_Pos)))
#define CHIPID_EXID_LCD_NIMP         CHIPID_EXID_LCD(0x0u) /* -------- < \brief (CHIPID_EXID) LCD is not implemented -------- */
#define CHIPID_EXID_LCD_IMP          CHIPID_EXID_LCD(0x1u) /* -------- < \brief (CHIPID_EXID) LCD is implemented -------- */
/* -------- < \brief (CHIPID_EXID) Chip ID Extension LCD Option -------- */
#define CHIPID_EXID_PACKAGE_Pos      24
#define CHIPID_EXID_PACKAGE_Msk      (0x7u << CHIPID_EXID_PACKAGE_Pos) /**< \brief (CHIPID_EXID) Package Type */
#define CHIPID_EXID_PACKAGE(value)   ((CHIPID_EXID_PACKAGE_Msk & ((value) << CHIPID_EXID_PACKAGE_Pos)))
#define CHIPID_EXID_PACKAGE_24       CHIPID_EXID_PACKAGE(0x0u) /* -------- < \brief (CHIPID_EXID) 24-pin package -------- */
#define CHIPID_EXID_PACKAGE_32       CHIPID_EXID_PACKAGE(0x1u) /* -------- < \brief (CHIPID_EXID) 32-pin package -------- */
#define CHIPID_EXID_PACKAGE_48       CHIPID_EXID_PACKAGE(0x2u) /* -------- < \brief (CHIPID_EXID) 48-pin package -------- */
#define CHIPID_EXID_PACKAGE_64       CHIPID_EXID_PACKAGE(0x3u) /* -------- < \brief (CHIPID_EXID) 64-pin package -------- */
#define CHIPID_EXID_PACKAGE_100      CHIPID_EXID_PACKAGE(0x4u) /* -------- < \brief (CHIPID_EXID) 100-pin package -------- */
#define CHIPID_EXID_PACKAGE_144      CHIPID_EXID_PACKAGE(0x5u) /* -------- < \brief (CHIPID_EXID) 144-pin package -------- */

/*@}*/

#endif /* _SAM4L_CHIPID_COMPONENT_ */
