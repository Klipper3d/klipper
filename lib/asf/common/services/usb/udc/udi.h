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

#ifndef _UDI_H_
#define _UDI_H_

#include "conf_usb.h"
#include "usb_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup usb_device_group
 * \defgroup udi_group USB Device Interface (UDI)
 * The UDI provides a common API for all classes,
 * and this is used by UDC for the main control of USB Device interface.
 * @{
 */

/**
 * \brief UDI API.
 *
 * The callbacks within this structure are called only by
 * USB Device Controller (UDC)
 *
 * The udc_get_interface_desc() can be use by UDI to know the interface descriptor
 * selected by UDC.
 */
typedef struct {
	/**
	 * \brief Enable the interface.
	 *
	 * This function is called when the host selects a configuration
	 * to which this interface belongs through a Set Configuration
	 * request, and when the host selects an alternate setting of
	 * this interface through a Set Interface request.
	 *
	 * \return \c 1 if function was successfully done, otherwise \c 0.
	 */
	bool(*enable) (void);

	/**
	 * \brief Disable the interface.
	 *
	 * This function is called when this interface is currently
	 * active, and
	 * - the host selects any configuration through a Set
	 *   Configuration request, or
	 * - the host issues a USB reset, or
	 * - the device is detached from the host (i.e. Vbus is no
	 *   longer present)
	 */
	void (*disable) (void);

	/**
	 * \brief Handle a control request directed at an interface.
	 *
	 * This function is called when this interface is currently
	 * active and the host sends a SETUP request
	 * with this interface as the recipient.
	 *
	 * Use udd_g_ctrlreq to decode and response to SETUP request.
	 *
	 * \return \c 1 if this interface supports the SETUP request, otherwise \c 0.
	 */
	bool(*setup) (void);

	/**
	 * \brief Returns the current setting of the selected interface.
	 *
	 * This function is called when UDC when know alternate setting of selected interface.
	 *
	 * \return alternate setting of selected interface
	 */
	uint8_t(*getsetting) (void);

	/**
	 * \brief To signal that a SOF is occurred
	 */
	void(*sof_notify) (void);
} udi_api_t;

//@}

#ifdef __cplusplus
}
#endif
#endif // _UDI_H_
