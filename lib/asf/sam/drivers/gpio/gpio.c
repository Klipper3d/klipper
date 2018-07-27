/**
 * \file
 *
 * \brief GPIO driver.
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

#include "gpio.h"

/**
 * Maximum number of interrupt sources that can be defined. This
 * constant can be increased, but the current value is the smallest possible
 * one that will be compatible with all existing projects.
 */
#ifndef GPIO_MAX_INTERRUPT_SOURCES
#define GPIO_MAX_INTERRUPT_SOURCES       6
#endif

/** GPIO interrupt generators are grouped into groups of eight  */
#define GPIO_INT_GROUP_MASK    0xFFu

/**
 * Structure for a GPIO pin interrupt source.
 */
struct gpio_interrupt_source {
	uint32_t pin;                 /**< The pin number */
	gpio_pin_callback_t callback; /**< Interrupt callback function. */
};

/* List of interrupt sources. */
static struct gpio_interrupt_source gpio_int_sources[GPIO_MAX_INTERRUPT_SOURCES];

/* Number of currently defined interrupt sources. */
static uint32_t gpio_nb_sources = 0;

/**
 * \brief Set callback for given GPIO pin
 *
 * \param [in] pin The pin number
 * \param [in] callback callback function pointer
 * \param [in] irq_level interrupt level
 *
 * \retval true Set successfully
 * \retval false Wrong parameters or maximum number of interrupt
 *                  sources has been exceeding
 */
bool gpio_set_pin_callback(ioport_pin_t pin, gpio_pin_callback_t callback,
		uint8_t irq_level)
{
	int8_t i;
	int8_t irq_line;

	if (gpio_nb_sources >= GPIO_MAX_INTERRUPT_SOURCES) {
		return false;
	}

	/*
	 * Get IRQ line for the given pin.
	 *
	 * \note Grouping interrupt generators into groups of eight, four
	 * different interrupt handlers can be installed for each GPIO port.
	 */
	for (i = 0; i < 4; i++) {
		if (ioport_pin_to_mask(pin) & (GPIO_INT_GROUP_MASK << (i * 8))) {
			break;
		}
	}
	irq_line = GPIO_0_IRQn + ioport_pin_to_port_id(pin) * 4 + i;

	gpio_int_sources[gpio_nb_sources].pin = pin;
	gpio_int_sources[gpio_nb_sources].callback = callback;
	NVIC_ClearPendingIRQ((IRQn_Type)irq_line);
	NVIC_SetPriority((IRQn_Type)irq_line, irq_level);
	NVIC_EnableIRQ((IRQn_Type)irq_line);

	gpio_nb_sources++;

	return true;
}

/**
 * Common GPIO handler.
 */
static void gpio_common_handler(uint32_t port_id, uint32_t port_mask)
{
	GpioPort *gpio_port = &(GPIO->GPIO_PORT[port_id]);
	uint32_t i;
	uint32_t int_flags;
	ioport_pin_t pin;

	int_flags = gpio_port->GPIO_IFR;

	for (i = 0; i < gpio_nb_sources; i++) {
		pin = gpio_int_sources[i].pin;
		if ((ioport_pin_to_port_id(pin) == port_id) &&
				(ioport_pin_to_mask(pin) & int_flags)) {
			if (gpio_int_sources[i].callback != NULL) {
				gpio_int_sources[i].callback();
			} else {
				Assert(false); /* Catch unexpected interrupt */
			}
		}
	}

	gpio_port->GPIO_IFRC = (int_flags & port_mask);
}

/**
 * GPIO handler 0 (PA 0..7)
 */
void GPIO_0_Handler(void)
{
	gpio_common_handler(IOPORT_GPIOA, GPIO_INT_GROUP_MASK);
}

/**
 * GPIO handler 1 (PA 8..15)
 */
void GPIO_1_Handler(void)
{
	gpio_common_handler(IOPORT_GPIOA, (GPIO_INT_GROUP_MASK << 8));
}

/**
 * GPIO handler 2 (PA 16..23)
 */
void GPIO_2_Handler(void)
{
	gpio_common_handler(IOPORT_GPIOA, (GPIO_INT_GROUP_MASK << 16));
}

/**
 * GPIO handler 3 (PA 24..31)
 */
void GPIO_3_Handler(void)
{
	gpio_common_handler(IOPORT_GPIOA, (GPIO_INT_GROUP_MASK << 24));
}

/**
 * GPIO handler 4 (PB 0..7)
 */
void GPIO_4_Handler(void)
{
	gpio_common_handler(IOPORT_GPIOB, GPIO_INT_GROUP_MASK);
}

/**
 * GPIO handler 5 (PB 8..15)
 */
void GPIO_5_Handler(void)
{
	gpio_common_handler(IOPORT_GPIOB, (GPIO_INT_GROUP_MASK << 8));
}

/**
 * GPIO handler 6 (PB 16..23)
 */
void GPIO_6_Handler(void)
{
	gpio_common_handler(IOPORT_GPIOB, (GPIO_INT_GROUP_MASK << 16));
}

/**
 * GPIO handler 7 (PB 24..31)
 */
void GPIO_7_Handler(void)
{
	gpio_common_handler(IOPORT_GPIOB, (GPIO_INT_GROUP_MASK << 24));
}

/**
 * GPIO handler 8 (PC 0..7)
 */
void GPIO_8_Handler(void)
{
	gpio_common_handler(IOPORT_GPIOC, GPIO_INT_GROUP_MASK);
}

/**
 * GPIO handler 9 (PC 8..15)
 */
void GPIO_9_Handler(void)
{
	gpio_common_handler(IOPORT_GPIOC, (GPIO_INT_GROUP_MASK << 8));
}

/**
 * GPIO handler 10 (PC 16..23)
 */
void GPIO_10_Handler(void)
{
	gpio_common_handler(IOPORT_GPIOC, (GPIO_INT_GROUP_MASK << 16));
}

/**
 * GPIO handler 11 (PC 24..31)
 */
void GPIO_11_Handler(void)
{
	gpio_common_handler(IOPORT_GPIOC, (GPIO_INT_GROUP_MASK << 24));
}
