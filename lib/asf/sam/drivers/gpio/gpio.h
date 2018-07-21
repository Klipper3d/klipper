/**
 * \file
 *
 * \brief SAM GPIO driver.
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
#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

/**
 * \defgroup asfdoc_sam_drivers_gpio_group SAM4L General-Purpose Input/Output (GPIO) Driver
 * 
 *
 * This driver for Atmel&reg; | SMART ARM&reg;-based microcontrollers 
 * provides an interface for the configuration and management of 
 * the on-chip General-Purpose Input/Output (GPIO) controller with 
 * both interrupt and event support.
 *
 * Devices from the following series can use this module:
 * - Atmel | SMART SAM4L
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_sam_drivers_gpiox_prerequisites
 *  - \ref asfdoc_sam_drivers_gpiox_module_overview
 *  - \ref asfdoc_sam_drivers_gpiox_special_considerations
 *  - \ref asfdoc_sam_drivers_gpiox_extra_info
 *  - \ref asfdoc_sam_drivers_gpiox_examples
 *  - \ref asfdoc_sam_drivers_gpiox_api_overview
 *
 *
 * \section asfdoc_sam_drivers_gpiox_prerequisites Prerequisites
 *
 * The GPIO module depends on the following modules:
 * - \ref sysclk_group "System Clock Management (Sysclock)" for clock control
 * - \ref interrupt_group "Global interrupt management" for enabling or disabling interrupts
 * - \ref ioport_group "Common IOPORT API" for basic GPIO functions
 *
 *
 * \section asfdoc_sam_drivers_gpiox_module_overview Module Overview
 * The General Purpose Input/Output Controller (GPIO) controls the I/O pins of the microcontroller.
 * Each GPIO pin may be used as a general-purpose I/O or be assigned to a function of an embedded
 * peripheral.
 *
 * The GPIO is configured using the Peripheral Bus (PB).
 *
 *
 * \section asfdoc_sam_drivers_gpiox_special_considerations Special Considerations
 * There are no special considerations for this module.
 *
 *
 * \section asfdoc_sam_drivers_gpiox_extra_info Extra Information
 *
 * For extra information, see \ref asfdoc_sam_drivers_gpiox_extra. This includes:
 *  - \ref asfdoc_sam_drivers_gpiox_extra_acronyms
 *  - \ref asfdoc_sam_drivers_gpiox_extra_dependencies
 *  - \ref asfdoc_sam_drivers_gpiox_extra_errata
 *  - \ref asfdoc_sam_drivers_gpiox_extra_history
 *
 * \section asfdoc_sam_drivers_gpiox_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_sam_drivers_gpiox_exqsg.
 *
 *
 * \section asfdoc_sam_drivers_gpiox_api_overview API Overview
 * @{
 */

#include <compiler.h>
#include <ioport.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Interrupt callback function type for a GPIO pin.
 *
 * The interrupt handler can be configured to do a function callback,
 * the callback function must match the gpio_pin_callback_t type.
 *
 */
typedef void (*gpio_pin_callback_t)(void);

/**
 * \name Interrupt Support
 *
 * The GPIO can be configured to generate an interrupt when it detects a
 * change on a GPIO pin.
 *
 * @{
 */

bool gpio_set_pin_callback(ioport_pin_t pin, gpio_pin_callback_t callback,
		uint8_t irq_level);

/**
 * \brief Enable the interrupt of a pin.
 *
 * \param [in] pin The pin number
 */
static inline void gpio_enable_pin_interrupt(ioport_pin_t pin)
{
	GpioPort *gpio_port = &(GPIO->GPIO_PORT[ioport_pin_to_port_id(pin)]);
	gpio_port->GPIO_IERS = ioport_pin_to_mask(pin);
}

/**
 * \brief Disable the interrupt of a pin.
 *
 * \param [in] pin The pin number
 */
static inline void gpio_disable_pin_interrupt(ioport_pin_t pin)
{
	GpioPort *gpio_port = &(GPIO->GPIO_PORT[ioport_pin_to_port_id(pin)]);
	gpio_port->GPIO_IERC = ioport_pin_to_mask(pin);
}

/**
 * \brief Get the interrupt flag of a pin.
 *
 * \param [in] pin The pin number
 *
 * \retval uint32_t The pin interrupt flag (0/1)
 */
static inline uint32_t gpio_get_pin_interrupt_flag(ioport_pin_t pin)
{
	GpioPort *gpio_port = &(GPIO->GPIO_PORT[ioport_pin_to_port_id(pin)]);
	return (((gpio_port->GPIO_IFR && ioport_pin_to_mask(pin)) == 0) ? 0 : 1);
}

/**
 * \brief Clear the interrupt flag of a pin.
 *
 * \param [in] pin The pin number
 */
static inline void gpio_clear_pin_interrupt_flag(ioport_pin_t pin)
{
	GpioPort *gpio_port = &(GPIO->GPIO_PORT[ioport_pin_to_port_id(pin)]);
	gpio_port->GPIO_IFRC = ioport_pin_to_mask(pin);
}



/**
 * \name Peripheral Event System Support
 *
 * The GPIO can be programmed to output peripheral events whenever an interrupt
 * condition is detected, such as pin value change, or only when a rising or
 * falling edge is detected.
 *
 * @{
 */

/**
 * \brief Enable the peripheral event generation of a pin.
 *
 * \param [in] pin The pin number
 */
static inline void gpio_enable_pin_periph_event(ioport_pin_t pin)
{
	GpioPort *gpio_port = &(GPIO->GPIO_PORT[ioport_pin_to_port_id(pin)]);
	gpio_port->GPIO_EVERS = ioport_pin_to_mask(pin);
}

