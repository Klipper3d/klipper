/**
 * \file
 *
 * \brief Chip-specific system clock management functions
 *
 * Copyright (c) 2010-2018 Microchip Technology Inc. and its subsidiaries.
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

#include <compiler.h>

#include <sysclk.h>
#include <osc.h>
#include <pll.h>

#if XMEGA_AU || XMEGA_B || XMEGA_C
#  include <nvm.h>
#endif


void sysclk_init(void)
{
	uint8_t *reg = (uint8_t *)&PR.PRGEN;
	uint8_t i;
#ifdef CONFIG_OSC_RC32_CAL
	uint16_t cal;
	/* avoid Cppcheck Warning */
	UNUSED(cal);
#endif
	bool need_rc2mhz = false;

	/* Turn off all peripheral clocks that can be turned off. */
	for (i = 0; i <= SYSCLK_PORT_F; i++) {
		*(reg++) = 0xff;
	}

	/* Set up system clock prescalers if different from defaults */
	if ((CONFIG_SYSCLK_PSADIV != SYSCLK_PSADIV_1)
			|| (CONFIG_SYSCLK_PSBCDIV != SYSCLK_PSBCDIV_1_1)) {
		sysclk_set_prescalers(CONFIG_SYSCLK_PSADIV,
				CONFIG_SYSCLK_PSBCDIV);
	}
#if (CONFIG_OSC_RC32_CAL==48000000UL)
	MSB(cal) = nvm_read_production_signature_row(
			nvm_get_production_signature_row_offset(USBRCOSC));
	LSB(cal) = nvm_read_production_signature_row(
			nvm_get_production_signature_row_offset(USBRCOSCA));
	/*
	* If a device has an uncalibrated value in the
	* production signature row (early sample part), load a
	* sane default calibration value.
	*/
	if (cal == 0xFFFF) {
		cal = 0x2340;
	}
	osc_user_calibration(OSC_ID_RC32MHZ,cal);
#endif
	/*
	 * Switch to the selected initial system clock source, unless
	 * the default internal 2 MHz oscillator is selected.
	 */
	if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_RC2MHZ) {
		need_rc2mhz = true;
	} else {
		switch (CONFIG_SYSCLK_SOURCE) {
		case SYSCLK_SRC_RC32MHZ:
			osc_enable(OSC_ID_RC32MHZ);
			osc_wait_ready(OSC_ID_RC32MHZ);
#ifdef CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC
			if (CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC
					!= OSC_ID_USBSOF) {
				osc_enable(CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC);
				osc_wait_ready(CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC);
			}
			osc_enable_autocalibration(OSC_ID_RC32MHZ,
					CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC);
#endif
			break;

		case SYSCLK_SRC_RC32KHZ:
			osc_enable(OSC_ID_RC32KHZ);
			osc_wait_ready(OSC_ID_RC32KHZ);
			break;

		case SYSCLK_SRC_XOSC:
			osc_enable(OSC_ID_XOSC);
			osc_wait_ready(OSC_ID_XOSC);
			break;

#ifdef CONFIG_PLL0_SOURCE
		case SYSCLK_SRC_PLL:
			if (CONFIG_PLL0_SOURCE == PLL_SRC_RC2MHZ) {
				need_rc2mhz = true;
			}
			pll_enable_config_defaults(0);
			break;
#endif
#if XMEGA_E
		case SYSCLK_SRC_RC8MHZ:
			osc_enable(OSC_ID_RC8MHZ);
			osc_wait_ready(OSC_ID_RC8MHZ);
			break;
#endif
		default:
			//unhandled_case(CONFIG_SYSCLK_SOURCE);
			return;
		}

		ccp_write_io((uint8_t *)&CLK.CTRL, CONFIG_SYSCLK_SOURCE);
		Assert(CLK.CTRL == CONFIG_SYSCLK_SOURCE);
	}

	if (need_rc2mhz) {
#ifdef CONFIG_OSC_AUTOCAL_RC2MHZ_REF_OSC
		osc_enable(CONFIG_OSC_AUTOCAL_RC2MHZ_REF_OSC);
		osc_wait_ready(CONFIG_OSC_AUTOCAL_RC2MHZ_REF_OSC);
		osc_enable_autocalibration(OSC_ID_RC2MHZ,
				CONFIG_OSC_AUTOCAL_RC2MHZ_REF_OSC);
#endif
	} else {
		osc_disable(OSC_ID_RC2MHZ);
	}

#ifdef CONFIG_RTC_SOURCE
	sysclk_rtcsrc_enable(CONFIG_RTC_SOURCE);
#endif
}

void sysclk_enable_module(enum sysclk_port_id port, uint8_t id)
{
	irqflags_t flags = cpu_irq_save();

	*((uint8_t *)&PR.PRGEN + port) &= ~id;

	cpu_irq_restore(flags);
}

void sysclk_disable_module(enum sysclk_port_id port, uint8_t id)
{
	irqflags_t flags = cpu_irq_save();

	*((uint8_t *)&PR.PRGEN + port) |= id;

	cpu_irq_restore(flags);
}

#if XMEGA_AU || XMEGA_B || XMEGA_C || defined(__DOXYGEN__)

/**
 * \brief Enable clock for the USB module
 *
 * \pre CONFIG_USBCLK_SOURCE must be defined.
 *
 * \param frequency The required USB clock frequency in MHz:
 * \arg \c 6 for 6 MHz
 * \arg \c 48 for 48 MHz
 */
void sysclk_enable_usb(uint8_t frequency)
{
	uint8_t prescaler;

	Assert((frequency == 6) || (frequency == 48));

	/*
	 * Enable or disable prescaler depending on if the USB frequency is 6
	 * MHz or 48 MHz. Only 6 MHz USB frequency requires prescaling.
	 */
	if (frequency == 6) {
		prescaler = CLK_USBPSDIV_8_gc;
	}
	else {
		prescaler = 0;
	}

	/*
	 * Switch to the system clock selected by the user.
	 */
	switch (CONFIG_USBCLK_SOURCE) {
	case USBCLK_SRC_RCOSC:
		if (!osc_is_ready(OSC_ID_RC32MHZ)) {
			osc_enable(OSC_ID_RC32MHZ);
			osc_wait_ready(OSC_ID_RC32MHZ);
#ifdef CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC
			if (CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC
					!= OSC_ID_USBSOF) {
				osc_enable(CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC);
				osc_wait_ready(CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC);
			}
			osc_enable_autocalibration(OSC_ID_RC32MHZ,
					CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC);
#endif
		}
		ccp_write_io((uint8_t *)&CLK.USBCTRL, (prescaler)
				| CLK_USBSRC_RC32M_gc
				| CLK_USBSEN_bm);
		break;

#ifdef CONFIG_PLL0_SOURCE
	case USBCLK_SRC_PLL:
		pll_enable_config_defaults(0);
		ccp_write_io((uint8_t *)&CLK.USBCTRL, (prescaler)
				| CLK_USBSRC_PLL_gc
				| CLK_USBSEN_bm);
		break;
#endif

	default:
		Assert(false);
		break;
	}

	sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_USB);
}

/**
 * \brief Disable clock for the USB module
 */
void sysclk_disable_usb(void)
{
	sysclk_disable_module(SYSCLK_PORT_GEN, SYSCLK_USB);
	ccp_write_io((uint8_t *)&CLK.USBCTRL, 0);
}
#endif // XMEGA_AU || XMEGA_B || XMEGA_C
