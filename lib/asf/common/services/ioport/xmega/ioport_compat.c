/**
 * \file
 *
 * \brief XMEGA legacy IOPORT software compatibility driver interface.
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
#include "ioport_compat.h"

#if defined(IOPORT_XMEGA_COMPAT)
void ioport_configure_port_pin(void *port, pin_mask_t pin_mask,
		port_pin_flags_t flags)
{
	uint8_t pin;

	for (pin = 0; pin < 8; pin++) {
		if (pin_mask & (1 << pin)) {
			*((uint8_t *)port + PORT_PIN0CTRL + pin) = flags >> 8;
		}
	}
	/* Select direction and initial pin state */
	if (flags & IOPORT_DIR_OUTPUT) {
		if (flags & IOPORT_INIT_HIGH) {
			*((uint8_t *)port + PORT_OUTSET) = pin_mask;
		} else {
			*((uint8_t *)port + PORT_OUTCLR) = pin_mask;
		}

		*((uint8_t *)port + PORT_DIRSET) = pin_mask;
	} else {
		*((uint8_t *)port + PORT_DIRCLR) = pin_mask;
	}
}

#endif
