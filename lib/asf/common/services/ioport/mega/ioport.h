/**
 * \file
 *
 * \brief MEGA and MEGA_RF architecture specific IOPORT service implementation
 * header file.
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
#ifndef IOPORT_MEGA_RF_H
#define IOPORT_MEGA_RF_H

#define IOPORT_CREATE_PIN(port, pin) ((IOPORT_ ## port) * 8 + (pin))
#define IOPORT_BASE_ADDRESS 0x20
#define IOPORT_PORT_OFFSET  0x03

/** \name IOPORT port numbers */
/** @{ */

#if MEGA_XX0_1 || MEGA_RF || MEGA_UNSPECIFIED || MEGA_XX4 || MEGA_XX4_A
#define IOPORT_PORTA    0 /* not connected to pins for megaRF*/
#endif

#if MEGA_XX0_1 || MEGA_RF || MEGA_UNSPECIFIED || MEGA_XX8 || MEGA_XX8_A || \
	MEGA_XX4 || MEGA_XX4_A
#define IOPORT_PORTB    1
#define IOPORT_PORTC    2 /* not connected to pins for megaRF*/
#define IOPORT_PORTD    3
#endif

#if AVR8_PART_IS_DEFINED(ATmega328PB) || AVR8_PART_IS_DEFINED(ATmega324PB)
#define IOPORT_PORTE    4
#endif

#if MEGA_XX0_1 || MEGA_RF
#define IOPORT_PORTE    4
#define IOPORT_PORTF    5
#define IOPORT_PORTG    6
#endif

#if MEGA_XX0
#define IOPORT_PORTH    74
#define IOPORT_PORTJ    75
#define IOPORT_PORTK    76
#define IOPORT_PORTL    77

// base address for ports with addresses greater 0x100
#define IOPORT_BASE_ADDRESS_H 0x100
#endif
/** @} */

/**
 * \weakgroup ioport_service_group
 * \section ioport_modes IOPORT Modes
 *
 * For details on these please see the megaRF Manual.
 *
 * @{
 */

/** \name IOPORT Mode bit definitions */
/** @{ */
#define IOPORT_MODE_PULLDOWN        (0x01) /*!< Pull-down */
#define IOPORT_MODE_PULLUP          (0x00) /*!< Pull-up */
/** @} */

typedef uint8_t ioport_mode_t;
typedef uint16_t ioport_pin_t;
typedef uint8_t ioport_port_t;
typedef uint8_t ioport_port_mask_t;

/* I/O Ports */
typedef struct PORT_struct {
	volatile uint8_t PINCRL;         /* I/O Port PIN DATA READ ONLY */
	volatile uint8_t DIR;            /* I/O Port Data Direction Set */
	volatile uint8_t PORTDATA;       /* I/O Port DATA register */
} PORT_t;

/**
 * \brief A pin mask
 *
 * This type is used to describe the port pin mask on the part.
 */
typedef uint8_t pin_mask_t;

/**
 * \brief A PORT pin
 *
 * This type is used to describe the PORT pins on the part.
 */
typedef uint8_t port_pin_t;

/**
 * \brief Pin configuration flags
 *
 * This is a bitmask containing configuration flags for the pins that shall be
 * configured.
 */
typedef uint16_t port_pin_flags_t;

/**
 * \brief A port id
 *
 * This type is used to describe the port id on the part (0 is PORTA).
 */
typedef uint8_t port_id_t;

/** \name Initial Output State Flags */

/*With Port as input port the initial status is either tri state
 * or pulled high, based on PORTX value, and check the PUD value to make
 * sure pull up resistance is not disabled*/
/** @{ */
#define IOPORT_INIT_LOW           0 << 1  /*!< Initial Output State Low */
#define IOPORT_INIT_HIGH          1 << 1 /*!< Initial Output State High */

/** @} */

/** \name Output and Pull Configuration Flags */
/** @{ */
#define IOPORT_PULL_DOWN          (0 << 2) /*!< Pull-Down (when input) */
#define IOPORT_PULL_UP            (1 << 2) /*!< Pull-Up (when input) */
/** @} */

/**
 * \brief: To get the port number from pin nuber
 * \param: pin. the pin number defined by IOPORT_CREATE_PIN
 */
__always_inline static inline ioport_port_t arch_ioport_pin_to_port_id(
		uint8_t pin)
{
	return pin >> 3;
}

