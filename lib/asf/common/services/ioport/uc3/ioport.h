/**
 * \file
 *
 * \brief UC3 architecture specific IOPORT service implementation header file.
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
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
#ifndef IOPORT_UC3_H
#define IOPORT_UC3_H

#include <sysclk.h>

#define IOPORT_CREATE_PIN(port, pin) ((port) * 32 + (pin))

/**
 * \weakgroup ioport_group
 * \section ioport_modes IOPORT Modes
 *
 * For details on these please see the device datasheet.
 *
 * @{
 */

/** \name IOPORT Mode bit definitions */
/** @{ */
#define IOPORT_MODE_MUX_MASK            (7 << 0) /*!< MUX bits mask */
#define IOPORT_MODE_MUX_BIT0            (1 << 0) /*!< MUX BIT0 mask */
#define IOPORT_MODE_MUX_BIT1            (1 << 1) /*!< MUX BIT1 mask */
#define IOPORT_MODE_MUX_A               (0 << 0) /*!< MUX function A */
#define IOPORT_MODE_MUX_B               (1 << 0) /*!< MUX function B */
#define IOPORT_MODE_MUX_C               (2 << 0) /*!< MUX function C */
#define IOPORT_MODE_MUX_D               (3 << 0) /*!< MUX function D */

#if UC3C || UC3D || UC3L
#define IOPORT_MODE_MUX_BIT2            (1 << 2) /*!< MUX BIT2 mask */
#define IOPORT_MODE_MUX_E               (4 << 0) /*!< MUX function E */
#define IOPORT_MODE_MUX_F               (5 << 0) /*!< MUX function F */
#define IOPORT_MODE_MUX_G               (6 << 0) /*!< MUX function G */
#define IOPORT_MODE_MUX_H               (7 << 0) /*!< MUX function H */
#endif

#define IOPORT_MODE_PULLUP              (1 << 3) /*!< Pull-up */

#if UC3C
#define IOPORT_MODE_PULLDOWN            (1 << 4) /*!< Pull-down */
#endif

#if 0
/* \internal Not currently synthesized on any UC3 parts */
#define IOPORT_MODE_OPEN_DRAIN          (1 << 5) /*!< Open drain */
#endif

#define IOPORT_MODE_GLITCH_FILTER       (1 << 6) /*!< Glitch filter */

#if UC3C
#define IOPORT_MODE_DRIVE_STRENGTH      (1 << 7) /*!< Extra drive strength */
#endif
/** @} */

/** @} */

typedef uint32_t ioport_mode_t;
typedef uint32_t ioport_pin_t;
typedef uint32_t ioport_port_t;
typedef uint32_t ioport_port_mask_t;

__always_inline static ioport_port_t arch_ioport_pin_to_port_id(ioport_pin_t pin)
{
	return pin >> 5;
}

__always_inline static volatile avr32_gpio_port_t *arch_ioport_port_to_base(
		ioport_port_t port)
{
	return (volatile avr32_gpio_port_t *)(AVR32_GPIO_ADDRESS
	       + port * sizeof(avr32_gpio_port_t));
}

__always_inline static volatile avr32_gpio_port_t *arch_ioport_pin_to_base(
		ioport_pin_t pin)
{
	return arch_ioport_port_to_base(arch_ioport_pin_to_port_id(pin));
}

__always_inline static ioport_port_mask_t arch_ioport_pin_to_mask(
		ioport_pin_t pin)
{
	return 1U << (pin & 0x1F);
}

__always_inline static void arch_ioport_init(void)
{
	sysclk_enable_pba_module(SYSCLK_GPIO);
}

__always_inline static void arch_ioport_enable_port(ioport_port_t port,
		ioport_port_mask_t mask)
{
	arch_ioport_port_to_base(port)->gpers = mask;
}

__always_inline static void arch_ioport_disable_port(ioport_port_t port,
		ioport_port_mask_t mask)
{
	arch_ioport_port_to_base(port)->gperc = mask;
}

__always_inline static void arch_ioport_enable_pin(ioport_pin_t pin)
{
	arch_ioport_enable_port(arch_ioport_pin_to_port_id(pin),
			arch_ioport_pin_to_mask(pin));
}

__always_inline static void arch_ioport_disable_pin(ioport_pin_t pin)
{
	arch_ioport_disable_port(arch_ioport_pin_to_port_id(pin),
			arch_ioport_pin_to_mask(pin));
}

