/**
 * \file
 *
 * \brief SAM Timer Counter (TC) driver.
 *
 * Copyright (c) 2011-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef TC_H_INCLUDED
#define TC_H_INCLUDED

/**
 * \defgroup asfdoc_sam_drivers_tc_group SAM3/4S/4L/4E/4N/4CM/4C/G Timer Counter (TC) Driver
 *
 * This driver for Atmel&reg; | SMART ARM&reg;-based microcontrollers 
 * provides an interface for the configuration and management of the 
 * device's Timer Counter functionality.
 *
 * The Timer Counter (TC) includes several identical 16-bit or 32-bit
 * Timer Counter channels. Each channel can be independently programmed
 * to perform a wide range of functions that includes frequency
 * measurement, event counting, interval measurement, pulse generation,
 * delay timing, and pulse width modulation.
 *
 * Devices from the following series can use this module:
 * - Atmel | SMART SAM3
 * - Atmel | SMART SAM4S
 * - Atmel | SMART SAM4L
 * - Atmel | SMART SAM4E
 * - Atmel | SMART SAM4N
 * - Atmel | SMART SAM4CM
 * - Atmel | SMART SAM4C
 * - Atmel | SMART SAMG
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_sam_drivers_tc_prerequisites
 *  - \ref asfdoc_sam_drivers_tc_module_overview
 *  - \ref asfdoc_sam_drivers_tc_special_considerations
 *  - \ref asfdoc_sam_drivers_tc_extra_info
 *  - \ref asfdoc_sam_drivers_tc_examples
 *  - \ref asfdoc_sam_drivers_tc_api_overview
 *
 *
 * \section asfdoc_sam_drivers_tc_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_sam_drivers_tc_module_overview Module Overview
 * The Timer Counter (TC) includes several identical 16-bit or 32-bit Timer
 * Counter channels. The number of TC channels is device specific, refer
 * to the device-specific datasheet for more information.
 *
 * Each channel can be independently programmed to perform a wide range of
 * functions that includes frequency measurement, event counting, interval measurement,
 * pulse generation, delay timing, and pulse width modulation.
 *
 * Each channel has three external clock inputs, five internal clock inputs,
 * and two multi-purpose input/output signals which can be configured by the user.
 * Each channel drives an internal interrupt signal which can be programmed to
 * generate processor interrupts.
 *
 * The Timer Counter (TC) embeds a quadrature decoder logic connected in front of
 * the timers. When enabled, the quadrature decoder performs the input line
 * filtering, decoding of quadrature signals and connects to the timers/counters
 * in order to read the position and speed of the motor.
 *
 *
 * \section asfdoc_sam_drivers_tc_special_considerations Special Considerations
 * \subsection asfdoc_sam_drivers_tc_special_considerations_clock External Clock
 * In all cases, if an external clock is used, the duration of each of its levels
 * must be longer than the master clock (MCLK) period. The external clock frequency
 * must be at least 2.5 times lower than the master clock.
 *
 * \subsection asfdoc_sam_drivers_tc_special_considerations_trigger External Trigger
 * If an external trigger is used, the duration of its pulses must be longer than
 * the master clock (MCLK) period in order to be detected.
 *
 *
 * \section asfdoc_sam_drivers_tc_extra_info Extra Information
 *
 * For extra information, see \ref asfdoc_sam_drivers_tc_extra. This includes:
 *  - \ref asfdoc_sam_drivers_tc_extra_acronyms
 *  - \ref asfdoc_sam_drivers_tc_extra_dependencies
 *  - \ref asfdoc_sam_drivers_tc_extra_errata
 *  - \ref asfdoc_sam_drivers_tc_extra_history
 *
 * \section asfdoc_sam_drivers_tc_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_sam_drivers_tc_exqsg.
 *
 *
 * \section asfdoc_sam_drivers_tc_api_overview API Overview
 * @{
 */

#include <compiler.h>

/// @cond
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

void tc_init(Tc *p_tc, uint32_t ul_Channel, uint32_t ul_Mode);
void tc_sync_trigger(Tc *p_tc);
void tc_set_block_mode(Tc *p_tc, uint32_t ul_blockmode);