/**
 * \brief: To get the base address of port number
 * \param: port number e.g. IOPORT_PORTB
 */
__always_inline static PORT_t *arch_ioport_port_to_base(uint8_t port)
{
#ifdef IOPORT_BASE_ADDRESS_H	
	if (port >= IOPORT_PORTH) {
	    return (PORT_t *)((uintptr_t)(IOPORT_BASE_ADDRESS_H  + ((port - IOPORT_PORTH) * IOPORT_PORT_OFFSET)));	
	} else 
#endif
	{
	    return (PORT_t *)((uintptr_t)(IOPORT_BASE_ADDRESS + (port * IOPORT_PORT_OFFSET)));
	}
}

/**
 * \brief: To get the base address of port number from defined pin
 * \param: pin. the pin number defined by IOPORT_CREATE_PIN
 */
__always_inline static PORT_t *arch_ioport_pin_to_base(ioport_pin_t pin)
{
	return arch_ioport_port_to_base(pin >> 3);
}

/**
 * \brief: To get the 8-bit pin mask for pin
 * \param: pin. the pin number defined by IOPORT_CREATE_PIN
 */
__always_inline static ioport_port_mask_t arch_ioport_pin_to_mask
	(ioport_pin_t pin)
{
	return 1U << (pin & 0x07);
}

/**
 * \brief Set multiple pin modes in a single GPIO port, such as pull-up,
 * pull-down, etc. configuration.
 *
 * \param port GPIO port to configure  e.g. IOPORT_PORTB
 * \param mask Pin mask of pins to configure
 * \param mode Mode masks to configure for the specified pins (\ref
 * ioport_modes)
 */
__always_inline static void arch_ioport_set_port_mode(ioport_port_t port,
		ioport_port_mask_t mask, ioport_mode_t mode)
{
#ifdef MEGA_RF
	PORT_t *base = arch_ioport_port_to_base(port);
	if (mode == IOPORT_MODE_PULLUP) {
		base->PORTDATA = mask;
	} else if (mode == IOPORT_MODE_PULLDOWN) {
		base->PORTDATA = ~mask;
	}
#endif
}

/**
 * \brief Set multiple pin modes in a single GPIO port, such as pull-up,
 * pull-down, etc. configuration.
 *
 * \param: pin. the pin number defined by IOPORT_CREATE_PIN
 * \param mask Pin mask of pins to configure
 * \param mode Mode masks to configure for the specified pins (\ref
 * ioport_modes)
 */
__always_inline static void arch_ioport_set_pin_mode(ioport_pin_t pin,
		ioport_mode_t mode)
{
#ifdef MEGA_RF
	PORT_t *base = arch_ioport_pin_to_base(pin);
	ioport_pin_t mask_pin = arch_ioport_pin_to_mask(pin);
	if (mode == IOPORT_MODE_PULLUP) {
		base->PORTDATA |=  mask_pin;
	} else if (mode == IOPORT_MODE_PULLDOWN) {
		base->PORTDATA &= ~mask_pin;
	}
#endif
}

/**
 * \brief : Set port direction as input or output.
 * \param port GPIO port to configure  e.g. IOPORT_PORTB
 * \param mask Pin mask of pins to configure
 * \param mode dir, \refer ioport_direction
 */
__always_inline static void arch_ioport_set_port_dir(ioport_port_t port,
		ioport_port_mask_t mask, enum ioport_direction dir)
{
	PORT_t *base = arch_ioport_port_to_base(port);

	if (dir == IOPORT_DIR_OUTPUT) {
		base->DIR = mask;
	} else if (dir == IOPORT_DIR_INPUT) {
		base->DIR = ~mask;
	}
}

/**
 * \brief : Set pin direction as input or output.
 * \param: pin. the pin number defined by IOPORT_CREATE_PIN
 * \param mask Pin mask of pins to configure
 * \param mode dir, \refer ioport_direction
 */
__always_inline static void arch_ioport_set_pin_dir(ioport_pin_t pin,
		enum ioport_direction dir)
{
	PORT_t *base = arch_ioport_pin_to_base(pin);

	if (dir == IOPORT_DIR_OUTPUT) {
		base->DIR |= arch_ioport_pin_to_mask(pin);
	} else if (dir == IOPORT_DIR_INPUT) {
		base->DIR &=  ~arch_ioport_pin_to_mask(pin);
	}
}

