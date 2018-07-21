/**
 * \file
 *
 * \brief Common API for USB Device Interface
 *
 * Copyright (c) 2009-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _UDC_DESC_H_
#define _UDC_DESC_H_

#include "conf_usb.h"
#include "usb_protocol.h"
#include "udi.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup udc_group
 * \defgroup udc_desc_group USB Device Descriptor
 *
 * @{
 */

/**
 * \brief Defines the memory's location of USB descriptors
 *
 * By default the Descriptor is stored in RAM
 * (UDC_DESC_STORAGE is defined empty).
 *
 * If you have need to free RAM space,
 * it is possible to put descriptor in flash in following case:
 * - USB driver authorize flash transfer (USBB on UC3 and USB on Mega)
 * - USB Device is not high speed (UDC no need to change USB descriptors)
 *
 * For UC3 application used "const".
 *
 * For Mega application used "code".
 */
#define  UDC_DESC_STORAGE
	// Descriptor storage in internal RAM
#if (defined UDC_DATA_USE_HRAM_SUPPORT)
#	if defined(__GNUC__)
#		define UDC_DATA(x)              COMPILER_WORD_ALIGNED __attribute__((__section__(".data_hram0")))
#		define UDC_BSS(x)               COMPILER_ALIGNED(x)   __attribute__((__section__(".bss_hram0")))
#	elif defined(__ICCAVR32__)
#		define UDC_DATA(x)              COMPILER_ALIGNED(x)   __data32
#		define UDC_BSS(x)               COMPILER_ALIGNED(x)   __data32
#	endif
#else
#	define UDC_DATA(x)              COMPILER_ALIGNED(x)
#	define UDC_BSS(x)               COMPILER_ALIGNED(x)
#endif



/**
 * \brief Configuration descriptor and UDI link for one USB speed
 */
typedef struct {
	//! USB configuration descriptor
	usb_conf_desc_t UDC_DESC_STORAGE *desc;
	//! Array of UDI API pointer
	udi_api_t UDC_DESC_STORAGE *UDC_DESC_STORAGE * udi_apis;
} udc_config_speed_t;


/**
 * \brief All information about the USB Device
 */
typedef struct {
	//! USB device descriptor for low or full speed
	usb_dev_desc_t UDC_DESC_STORAGE *confdev_lsfs;
	//! USB configuration descriptor and UDI API pointers for low or full speed
	udc_config_speed_t UDC_DESC_STORAGE *conf_lsfs;
#ifdef USB_DEVICE_HS_SUPPORT
	//! USB device descriptor for high speed
	usb_dev_desc_t UDC_DESC_STORAGE *confdev_hs;
	//! USB device qualifier, only use in high speed mode
	usb_dev_qual_desc_t UDC_DESC_STORAGE *qualifier;
	//! USB configuration descriptor and UDI API pointers for high speed
	udc_config_speed_t UDC_DESC_STORAGE *conf_hs;
#endif
	usb_dev_bos_desc_t UDC_DESC_STORAGE *conf_bos;
} udc_config_t;

//! Global variables of USB Device Descriptor and UDI links
extern UDC_DESC_STORAGE udc_config_t udc_config;

//@}

#ifdef __cplusplus
}
#endif
#endif // _UDC_DESC_H_