/**
 * \brief Disable the peripheral event generation of a pin.
 *
 * \param [in] pin The pin number
 *
 */
static inline void gpio_disable_pin_periph_event(ioport_pin_t pin)
{
	GpioPort *gpio_port = &(GPIO->GPIO_PORT[ioport_pin_to_port_id(pin)]);
	gpio_port->GPIO_EVERC = ioport_pin_to_mask(pin);
}

#ifdef __cplusplus
}
#endif

/** @} */
/** @} */

/**
 * \page asfdoc_sam_drivers_gpiox_extra Extra Information for GPIO
 *
 * \section asfdoc_sam_drivers_gpiox_extra_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *  <tr>
 *      <th>Acronym</th>
 *      <th>Definition</th>
 *  </tr>
 *  <tr>
 *		<td>PB</td>
 *		<td>Peripheral Bus</td>
 *  </tr>
 *  <tr>
 *      <td>QSG</td>
 *      <td>Quick Start Guide</td>
 * </tr>
 * </table>
 *
 *
 * \section asfdoc_sam_drivers_gpiox_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - \ref asfdoc_sam_power_management "Power Management"
 *
 * If the CPU enters a sleep mode that disables clocks used by the GPIO, the GPIO will stop functioning
 * and resume operation after the system wakes up from sleep mode.
 * If a peripheral function is configured for a GPIO pin, the peripheral will be able to control the
 * GPIO pin even if the GPIO clock is stopped.
 *  - \ref asfdoc_sam_sysclock "Clocks"
 *
 * The GPIO is connected to a Peripheral Bus clock (CLK_GPIO). This clock is generated by the
 * Power Manager. CLK_GPIO is enabled at reset, and can be disabled by writing to the Power
 * Manager. CLK_GPIO must be enabled in order to access the configuration registers of the GPIO
 * or to use the GPIO interrupts. After configuring the GPIO, the CLK_GPIO can be disabled by
 * writing to the Power Manager if interrupts are not used.
 *  - \ref asfdoc_sam_interrupts "Interrupts"
 *
 * The GPIO interrupt request lines are connected to the NVIC. Using the GPIO interrupts requires
 * the NVIC to be programmed first.
 *  - \ref asfdoc_sam_peripheral_events "Peripheral Events"
 *
 * The GPIO peripheral events are connected via the Peripheral Event System.
 *  - \ref asfdoc_sam_debug_operation "Debug Operation"
 *
 * When an external debugger forces the CPU into debug mode, the GPIO continues normal operation.
 * If the GPIO is configured in a way that requires it to be periodically serviced by the CPU
 * through interrupts or similar, improper operation or data loss may result during debugging.
 *
 * \section asfdoc_sam_drivers_gpiox_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_sam_drivers_gpiox_extra_history Module History
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in
 * the table.
 *
 * <table>
 *	<tr>
 *		<th>Changelog</th>
 *	</tr>
 *	<tr>
 *		<td>Initial document release</td>
 *	</tr>
 * </table>


 * \page asfdoc_sam_drivers_gpiox_exqsg Examples for GPIO
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_sam_drivers_gpio_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that a QSG can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \ref asfdoc_sam_drivers_gpiox_qsg
 *
 * \page asfdoc_sam_drivers_gpiox_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Doc. Rev.</td>
 *		<th>Date</td>
 *		<th>Comments</td>
 *	</tr>
 *	<tr>
 *		<td>42280B</td>
 *		<td>07/2015</td>
 *		<td>Updated title of application note and added list of supported devices</td>
 *	</tr>
 *	<tr>
 *		<td>42280A</td>
 *		<td>05/2014</td>
 *		<td>Initial document release</td>
 *	</tr>
 * </table>
 *
 */

/**
 * \page sam_gpio_quick_start Quick Start Guide for the GPIO driver
 *
 * This is the quick start guide for the \ref group_sam_drivers_gpio, with
 * step-by-step instructions on how to configure and use the driver for
 * a specific use case.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section gpio_qs_use_cases Use Cases
 * - \ref gpio_int
 *
 * \section gpio_int GPIO Interrupt Usage
 *
 * This use case will demonstrate how to configure a pin(PC03) to trigger an
 * interrupt on SAM4L-EK board.
 *
 * \section gpio_int_setup Setup Steps
 *
 * \subsection gpio_int_prereq Prerequisites
 *
 * This module requires the following service:
 * - \ref clk_group "Clock management"
 * - \ref ioport_group "Common IOPORT (for GPIO)"
 *




 * \page asfdoc_sam_drivers_gpiox_qsg Example
 * \subsection gpio_int_setup_code Setup Code Example
 *
 * Add this to the main loop or a setup function:
 * \snippet example/gpio_example.c config_button_0_trig_fall
 *
 * \subsection gpio_setup_workflow Basic Setup Workflow
 *
 * -# Initialize a pin to trigger an interrupt. Here, we initialize PC03 as an
 * input pin with pull up and glitch filter and to generate an interrupt on a
 * falling edge.
 * \snippet example/gpio_example.c config_button_0_trig_fall_1
 * -# Set a callback for the pin interrupt.
 * \snippet example/gpio_example.c config_button_0_trig_fall_2
 * -# Enable pin interrupt.
 * \snippet example/gpio_example.c enable_pin_interrupt
 *
 * \section gpio_int_usage Interrupt Usage
 *
 * When an interrupt happens on a pin, it will execute your callback function.
 * \snippet example/gpio_example.c example_pb0_callback
 *
 */
#endif  /* GPIO_H_INCLUDED */