/**
 * \brief : Set pin level to high or low.
 * \param: pin. the pin number defined by IOPORT_CREATE_PIN
 * \param: level high/low
 */
__always_inline static void arch_ioport_set_pin_level(ioport_pin_t pin,
		bool level)
{
	PORT_t *base_add = arch_ioport_port_to_base(pin >> 3);

	if (level) {
		base_add->PORTDATA
			= (base_add->PORTDATA  | arch_ioport_pin_to_mask
					(pin));
	} else {
		base_add->PORTDATA
			= (base_add->PORTDATA  & (~arch_ioport_pin_to_mask
					(pin)));
	}
}

/**
 * \brief : Set port multiple pins level to high or low.
 * \param port GPIO port to configure  e.g. IOPORT_PORTB
 * \param: mask Pin mask of pins to configure.
 * \param: level high/low
 */
__always_inline static void arch_ioport_set_port_level(ioport_port_t port,
		ioport_port_mask_t mask, enum ioport_value level)
{
	PORT_t *base = arch_ioport_port_to_base(port);

	if (level) {
		base->PORTDATA |= mask;
	} else {
		base->PORTDATA &= ~mask;
	}
}

/**
 * \brief : Get pin level of pin.
 * \param: pin. the pin number defined by IOPORT_CREATE_PIN
 * \return: level high/low
 */
__always_inline static bool arch_ioport_get_pin_level(ioport_pin_t pin)
{
	PORT_t *base = arch_ioport_pin_to_base(pin);
	return base->PINCRL & arch_ioport_pin_to_mask(pin);
}

/**
 * \brief : Get multiple pins level of single port.
 * \param port GPIO port to configure  e.g. IOPORT_PORTB
 * \param mask Pin mask of pins to read data.
 * \return: 8 bit value based on level voltage on pins.
 */
__always_inline static ioport_port_mask_t arch_ioport_get_port_level(
		ioport_port_t port, ioport_port_mask_t mask)
{
	PORT_t *base = arch_ioport_port_to_base(port);

	return (base->PINCRL & mask);
}

/**
 * \brief : To toggle the pin
 * \param: pin. the pin number defined by IOPORT_CREATE_PIN
 */
__always_inline static void arch_ioport_toggle_pin_level(ioport_pin_t pin)
{
	PORT_t *base = arch_ioport_pin_to_base(pin);

	base->PINCRL = arch_ioport_pin_to_mask(pin);
}

/**
 * \brief : To toggle the multiple pins on a single port.
 * \param port GPIO port to toggle  e.g. IOPORT_PORTB
 * \param mask Pin mask of pins to read data.
 */

__always_inline static void arch_ioport_toggle_port_level(ioport_port_t port,
		ioport_port_mask_t mask)
{
	PORT_t *base = arch_ioport_port_to_base(port);

	base->PINCRL = mask;
}

/**
 * \brief : To configure a single GPIO pin for trigger modes:
 *  sense the edge/level/both trigger.
 * \param: pin. the pin number defined by IOPORT_CREATE_PIN
 * \param pin_sense, refer to \ioport_sense.
 */

__always_inline static void arch_ioport_set_pin_sense_mode(ioport_pin_t pin,
		enum ioport_sense pin_sense)
{
	uint8_t flags = cpu_irq_save();
	port_id_t port_id = arch_ioport_pin_to_port_id(pin);
#if  MEGA_XX0_1 || MEGA_RF
	if (port_id == IOPORT_PORTD && ((pin & 0x07) < 0x04)) {
		EICRA &= ~((0x03) << ((pin & 0x03) * 2));
		EICRA |= pin_sense << ((pin & 0x03) * 2);
	} else if (port_id == IOPORT_PORTE && ((pin & 0x07) > 0x03)) {
		EICRB &= ~((0x03) << ((pin & 0x03) * 2));
		EICRB |= pin_sense << ((pin & 0x03) * 2);
	}
#endif

#if  MEGA_XX8
	if (port_id == IOPORT_PORTD &&
			((pin & 0x07) == 2 || (pin & 0x07) == 3)) {
		EICRA &= ~((0x03) << (((pin & 0x03) - 2) * 2));
		EICRA |= pin_sense << (((pin & 0x03) - 2) * 2);
	}
#endif

#if  MEGA_XX4
	if (port_id == IOPORT_PORTD &&
			((pin & 0x07) == 2 || (pin & 0x07) == 3)) {
		EICRA &= ~((0x03) << (((pin & 0x03) - 2) * 2));
		EICRA |= pin_sense << (((pin & 0x03) - 2) * 2);
	}

