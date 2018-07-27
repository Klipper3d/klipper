/**
 * \file
 *
 * \brief Chip-specific oscillator management functions
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

#include <osc.h>

#ifdef BOARD_OSC0_HZ
void osc_priv_enable_osc0(void)
{
	irqflags_t flags;

	flags = cpu_irq_save();
	AVR32_SCIF.unlock = 0xaa000000 | AVR32_SCIF_OSCCTRL;
	AVR32_SCIF.oscctrl[0] =
			(OSC0_STARTUP_VALUE << AVR32_SCIF_OSCCTRL_STARTUP)
			| (OSC0_GAIN_VALUE << AVR32_SCIF_OSCCTRL_GAIN)
			| (OSC0_MODE_VALUE << AVR32_SCIF_OSCCTRL_MODE)
			| (1U << AVR32_SCIF_OSCCTRL_OSCEN);
	cpu_irq_restore(flags);
}

void osc_priv_disable_osc0(void)
{
	irqflags_t flags;

	flags = cpu_irq_save();
	AVR32_SCIF.unlock = 0xaa000000 | AVR32_SCIF_OSCCTRL;
	AVR32_SCIF.oscctrl[0] = 0;
	cpu_irq_restore(flags);
}
#endif /* BOARD_OSC0_HZ */

#ifdef BOARD_OSC32_HZ
void osc_priv_enable_osc32(void)
{
	irqflags_t flags;

	flags = cpu_irq_save();
	AVR32_SCIF.unlock = 0xaa000000 | AVR32_SCIF_OSCCTRL32;
	AVR32_SCIF.oscctrl32 =
			(OSC32_STARTUP_VALUE << AVR32_SCIF_OSCCTRL32_STARTUP)
			| (OSC32_MODE_VALUE << AVR32_SCIF_OSCCTRL32_MODE)
			| (1U << AVR32_SCIF_OSCCTRL32_OSC32EN);
	cpu_irq_restore(flags);
}

void osc_priv_disable_osc32(void)
{
	irqflags_t flags;

	flags = cpu_irq_save();
	AVR32_SCIF.unlock = 0xaa000000 | AVR32_SCIF_OSCCTRL32;
	AVR32_SCIF.oscctrl32 = 0;
	cpu_irq_restore(flags);
}
#endif /* BOARD_OSC32_HZ */

void osc_priv_enable_rc120m(void)
{
	irqflags_t flags;

	flags = cpu_irq_save();
	AVR32_SCIF.unlock = 0xaa000000 | AVR32_SCIF_RC120MCR;
	AVR32_SCIF.rc120mcr = 1U << AVR32_SCIF_RC120MCR_EN;
	cpu_irq_restore(flags);
}

void osc_priv_disable_rc120m(void)
{
	irqflags_t flags;

	flags = cpu_irq_save();
	AVR32_SCIF.unlock = 0xaa000000 | AVR32_SCIF_RC120MCR;
	AVR32_SCIF.rc120mcr = 0;
	cpu_irq_restore(flags);
}