__always_inline static void arch_ioport_set_port_mode(ioport_port_t port,
		ioport_port_mask_t mask, ioport_mode_t mode)
{
	volatile avr32_gpio_port_t *base = arch_ioport_port_to_base(port);

	if (mode & IOPORT_MODE_PULLUP) {
		base->puers = mask;
	} else {
		base->puerc = mask;
	}

#ifdef IOPORT_MODE_PULLDOWN
	if (mode & IOPORT_MODE_PULLDOWN) {
		base->pders = mask;
	} else {
		base->pderc = mask;
	}

#endif

#ifdef IOPORT_MODE_OPEN_DRAIN
	if (mode & IOPORT_MODE_OPEN_DRAIN) {
		base->odmers = mask;
	} else {
		base->odmerc = mask;
	}

#endif

	if (mode & IOPORT_MODE_GLITCH_FILTER) {
		base->gfers = mask;
	} else {
		base->gferc = mask;
	}

#ifdef IOPORT_MODE_DRIVE_STRENGTH
	if (mode & IOPORT_MODE_DRIVE_STRENGTH) {
		base->odcr0s = mask;
	} else {
		base->odcr0c = mask;
	}

#endif

	if (mode & IOPORT_MODE_MUX_BIT0) {
		base->pmr0s = mask;
	} else {
		base->pmr0c = mask;
	}

	if (mode & IOPORT_MODE_MUX_BIT1) {
		base->pmr1s = mask;
	} else {
		base->pmr1c = mask;
	}

#ifdef IOPORT_MODE_MUX_BIT2
	if (mode & IOPORT_MODE_MUX_BIT2) {
		base->pmr2s = mask;
	} else {
		base->pmr2c = mask;
	}

#endif
}

__always_inline static void arch_ioport_set_pin_mode(ioport_pin_t pin,
		ioport_mode_t mode)
{
	arch_ioport_set_port_mode(arch_ioport_pin_to_port_id(pin),
			arch_ioport_pin_to_mask(pin), mode);
}

__always_inline static void arch_ioport_set_port_dir(ioport_port_t port,
		ioport_port_mask_t mask, unsigned char group_direction)
{
	if (group_direction == IOPORT_DIR_OUTPUT) {
		arch_ioport_port_to_base(port)->oders = mask;
	} else if (group_direction == IOPORT_DIR_INPUT) {
		arch_ioport_port_to_base(port)->oderc = mask;
	}
}

__always_inline static void arch_ioport_set_pin_dir(ioport_pin_t pin,
		enum ioport_direction dir)
{
	if (dir == IOPORT_DIR_OUTPUT) {
		arch_ioport_pin_to_base(pin)->oders = arch_ioport_pin_to_mask(
				pin);
	} else if (dir == IOPORT_DIR_INPUT) {
		arch_ioport_pin_to_base(pin)->oderc = arch_ioport_pin_to_mask(
				pin);
	}
}

__always_inline static void arch_ioport_set_pin_level(ioport_pin_t pin,
		bool level)
{
	if (level) {
		arch_ioport_pin_to_base(pin)->ovrs
			= arch_ioport_pin_to_mask(pin);
	} else {
		arch_ioport_pin_to_base(pin)->ovrc
			= arch_ioport_pin_to_mask(pin);
	}
}

__always_inline static void arch_ioport_set_port_level(ioport_port_t port,
		ioport_port_mask_t mask, enum ioport_value level)
{
	volatile avr32_gpio_port_t *base = arch_ioport_port_to_base(port);
	if (level) {
		base->ovrs |= mask;
		base->ovrc &= ~mask;
	} else {
		base->ovrs &= ~mask;
		base->ovrc |= mask;
	}
}

__always_inline static bool arch_ioport_get_pin_level(ioport_pin_t pin)
{
	return arch_ioport_pin_to_base(pin)->pvr & arch_ioport_pin_to_mask(pin);
}

__always_inline static ioport_port_mask_t arch_ioport_get_port_level(
		ioport_port_t port, ioport_port_mask_t mask)
{
	return arch_ioport_port_to_base(port)->pvr & mask;
}

__always_inline static void arch_ioport_toggle_pin_level(ioport_pin_t pin)
{
	arch_ioport_pin_to_base(pin)->ovrt = arch_ioport_pin_to_mask(pin);
}

__always_inline static void arch_ioport_toggle_port_level(ioport_port_t port,
		ioport_port_mask_t mask)
{
	arch_ioport_port_to_base(port)->ovrt = mask;
}

__always_inline static void arch_ioport_set_port_sense_mode(ioport_port_t port,
		ioport_port_mask_t mask, enum ioport_sense pin_sense)
{
	volatile avr32_gpio_port_t *base = arch_ioport_port_to_base(port);

	if (pin_sense & 0x01) {
		base->imr0s = mask;
	} else {
		base->imr0c = mask;
	}

	if (pin_sense & 0x02) {
		base->imr1s = mask;
	} else {
		base->imr1c = mask;
	}
}

__always_inline static void arch_ioport_set_pin_sense_mode(ioport_pin_t pin,
		enum ioport_sense pin_sense)
{
	arch_ioport_set_port_sense_mode(arch_ioport_pin_to_port_id(pin),
			arch_ioport_pin_to_mask(pin), pin_sense);
}

#endif /* IOPORT_UC3_H */