	if (port_id == IOPORT_PORTB && ((pin & 0x07) == 2)) {
		EICRA &= ~((0x03) << 4);
		EICRA |= pin_sense << 4;
	}
#endif
	cpu_irq_restore(flags);
}

/**
 * \brief : To configure a multiple pins for trigger modes: sense the edge/
 *  level/both trigger.
 * \param: pin. the pin number defined by IOPORT_CREATE_PIN
 * \param pin_sense, refer to \ioport_sense.
 */
__always_inline static void arch_ioport_set_port_sense_mode(ioport_port_t port,
		ioport_port_mask_t mask, enum ioport_sense pin_sense)
{
	uint8_t flags = cpu_irq_save();

#if  !MEGA_UNSPECIFIED
	if (port == IOPORT_PORTD) {
		EICRA =  mask;
		EICRA &= (pin_sense << 6) || (pin_sense << 4) ||
				(pin_sense << 2) ||
				(pin_sense);
	}
#endif
#if  MEGA_XX8
	if (port == IOPORT_PORTD) {
		EICRA = mask;
		EICRA &= (pin_sense << 2) || (pin_sense);
	}
#endif
#if  MEGA_XX0_1 || MEGA_RF
	else if (port == IOPORT_PORTE) {
		EICRB = mask;
		EICRB &= (pin_sense << 6) || (pin_sense << 4) ||
				(pin_sense << 2) ||
				(pin_sense);
	}
#endif

	cpu_irq_restore(flags);
}

/****************************************************************************
 *
 * Functional register are not available for this architecture through
 * IOPORT register.
 *
 *****************************************************************************/
__always_inline static void arch_ioport_init(void)
{
}

__always_inline static void arch_ioport_enable_pin(ioport_pin_t pin)
{
}

__always_inline static void arch_ioport_enable_port(ioport_port_t port,
		ioport_port_mask_t mask)
{
}

__always_inline static void arch_ioport_disable_pin(ioport_pin_t pin)
{
}

__always_inline static void arch_ioport_disable_port(ioport_port_t port,
		ioport_port_mask_t mask)
{
}

/****************************************************************************/

/**
 * \brief : To get the port address pointer from pin
 * \param: pin. the pin number defined by IOPORT_CREATE_PIN
 * \return pointer to base address of ports(8 bit)
 */
__always_inline static inline PORT_t *ioport_pin_to_port(port_pin_t pin)
{
	return arch_ioport_pin_to_base(pin);
}

/**
 * \brief : To get the port address pointer from port number
 * \param port GPIO port to toggle  e.g. IOPORT_PORTB
 * \return pointer to base address of ports(8 bit)
 */

__always_inline static inline PORT_t *ioport_id_pin_to_port(port_id_t port)
{
	return arch_ioport_port_to_base(port);
}

/**
 * \brief Configure the IO PORT pin function for a set of pins on a port
 *
 * \param port Pointer to the port
 * \param pin_mask Mask containing the pins that should be configured
 * \param flags Bitmask of flags specifying additional configuration
 * parameters.
 */
__always_inline static inline void ioport_configure_port_pin(void *port,
		pin_mask_t pin_mask,
		port_pin_flags_t flags)
{
	/* Select direction and initial pin state */
	if (flags & IOPORT_DIR_OUTPUT) {
		if (flags & IOPORT_INIT_HIGH) {
			*((uint8_t *)port + 2) |= pin_mask;
		} else {
			*((uint8_t *)port + 2) &= ~pin_mask;
		}

		*((uint8_t *)port + 1) |= pin_mask;
	} else {
		*((uint8_t *)port + 1) &= ~pin_mask;
		if (flags & IOPORT_PULL_UP) {
			*((uint8_t *)port + 2) |= pin_mask;
		} else {
			*((uint8_t *)port + 2) &= ~pin_mask;
		}
	}
}

/**
 * \brief Select the port function for a single pin
 * \param: pin. the pin number defined by IOPORT_CREATE_PIN
 * \param flags Bitmask of flags specifying additional configuration
 * parameters.
 */