#if (!SAM3U) || defined(__DOXYGEN__)
uint32_t tc_init_2bit_gray(Tc *p_tc, uint32_t ul_channel,
		uint32_t ul_steppermode);
#endif /* (!SAM3U) || defined(__DOXYGEN__) */

void tc_start(Tc *p_tc, uint32_t ul_channel);
void tc_stop(Tc *p_tc, uint32_t ul_channel);

uint32_t tc_read_cv(Tc *p_tc, uint32_t ul_channel);
uint32_t tc_read_ra(Tc *p_tc, uint32_t ul_channel);
uint32_t tc_read_rb(Tc *p_tc, uint32_t ul_channel);
uint32_t tc_read_rc(Tc *p_tc, uint32_t ul_channel);

void tc_write_ra(Tc *p_tc, uint32_t ul_channel,
		uint32_t ul_value);
void tc_write_rb(Tc *p_tc, uint32_t ul_channel,
		uint32_t ul_value);
void tc_write_rc(Tc *p_tc, uint32_t ul_channel,
		uint32_t ul_value);

uint32_t tc_find_mck_divisor(uint32_t ul_freq, uint32_t ul_mck,
		uint32_t *p_uldiv, uint32_t *ul_tcclks, uint32_t ul_boardmck);
void tc_enable_interrupt(Tc *p_tc, uint32_t ul_channel,
		uint32_t ul_sources);
void tc_disable_interrupt(Tc *p_tc, uint32_t ul_channel,
		uint32_t ul_sources);
uint32_t tc_get_interrupt_mask(Tc *p_tc, uint32_t ul_channel);
uint32_t tc_get_status(Tc *p_tc, uint32_t ul_channel);
#if (!SAM4L && !SAMG) || defined(__DOXYGEN__)
void tc_enable_qdec_interrupt(Tc *p_tc, uint32_t ul_sources);
void tc_disable_qdec_interrupt(Tc *p_tc, uint32_t ul_sources);
uint32_t tc_get_qdec_interrupt_mask(Tc *p_tc);
uint32_t tc_get_qdec_interrupt_status(Tc *p_tc);
#endif /* (!SAM4L && !SAMG) || defined(__DOXYGEN__) */

#if (!SAM3U)
void tc_set_writeprotect(Tc *p_tc, uint32_t ul_enable);
#endif /* (!SAM3U) */

#if SAM4L || defined(__DOXYGEN__)
uint32_t tc_get_feature(Tc *p_tc);
uint32_t tc_get_version(Tc *p_tc);
#endif /* SAM4L || defined(__DOXYGEN__) */

/// @cond
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

/** @} */

/**
 * \page asfdoc_sam_drivers_tc_extra Extra Information for Timer Counter Driver
 *
 * \section asfdoc_sam_drivers_tc_extra_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *  <tr>
 *      <th>Acronym</th>
 *      <th>Definition</th>
 *  </tr>
 *  <tr>
 *      <td>MCK</td>
 *      <td>Master Clock</td>
 * </tr>
 *  <tr>
 *      <td>PBA</td>
 *      <td>Peripheral Bus A clock</td>
 * </tr>
 *  <tr>
 *      <td>PHA</td>
 *      <td>Quadrature Decoder input signal Phase A</td>
 * </tr>
 *  <tr>
 *      <td>PHB</td>
 *      <td>Quadrature Decoder input signal Phase B</td>
 * </tr>
 *  <tr>
 *      <td>QDEC</td>
 *      <td>Quadrature Decoder</td>
 * </tr>
 *  <tr>
 *      <td>QSG</td>
 *      <td>Quick Start Guide</td>
 * </tr>
 *  <tr>
 *      <td>RA</td>
 *      <td>Register A</td>
 * </tr>
 *  <tr>
 *      <td>RB</td>
 *      <td>Register B</td>
 * </tr>
 *  <tr>
 *      <td>RC</td>
 *      <td>Register C</td>
 * </tr>
 *  <tr>
 *      <td>TIOB</td>
 *      <td>Timer Input Output B</td>
 * </tr>
 * </table>
 *
 *
 * \section asfdoc_sam_drivers_tc_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - \ref clk_group "System Clock Management (sysclock)"
 *  - \ref asfdoc_sam_drivers_gpio_group "General Purpose I/O (GPIO) driver"
 *  - \ref asfdoc_sam_drivers_pmc_group "Power Manager Controller (PMC) driver"
 *
 *
 * \section asfdoc_sam_drivers_tc_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_sam_drivers_tc_extra_history Module History
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
 */
 
