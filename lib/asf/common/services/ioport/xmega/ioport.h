/**
 * \file
 *
 * \brief XMEGA architecture specific IOPORT service implementation header file.
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
#ifndef IOPORT_XMEGA_H
#define IOPORT_XMEGA_H

#define IOPORT_CREATE_PIN(port, pin) ((IOPORT_ ## port) * 8 + (pin))
#define IOPORT_BASE_ADDRESS 0x600
#define IOPORT_PORT_OFFSET  0x20

/** \name IOPORT port numbers */
/** @{ */
#if !XMEGA_B3
#  define IOPORT_PORTA  0
#endif

#define IOPORT_PORTB    1
#define IOPORT_PORTC    2
#define IOPORT_PORTD    3

#if !XMEGA_B3
#  define IOPORT_PORTE  4
#endif

#if XMEGA_A1 || XMEGA_A1U || XMEGA_A3 || XMEGA_A3U || XMEGA_A3B || XMEGA_A3BU || \
	XMEGA_C3 || XMEGA_D3
#  define IOPORT_PORTF  5
#endif

#if XMEGA_B1 || XMEGA_B3
#  define IOPORT_PORTG  6
#endif

#if XMEGA_A1 || XMEGA_A1U
#  define IOPORT_PORTH  7
#  define IOPORT_PORTJ  8
#  define IOPORT_PORTK  9
#endif

#if XMEGA_B1 || XMEGA_B3
#  define IOPORT_PORTM  11
#endif

#if XMEGA_A1 || XMEGA_A1U
#  define IOPORT_PORTQ  14
#endif

#define IOPORT_PORTR    15
/** @} */

/**
 * \weakgroup ioport_group
 * \section ioport_modes IOPORT Modes
 *
 * For details on these please see the XMEGA Manual.
 *
 * @{
 */

/** \name IOPORT Mode bit definitions */
/** @{ */
#define IOPORT_MODE_TOTEM           (0x00 << 3) /*!< Totem-pole */
#define IOPORT_MODE_BUSKEEPER       (0x01 << 3) /*!< Buskeeper */
#define IOPORT_MODE_PULLDOWN        (0x02 << 3) /*!< Pull-down */
#define IOPORT_MODE_PULLUP          (0x03 << 3) /*!< Pull-up */
#define IOPORT_MODE_WIREDOR         (0x04 << 3) /*!< Wired OR */
#define IOPORT_MODE_WIREDAND        (0x05 << 3) /*!< Wired AND */
#define IOPORT_MODE_WIREDORPULL     (0x06 << 3) /*!< Wired OR with pull-down */
#define IOPORT_MODE_WIREDANDPULL    (0x07 << 3) /*!< Wired AND with pull-up */
#define IOPORT_MODE_INVERT_PIN      (0x01 << 6) /*!< Invert output and input */
#define IOPORT_MODE_SLEW_RATE_LIMIT (0x01 << 7) /*!< Slew rate limiting */
/** @} */

/** @} */

typedef uint8_t ioport_mode_t;
typedef uint8_t ioport_pin_t;
typedef uint8_t ioport_port_t;
typedef uint8_t ioport_port_mask_t;

__always_inline static ioport_port_t arch_ioport_pin_to_port_id(ioport_pin_t pin)
{
	return pin >> 3;
}

__always_inline static PORT_t *arch_ioport_port_to_base(ioport_port_t port)
{
	return (PORT_t *)((uintptr_t)IOPORT_BASE_ADDRESS +
	       (port * IOPORT_PORT_OFFSET));
}

__always_inline static PORT_t *arch_ioport_pin_to_base(ioport_pin_t pin)
{
	return arch_ioport_port_to_base(arch_ioport_pin_to_port_id(pin));
}

__always_inline static ioport_port_mask_t arch_ioport_pin_to_mask(
		ioport_pin_t pin)
{
	return 1U << (pin & 0x07);
}

__always_inline static ioport_port_mask_t arch_ioport_pin_to_index(
		ioport_pin_t pin)
{
	return (pin & 0x07);
}

__always_inline static void arch_ioport_init(void)
{
}

__always_inline static void arch_ioport_enable_port(ioport_port_t port,
		ioport_port_mask_t mask)
{
	PORT_t *base = arch_ioport_port_to_base(port);
	volatile uint8_t *pin_ctrl = &base->PIN0CTRL;

	uint8_t flags = cpu_irq_save();

	for (uint8_t i = 0; i < 8; i++) {
		if (mask & arch_ioport_pin_to_mask(i)) {
			pin_ctrl[i] &= ~PORT_ISC_gm;
		}
	}

	cpu_irq_restore(flags);
}

__always_inline static void arch_ioport_enable_pin(ioport_pin_t pin)
{
	PORT_t *base = arch_ioport_pin_to_base(pin);
	volatile uint8_t *pin_ctrl
		= (&base->PIN0CTRL + arch_ioport_pin_to_index(pin));

	uint8_t flags = cpu_irq_save();

	*pin_ctrl &= ~PORT_ISC_gm;

	cpu_irq_restore(flags);
}

__always_inline static void arch_ioport_disable_port(ioport_port_t port,
		ioport_port_mask_t mask)
{
	PORT_t *base = arch_ioport_port_to_base(port);
	volatile uint8_t *pin_ctrl = &base->PIN0CTRL;

	uint8_t flags = cpu_irq_save();

	for (uint8_t i = 0; i < 8; i++) {
		if (mask & arch_ioport_pin_to_mask(i)) {
			pin_ctrl[i] |= PORT_ISC_INPUT_DISABLE_gc;
		}
	}

	cpu_irq_restore(flags);
}