__always_inline static inline void ioport_configure_pin(port_pin_t pin,
		port_pin_flags_t flags)
{
	ioport_configure_port_pin(arch_ioport_pin_to_base(pin),
			arch_ioport_pin_to_mask(pin), flags);
}

/**
 * \brief Configure a group of I/O pins on a specified port number
 *
 * \param port The port number
 * \param pin_mask The pin mask to configure
 * \param flags Bitmask of flags specifying additional configuration
 * parameters.
 */
__always_inline static inline void ioport_configure_group(port_id_t port,
		pin_mask_t pin_mask,
		port_pin_flags_t flags)
{
	ioport_configure_port_pin(arch_ioport_port_to_base(
			port), pin_mask, flags);
}

/**
 * \brief Drive a PORT pin to a given state
 *
 * This function will only have an effect if \a pin is configured as
 * an output.
 *
 * \param pin A number identifying the pin to act on.
 * \param value The desired state of the pin. \a true means drive the
 * pin high (towards Vdd), while \a false means drive the pin low
 * (towards Vss).
 */
__always_inline static inline void ioport_set_value(port_pin_t pin, bool value)
{
	arch_ioport_set_pin_level(pin, value);
}

/**
 * \brief Drive a PORT pin to a low level
 *
 * This function will only have an effect if \a pin is configured as
 * an output.
 *
 * \param: pin. the pin number defined by IOPORT_CREATE_PIN
 */
__always_inline static inline void ioport_set_pin_low(port_pin_t pin)
{
	arch_ioport_set_pin_level(pin, false);
}

/**
 * \brief Drive a PORT pin to a high level
 *
 * This function will only have an effect if \a pin is configured as
 * an output.
 *
 * \param pin A number identifying the pin to act on.
 */
__always_inline static inline void ioport_set_pin_high(port_pin_t pin)
{
	arch_ioport_set_pin_level(pin, true);
}

/**
 * \brief Read the current state of a PORT pin
 *
 * \param pin A number identifying the pin to read.
 * \retval true The pin is currently high (close to Vdd)
 * \retval false The pin is currently low (close to Vss)
 */
__always_inline static inline bool ioport_get_value(port_pin_t pin)
{
	return arch_ioport_get_pin_level(pin);
}

/**
 * \brief Read the current state of a PORT pin and test high level
 *
 * \param pin A number identifying the pin to read.
 * \retval true The pin is currently high (close to Vdd)
 * \retval false The pin is currently low (close to Vss)
 */
__always_inline static inline bool ioport_pin_is_high(port_pin_t pin)
{
	return (arch_ioport_get_pin_level(pin) == true);
}

/**
 * \brief Read the current state of a PORT pin and test high level
 *
 * \param pin A number identifying the pin to read.
 * \retval true The pin is currently high (close to Vdd)
 * \retval false The pin is currently low (close to Vss)
 */
__always_inline static inline bool ioport_pin_is_low(port_pin_t pin)
{
	return (arch_ioport_get_pin_level(pin) == false);
}

/**
 * \brief Toggle the current state of a PORT pin
 *
 * \param pin A number identifying the pin to act on.
 */
__always_inline static inline void ioport_toggle_pin(port_pin_t pin)
{
	arch_ioport_toggle_pin_level(pin);
}

/*! \brief Drives a group of I/O pin of a port to high level.
 *
 * \param port_id The port number.
 * \param port_mask The mask.
 */
__always_inline static inline void ioport_set_group_high(port_id_t port_id,
		pin_mask_t port_mask)
{
	arch_ioport_set_port_level(port_id, port_mask, port_mask);
}

/*! \brief Drives a group of I/O pin of a port to low level.
 *
 * \param port_id The port number.
 * \param port_mask The mask.
 */
__always_inline static inline void ioport_set_group_low(port_id_t port_id,
		pin_mask_t port_mask)
{
	arch_ioport_set_port_level(port_id, port_mask, 0);
}

/*! \brief Toggles a group of I/O pin of a port.
 *
 * \param port_id The port number.
 * \param port_mask The mask.
 */
__always_inline static inline void ioport_tgl_group(port_id_t port_id,
		pin_mask_t port_mask)
{
	arch_ioport_toggle_port_level(port_id, port_mask);
}

#endif /* IOPORT_MEGA_RF_H */
