/**
 * \file
 *
 * \brief Default CDC configuration for a USB Device with a single interface
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

#ifndef _UDI_CDC_CONF_H_
#define _UDI_CDC_CONF_H_

#include "usb_protocol_cdc.h"
#include "conf_usb.h"

#ifndef  UDI_CDC_PORT_NB
# define  UDI_CDC_PORT_NB 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup udi_cdc_group_single_desc
 * @{
 */

//! Control endpoint size (Endpoint 0)
#define  USB_DEVICE_EP_CTRL_SIZE       64

#if XMEGA
/**
 * \name Endpoint configuration on XMEGA
 * The XMEGA supports a IN and OUT endpoint with the same number endpoint,
 * thus XMEGA can support up to 7 CDC interfaces.
 */
//@{
#define  UDI_CDC_DATA_EP_IN_0          ( 1 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT_0         ( 2 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP_0             ( 2 | USB_EP_DIR_IN)  // Notify endpoint
#define  UDI_CDC_DATA_EP_IN_1          ( 3 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT_1         ( 4 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP_1             ( 4 | USB_EP_DIR_IN)  // Notify endpoint
#define  UDI_CDC_DATA_EP_IN_2          ( 5 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT_2         ( 6 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP_2             ( 6 | USB_EP_DIR_IN)  // Notify endpoint
#define  UDI_CDC_DATA_EP_IN_3          ( 7 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT_3         ( 8 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP_3             ( 8 | USB_EP_DIR_IN)  // Notify endpoint
#define  UDI_CDC_DATA_EP_IN_4          ( 9 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT_4         (10 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP_4             (10 | USB_EP_DIR_IN)  // Notify endpoint
#define  UDI_CDC_DATA_EP_IN_5          (11 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT_5         (12 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP_5             (12 | USB_EP_DIR_IN)  // Notify endpoint
#define  UDI_CDC_DATA_EP_IN_6          (13 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT_6         (14 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP_6             (14 | USB_EP_DIR_IN)  // Notify endpoint
//! 2 endpoints numbers used per CDC interface
#define  USB_DEVICE_MAX_EP             (2*UDI_CDC_PORT_NB)
//@}

#else

/**
 * \name Default endpoint configuration
 * The USBB, UDP, UDPHS and UOTGHS interfaces can support up to 2 CDC interfaces.
 */
//@{
#  if UDI_CDC_PORT_NB > 2
#    error USBB, UDP, UDPHS and UOTGHS interfaces have not enought endpoints.
#  endif
#define  UDI_CDC_DATA_EP_IN_0          (1 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT_0         (2 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP_0             (3 | USB_EP_DIR_IN)  // Notify endpoint
#  if SAM3U
     /* For 3U max endpoint size of 4 is 64, use 5 and 6 as bulk tx and rx */
#    define  UDI_CDC_DATA_EP_IN_1          (6 | USB_EP_DIR_IN)  // TX
#    define  UDI_CDC_DATA_EP_OUT_1         (5 | USB_EP_DIR_OUT) // RX
#    define  UDI_CDC_COMM_EP_1             (4 | USB_EP_DIR_IN)  // Notify
#  else
#    define  UDI_CDC_DATA_EP_IN_1          (4 | USB_EP_DIR_IN)  // TX
#    define  UDI_CDC_DATA_EP_OUT_1         (5 | USB_EP_DIR_OUT) // RX
#    define  UDI_CDC_COMM_EP_1             (6 | USB_EP_DIR_IN)  // Notify
#  endif
//! 3 endpoints used per CDC interface
#undef USB_DEVICE_MAX_EP   // undefine this definition in header file
#define  USB_DEVICE_MAX_EP             (3*UDI_CDC_PORT_NB)
//@}

#endif

/**
 * \name Default Interface numbers
 */
//@{
#define  UDI_CDC_COMM_IFACE_NUMBER_0   0
#define  UDI_CDC_DATA_IFACE_NUMBER_0   1
#define  UDI_CDC_COMM_IFACE_NUMBER_1   2
#define  UDI_CDC_DATA_IFACE_NUMBER_1   3
#define  UDI_CDC_COMM_IFACE_NUMBER_2   4
#define  UDI_CDC_DATA_IFACE_NUMBER_2   5
#define  UDI_CDC_COMM_IFACE_NUMBER_3   6
#define  UDI_CDC_DATA_IFACE_NUMBER_3   7
#define  UDI_CDC_COMM_IFACE_NUMBER_4   8
#define  UDI_CDC_DATA_IFACE_NUMBER_4   9
#define  UDI_CDC_COMM_IFACE_NUMBER_5   10
#define  UDI_CDC_DATA_IFACE_NUMBER_5   11
#define  UDI_CDC_COMM_IFACE_NUMBER_6   12
#define  UDI_CDC_DATA_IFACE_NUMBER_6   13
//@}

//@}

#ifdef __cplusplus
}
#endif
#endif // _UDI_CDC_CONF_H_