__always_inline static void arch_ioport_disable_pin(ioport_pin_t pin)
{
	PORT_t *base = arch_ioport_pin_to_base(pin);
	volatile uint8_t *pin_ctrl
		= (&base->PIN0CTRL + arch_ioport_pin_to_index(pin));

	uint8_t flags = cpu_irq_save();

	*pin_ctrl |= PORT_ISC_INPUT_DISABLE_gc;

	cpu_irq_restore(flags);
}

__always_inline static void arch_ioport_set_port_mode(ioport_port_t port,
		ioport_port_mask_t mask, ioport_mode_t mode)
{
	PORT_t *base = arch_ioport_port_to_base(port);
	volatile uint8_t *pin_ctrl = &base->PIN0CTRL;
	uint8_t new_mode_bits = (mode & ~PORT_ISC_gm);

	uint8_t flags = cpu_irq_save();

	for (uint8_t i = 0; i < 8; i++) {
		if (mask & arch_ioport_pin_to_mask(i)) {
			pin_ctrl[i]
				= (pin_ctrl[i] &
					PORT_ISC_gm) | new_mode_bits;
		}
	}

	cpu_irq_restore(flags);
}

__always_inline static void arch_ioport_set_pin_mode(ioport_pin_t pin,
		ioport_mode_t mode)
{
	PORT_t *base = arch_ioport_pin_to_base(pin);
	volatile uint8_t *pin_ctrl
		= (&base->PIN0CTRL + arch_ioport_pin_to_index(pin));

	uint8_t flags = cpu_irq_save();

	*pin_ctrl &= PORT_ISC_gm;
	*pin_ctrl |= mode;

	cpu_irq_restore(flags);
}

__always_inline static void arch_ioport_set_port_dir(ioport_port_t port,
		ioport_port_mask_t mask, enum ioport_direction dir)
{
	PORT_t *base = arch_ioport_port_to_base(port);

	if (dir == IOPORT_DIR_OUTPUT) {
		base->DIRSET = mask;
	} else if (dir == IOPORT_DIR_INPUT) {
		base->DIRCLR = mask;
	}
}

__always_inline static void arch_ioport_set_pin_dir(ioport_pin_t pin,
		enum ioport_direction dir)
{
	PORT_t *base = arch_ioport_pin_to_base(pin);

	if (dir == IOPORT_DIR_OUTPUT) {
		base->DIRSET = arch_ioport_pin_to_mask(pin);
	} else if (dir == IOPORT_DIR_INPUT) {
		base->DIRCLR = arch_ioport_pin_to_mask(pin);
	}
}

__always_inline static void arch_ioport_set_pin_level(ioport_pin_t pin,
		bool level)
{
	PORT_t *base = arch_ioport_pin_to_base(pin);

	if (level) {
		base->OUTSET = arch_ioport_pin_to_mask(pin);
	} else {
		base->OUTCLR = arch_ioport_pin_to_mask(pin);
	}
}

__always_inline static void arch_ioport_set_port_level(ioport_port_t port,
		ioport_port_mask_t mask, enum ioport_value level)
{
	PORT_t *base = arch_ioport_port_to_base(port);
	if (level) {
		base->OUTSET |= mask;
		base->OUTCLR &= ~mask;
	} else {
		base->OUTSET &= ~mask;
		base->OUTCLR |= mask;
	}
}

__always_inline static bool arch_ioport_get_pin_level(ioport_pin_t pin)
{
	PORT_t *base = arch_ioport_pin_to_base(pin);

	return base->IN & arch_ioport_pin_to_mask(pin);
}

__always_inline static ioport_port_mask_t arch_ioport_get_port_level(
		ioport_port_t port, ioport_port_mask_t mask)
{
	PORT_t *base = arch_ioport_port_to_base(port);

	return base->IN & mask;
}

__always_inline static void arch_ioport_toggle_pin_level(ioport_pin_t pin)
{
	PORT_t *base = arch_ioport_pin_to_base(pin);

	base->OUTTGL = arch_ioport_pin_to_mask(pin);
}

__always_inline static void arch_ioport_toggle_port_level(ioport_port_t port,
		ioport_port_mask_t mask)
{
	PORT_t *base = arch_ioport_port_to_base(port);

	base->OUTTGL = mask;
}

__always_inline static void arch_ioport_set_pin_sense_mode(ioport_pin_t pin,
		enum ioport_sense pin_sense)
{
	PORT_t *base = arch_ioport_pin_to_base(pin);
	volatile uint8_t *pin_ctrl
		= (&base->PIN0CTRL + arch_ioport_pin_to_index(pin));

	uint8_t flags = cpu_irq_save();

	*pin_ctrl &= ~PORT_ISC_gm;
	*pin_ctrl |= (pin_sense & PORT_ISC_gm);

	cpu_irq_restore(flags);
}

__always_inline static void arch_ioport_set_port_sense_mode(ioport_port_t port,
		ioport_port_mask_t mask, enum ioport_sense pin_sense)
{
	PORT_t *base = arch_ioport_port_to_base(port);
	volatile uint8_t *pin_ctrl = &base->PIN0CTRL;
	uint8_t new_sense_bits = (pin_sense & PORT_ISC_gm);

	uint8_t flags = cpu_irq_save();

	for (uint8_t i = 0; i < 8; i++) {
		if (mask & arch_ioport_pin_to_mask(i)) {
			pin_ctrl[i]
				= (pin_ctrl[i] &
					~PORT_ISC_gm) | new_sense_bits;
		}
	}

	cpu_irq_restore(flags);
}

#endif /* IOPORT_XMEGA_H */