/**
 * \page asfdoc_sam_drivers_tc_exqsg Examples for Timer Counter
 *
 * This is a list of the available Quick Start Guides (QSGs) and example
 * applications for \ref asfdoc_sam_drivers_tc_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that a QSG can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_sam_drivers_tc_qsg
 *  - \subpage asfdoc_sam_drivers_tc_example
 *
 * \page asfdoc_sam_drivers_tc_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Doc. Rev.</td>
 *		<th>Date</td>
 *		<th>Comments</td>
 *	</tr>
 *	<tr>
 *		<td>42301B</td>
 *		<td>07/2015</td>
 *		<td>Updated title of application note and added list of supported devices</td>
 *	</tr>
 *	<tr>
 *		<td>42301A</td>
 *		<td>05/2014</td>
 *		<td>Initial document release</td>
 *	</tr>
 * </table>
 *
 */
 
/**
 * \page asfdoc_sam_drivers_tc_qsg Quick Start Guide for the TC driver
 *
 * This is the quick start guide for the \ref asfdoc_sam_drivers_tc_group, with
 * step-by-step instructions on how to configure and use the driver for
 * a specific use case. The code examples can be copied into the main 
 * application loop or any other function that will need to control the 
 * AST module.
 *
 * \section asfdoc_sam_drivers_tc_qsg_use_cases Use Cases
 * - \ref asfdoc_sam_drivers_tc_qsg_basic_capture
 * - \ref asfdoc_sam_drivers_tc_qsg_basic_waveform
 *
 * \section asfdoc_sam_drivers_tc_qsg_basic_capture TC Capture Mode Basic Usage
 *
 * This use case will demonstrate how to initialize the TC module to
 * operate in capture mode using interrupts. Note, that the macros
 * used to select the TC channel are device specific. Refer to the
 * appropriate device-specific datasheet for more information.
 *
 *
 * \section asfdoc_sam_drivers_tc_qsg_basic_capture_setup Setup Steps
 *
 * \subsection asfdoc_sam_tc_qsg_basic_capture_prereq Prerequisites
 *
 * This module requires the following services:
 * - \ref clk_group "System Clock Management (sysclock)"
 * - \ref asfdoc_sam_drivers_gpio_group "General Purpose I/O (GPIO) driver"
 *
 * \subsection asfdoc_sam_drivers_tc_qsg_basic_capture_setup_code Setup Code
 *
 * Add these macros to the top of your main application C-file:
 * \snippet conf_board.h tc_define_peripheral
 * \snippet conf_board.h tc_define_ch2
 * \snippet conf_board.h tc_define_irq_handler
 *
 * Add this macro and functions to your main application C-file:
 * \snippet tc_capture_waveform_example.c tc_capture_selection
 * \snippet tc_capture_waveform_example.c tc_capture_init
 * \snippet tc_capture_waveform_example.c tc_capture_irq_handler_start
 * \snippet tc_capture_waveform_example.c tc_capture_irq_handler_end
 *
 * \subsection asfdoc_sam_drivers_tc_qsg_basic_capture_setup_workflow Workflow
 *
 * -# Enable the TC module's capture pin:
 * \snippet tc_capture_waveform_example.c tc_capture_gpio
 * -# Initialize the capture channel to the following:
 *   - Load RA on the rising edge of TIOA
 *   - Load RB on the falling edge of TIOA
 *   - Set the external trigger to TIOA
 *   - Set the external trigger to falling edge
 * \snippet tc_capture_waveform_example.c tc_capture_init_call
 * -# Enable the TC interrupt using NVIC:
 * \snippet tc_capture_waveform_example.c tc_capture_init_irq
 * -# Enable the capture channel interrupt:
 * \snippet tc_capture_waveform_example.c tc_capture_init_module_irq
 * -# In the \ref TC_Handler_null "TC_Handler()" function, the load.
 * RB interrupt can be checked by:
 * \snippet tc_capture_waveform_example.c tc_capture_irq_handler_status
 * \code } \endcode
 * -# In the \ref TC_Handler_null "TC_Handler()" function, the RA value.
 * can be read by:
 * \code uint32_t gs_ul_captured_ra; \endcode
 * \snippet tc_capture_waveform_example.c tc_capture_irq_handler_read_ra
 * -# In the \ref TC_Handler_null "TC_Handler()" function, the RB value.
 * can be read by:
 * \code uint32_t gs_ul_captured_rb; \endcode
 * \snippet tc_capture_waveform_example.c tc_capture_irq_handler_read_rb
 *
 * \section asfdoc_sam_drivers_tc_qsg_basic_waveform TC Waveform Mode Basic Usage
 *
 * This use case will demonstrate how to initialize the TC module to
 * operate in waveform mode. Note, that the macros used to select
 * the TC channel are device specific. Refer to the appropriate
 * device-specific datasheet for more information.
 *
 *
 * \section asfdoc_sam_drivers_tc_qsg_basic_waveform_setup Setup Steps
 *
 * \subsection asfdoc_sam_tc_qsg_basic_waveform_prereq Prerequisites
 *
 * This module requires the following services:
 * - \ref clk_group "System Clock Management (sysclock)"
 * - \ref asfdoc_sam_drivers_gpio_group "General Purpose I/O (GPIO) driver"
 *
 * \subsection asfdoc_sam_drivers_tc_qsg_basic_waveform_setup_code Setup Code
 *
 * Add these macros to the top of your main application C-file:
 * \snippet conf_board.h tc_define_peripheral
 * \snippet conf_board.h tc_define_ch1
 *
 * Add these macros and function to your main application C-file:
 * \code #define TC_WAVEFORM_TIMER_SELECTION TC_CMR_TCCLKS_TIMER_CLOCK4 \endcode
 * \code #define TC_WAVEFORM_DIVISOR     128 \endcode
 * \code #define TC_WAVEFORM_FREQUENCY   178 \endcode
 * \code #define TC_WAVEFORM_DUTY_CYCLE  30 \endcode
 * \code
 * static void tc_waveform_initialize(void)
 * {
 *	uint32_t ra, rc;
 *
 *	// Configure the PMC to enable the TC module.
 *	sysclk_enable_peripheral_clock(ID_TC_WAVEFORM);
 *
 *	// Init TC to waveform mode.
 *	tc_init(TC, TC_CHANNEL_WAVEFORM,
 *			TC_WAVEFORM_TIMER_SELECTION // Waveform Clock Selection
 *			| TC_CMR_WAVE       // Waveform mode is enabled
 *			| TC_CMR_ACPA_SET   // RA Compare Effect: set
 *			| TC_CMR_ACPC_CLEAR // RC Compare Effect: clear
 *			| TC_CMR_CPCTRG     // UP mode with automatic trigger on RC Compare
 *	);
 *
 *	// Configure waveform frequency and duty cycle.
 *	rc = (sysclk_get_peripheral_bus_hz(TC) /
 *			TC_WAVEFORM_DIVISOR /
 *			TC_WAVEFORM_FREQUENCY;
 *	tc_write_rc(TC, TC_CHANNEL_WAVEFORM, rc);
 *	ra = (100 - TC_WAVEFORM_FREQUENCY_DUTY_CYCLE * rc / 100;
 *	tc_write_ra(TC, TC_CHANNEL_WAVEFORM, ra);
 *
 *	// Enable TC TC_CHANNEL_WAVEFORM.
 *	tc_start(TC, TC_CHANNEL_WAVEFORM);
 * } \endcode
 *
 * \subsection asfdoc_sam_drivers_tc_qsg_basic_waveform_setup_workflow Workflow
 *
 * -# Enable the TC module's waveform pin:
 * \snippet tc_capture_waveform_example.c tc_waveform_gpio
 * -# Initialize the waveform channel to the following:
 *   - Output frequency of 178Hz, with a duty-cycle of 30%
 *   - Use TC_CMR_TCCLKS_TIMER_CLOCK4, with a divisor of 128
 * \snippet tc_capture_waveform_example.c tc_waveform_init_call
 */
#endif /* TC_H_INCLUDED */
