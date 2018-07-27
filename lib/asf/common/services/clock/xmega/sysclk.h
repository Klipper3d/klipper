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
#ifndef XMEGA_SYSCLK_H_INCLUDED
#define XMEGA_SYSCLK_H_INCLUDED

#include <board.h>
#include <compiler.h>
#include <parts.h>
#include <ccp.h>
#include <osc.h>
#include <pll.h>

// Include clock configuration for the project.
#include <conf_clock.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \page sysclk_quickstart Quick Start Guide for the System Clock Management service (XMEGA)
 *
 * This is the quick start guide for the \ref sysclk_group "System Clock Management"
 * service, with step-by-step instructions on how to configure and use the service for
 * specific use cases.
 *
 * \section sysclk_quickstart_usecases System Clock Management use cases
 * - \ref sysclk_quickstart_basic
 * - \ref sysclk_quickstart_use_case_2
 * - \ref sysclk_quickstart_use_case_3
 *
 * \section sysclk_quickstart_basic Basic usage of the System Clock Management service
 * This section will present a basic use case for the System Clock Management service.
 * This use case will configure the main system clock to 32MHz, using an internal PLL
 * module to multiply the frequency of a crystal attached to the microcontroller. The
 * secondary peripheral bus clock and CPU clock are scaled down from the speed of the
 * main system clock.
 *
 * \subsection sysclk_quickstart_use_case_1_prereq Prerequisites
 *  - None
 *
 * \subsection sysclk_quickstart_use_case_1_setup_steps Initialization code
 * Add to the application initialization code:
 * \code
	sysclk_init();
\endcode
 *
 * \subsection sysclk_quickstart_use_case_1_setup_steps_workflow Workflow
 * -# Configure the system clocks according to the settings in conf_clock.h:
 *    \code sysclk_init(); \endcode
 *
 * \subsection sysclk_quickstart_use_case_1_example_code Example code
 *   Add or uncomment the following in your conf_clock.h header file, commenting out all other
 *   definitions of the same symbol(s):
 *   \code
	   #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLL

	   // Fpll0 = (Fclk * PLL_mul) / PLL_div
	   #define CONFIG_PLL0_SOURCE          PLL_SRC_XOSC
	   #define CONFIG_PLL0_MUL             (32000000UL / BOARD_XOSC_HZ)
	   #define CONFIG_PLL0_DIV             1

	   // Fbus = Fsys / (2 ^ BUS_div)
	   #define CONFIG_SYSCLK_PSADIV        SYSCLK_PSADIV_1
	   #define CONFIG_SYSCLK_PSBCDIV       SYSCLK_PSBCDIV_1_2
\endcode
 *
 * \subsection sysclk_quickstart_use_case_1_example_workflow Workflow
 *  -# Configure the main system clock to use the output of the PLL module as its source:
 *   \code #define CONFIG_SYSCLK_SOURCE          SYSCLK_SRC_PLL \endcode
 *  -# Configure the PLL0 module to use external crystal oscillator XOSC as its source:
 *   \code #define CONFIG_PLL0_SOURCE            PLL_SRC_XOSC \endcode
 *  -# Configure the PLL0 module to multiply the external oscillator XOSC frequency up to 32MHz:
 *   \code
	#define CONFIG_PLL0_MUL             (32000000UL / BOARD_XOSC_HZ)
	#define CONFIG_PLL0_DIV             1
\endcode
 *   \note For user boards, \c BOARD_XOSC_HZ should be defined in the board \c conf_board.h configuration
 *         file as the frequency of the crystal attached to XOSC.
 *  -# Configure the main CPU clock and slow peripheral bus to run at 16MHz, run the fast peripheral bus
 *     at the full 32MHz speed:
 *    \code
	#define CONFIG_SYSCLK_PSADIV       SYSCLK_PSADIV_1
	#define CONFIG_SYSCLK_PSBCDIV      SYSCLK_PSBCDIV_1_2
\endcode
 *    \note Some dividers are powers of two, while others are integer division factors. Refer to the
 *          formulas in the conf_clock.h template commented above each division define.
 */

/**
 * \page sysclk_quickstart_use_case_2 Advanced use case - Peripheral Bus Clock Management (XMEGA)
 *
 * \section sysclk_quickstart_use_case_2 Advanced use case - Peripheral Bus Clock Management
 * This section will present a more advanced use case for the System Clock Management service.
 * This use case will configure the main system clock to 32MHz, using an internal PLL
 * module to multiply the frequency of a crystal attached to the microcontroller. The peripheral bus
 * clocks will run at the same speed as the CPU clock, and the USB clock will be configured to use
 * the internal 32MHz (nominal) RC oscillator calibrated to 48MHz with the USB Start-of-Frame as the
 * calibration reference.
 *
 * \subsection sysclk_quickstart_use_case_2_prereq Prerequisites
 *  - None
 *
 * \subsection sysclk_quickstart_use_case_2_setup_steps Initialization code
 * Add to the application initialization code:
 * \code
	sysclk_init();
\endcode
 *
 * \subsection sysclk_quickstart_use_case_2_setup_steps_workflow Workflow
 * -# Configure the system clocks according to the settings in conf_clock.h:
 *    \code sysclk_init(); \endcode
 *
 * \subsection sysclk_quickstart_use_case_2_example_code Example code
 *   Add or uncomment the following in your conf_clock.h header file, commenting out all other
 *   definitions of the same symbol(s):
 *   \code
	   #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLL

	   // Fpll0 = (Fclk * PLL_mul) / PLL_div
	   #define CONFIG_PLL0_SOURCE          PLL_SRC_XOSC
	   #define CONFIG_PLL0_MUL             (32000000UL / BOARD_XOSC_HZ)
	   #define CONFIG_PLL0_DIV             1

	   // Fbus = Fsys / (2 ^ BUS_div)
	   #define CONFIG_SYSCLK_PSADIV        SYSCLK_PSADIV_1
	   #define CONFIG_SYSCLK_PSBCDIV       SYSCLK_PSBCDIV_1_1

	   #define CONFIG_USBCLK_SOURCE        USBCLK_SRC_RCOSC
	   #define CONFIG_OSC_RC32_CAL         48000000UL
	   #define CONFIG_OSC_AUTOCAL          OSC_ID_RC32MHZ
	   #define CONFIG_OSC_AUTOCAL_REF_OSC  OSC_ID_USBSOF
\endcode
 *
 * \subsection sysclk_quickstart_use_case_2_example_workflow Workflow
 *  -# Configure the main system clock to use the output of the PLL module as its source:
 *   \code #define CONFIG_SYSCLK_SOURCE           SYSCLK_SRC_PLL \endcode
 *  -# Configure the PLL0 module to use external crystal oscillator XOSC as its source:
 *   \code #define CONFIG_PLL0_SOURCE             PLL_SRC_XOSC \endcode
 *  -# Configure the PLL0 module to multiply the external oscillator XOSC frequency up to 32MHz:
 *   \code
	#define CONFIG_PLL0_MUL              (32000000UL / BOARD_XOSC_HZ)
	#define CONFIG_PLL0_DIV              1
\endcode
 *   \note For user boards, \c BOARD_XOSC_HZ should be defined in the board \c conf_board.h configuration
 *         file as the frequency of the crystal attached to XOSC.
 *  -# Configure the main CPU and peripheral bus clocks to run at 32MHz:
 *    \code
	#define CONFIG_SYSCLK_PSADIV        SYSCLK_PSADIV_1
	#define CONFIG_SYSCLK_PSBCDIV       SYSCLK_PSBCDIV_1_2
\endcode
 *    \note Some dividers are powers of two, while others are integer division factors. Refer to the
 *          formulas in the conf_clock.h template commented above each division define.
 *  -# Configure the USB module clock to use the internal fast (32MHz) RC oscillator:
 *    \code
	#define CONFIG_USBCLK_SOURCE        USBCLK_SRC_RCOSC
\endcode
 *    \note When the internal RC oscillator is used for the USB module, it must be recalibrated to 48MHz for
 *          the USB peripheral to function. If this oscillator is then used as the main system clock source,
 *          the clock must be divided down via the peripheral and CPU bus clock division constants to ensure
 *          that the maximum allowable CPU frequency is not exceeded.
 *  -# Configure the internal fast (32MHz) RC oscillator to calibrate to 48MHz using the USB Start of Frame (SOF)
 *     as the calibration reference:
 *    \code
	#define CONFIG_OSC_RC32_CAL         48000000UL
	#define CONFIG_OSC_AUTOCAL          OSC_ID_RC32MHZ
	#define CONFIG_OSC_AUTOCAL_REF_OSC  OSC_ID_USBSOF
\endcode
 */

/**
 * \page sysclk_quickstart_use_case_3 Advanced use case - DFLL auto-calibration (XMEGA)
 *
 * \section sysclk_quickstart_use_case_3 Advanced use case - DFLL auto-calibration
 * This section will present a more advanced use case for the System Clock
 * Management service.  This use case will configure the main system clock to
 * 2MHz, using the internal 2MHz RC oscillator calibrated against the internal
 * 32KHz oscillator. The peripheral bus clocks will run at the same speed as
 * the CPU clock, and the USB clock will be configured to use the internal
 * 32MHz (nominal) RC oscillator calibrated to 48MHz with the USB
 * Start-of-Frame as the calibration reference.
 *
 * \subsection sysclk_quickstart_use_case_3_prereq Prerequisites
 *  - None
 *
 * \subsection sysclk_quickstart_use_case_3_setup_steps Initialization code
 * Add to the application initialization code:
 * \code
	sysclk_init();
\endcode
 *
 * \subsection sysclk_quickstart_use_case_3_setup_steps_workflow Workflow
 * -# Configure the system clocks according to the settings in conf_clock.h:
 *    \code sysclk_init(); \endcode
 *
 * \subsection sysclk_quickstart_use_case_3_example_code Example code
 *   Add or uncomment the following in your conf_clock.h header file,
 *   commenting out all other definitions of the same symbol(s):
 *   \code
	   #define CONFIG_SYSCLK_SOURCE                SYSCLK_SRC_RC2MHZ

	   #define CONFIG_OSC_AUTOCAL_RC2MHZ_REF_OSC   OSC_ID_RC32KHZ

	   #define CONFIG_USBCLK_SOURCE                USBCLK_SRC_RCOSC
	   #define CONFIG_OSC_RC32_CAL                 48000000UL
	   #define CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC  OSC_ID_USBSOF
\endcode
 *
 * \subsection sysclk_quickstart_use_case_3_example_workflow Workflow
 *  -# Configure the main system clock to use the internal 2MHz RC oscillator
 *   as its source:
 *   \code
	#define CONFIG_SYSCLK_SOURCE                SYSCLK_SRC_RC2MHZ
\endcode
 *  -# Configure the 2MHz DFLL auto-calibration to use the internal 32KHz RC
 *   oscillator:
 *   \code
	#define CONFIG_OSC_AUTOCAL_RC2MHZ_REF_OSC   OSC_ID_RC32KHZ
\endcode
 *   \note For auto-calibration it's typically more relevant to use an external
 *        32KHz crystal. So if that's the case use OSC_ID_XOSC instead.
 *  -# Configure the USB module clock to use the internal fast (32MHz) RC oscillator:
 *   \code
	#define CONFIG_USBCLK_SOURCE                USBCLK_SRC_RCOSC
\endcode
 *  -# Configure the internal fast (32MHz) RC oscillator to calibrate to 48MHz
 *   using the USB Start of Frame (SOF) as the calibration reference:
 *   \code
	#define CONFIG_USBCLK_SOURCE                USBCLK_SRC_RCOSC
	#define CONFIG_OSC_RC32_CAL                 48000000UL
	#define CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC  OSC_ID_USBSOF
\endcode
 */

/* Wrap old config into new one */
#ifdef CONFIG_OSC_AUTOCAL
# if CONFIG_OSC_AUTOCAL == OSC_ID_RC2MHZ
#  define CONFIG_OSC_AUTOCAL_RC2MHZ_REF_OSC CONFIG_OSC_AUTOCAL_REF_OSC
# elif CONFIG_OSC_AUTOCAL == OSC_ID_RC32MHZ
#  define CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC CONFIG_OSC_AUTOCAL_REF_OSC
# else
#  error Bad configuration of CONFIG_OSC_AUTOCAL and/or CONFIG_OSC_AUTOCAL_REF_OSC
# endif
#endif

// Use 2 MHz with no prescaling if config was empty.
#ifndef CONFIG_SYSCLK_SOURCE
# define CONFIG_SYSCLK_SOURCE    SYSCLK_SRC_RC2MHZ
#endif /* CONFIG_SYSCLK_SOURCE */

#ifndef CONFIG_SYSCLK_PSADIV
# define CONFIG_SYSCLK_PSADIV    SYSCLK_PSADIV_1
#endif /* CONFIG_SYSCLK_PSADIV */

#ifndef CONFIG_SYSCLK_PSBCDIV
# define CONFIG_SYSCLK_PSBCDIV   SYSCLK_PSBCDIV_1_1
#endif /* CONFIG_SYSCLK_PSBCDIV */

/**
 * \weakgroup sysclk_group
 *
 * \section sysclk_group_config Configuration Symbols
 *
 * The following configuration symbols may be used to specify the
 * initial system clock configuration. If any of the symbols are not
 * set, reasonable defaults will be provided.
 *   - \b CONFIG_SYSCLK_SOURCE: The initial system clock source.
 *   - \b CONFIG_SYSCLK_PSADIV: The initial Prescaler A setting.
 *   - \b CONFIG_SYSCLK_PSBCDIV: The initial Prescaler B setting.
 *   - \b CONFIG_USBCLK_SOURCE: The initial USB clock source.
 *
 * @{
 */

//! \name System Clock Sources
//@{
//! Internal 2 MHz RC oscillator
#define SYSCLK_SRC_RC2MHZ    CLK_SCLKSEL_RC2M_gc
//! Internal 32 MHz RC oscillator
#define SYSCLK_SRC_RC32MHZ   CLK_SCLKSEL_RC32M_gc
//! Internal 32 KHz RC oscillator
#define SYSCLK_SRC_RC32KHZ   CLK_SCLKSEL_RC32K_gc
//! External oscillator
#define SYSCLK_SRC_XOSC      CLK_SCLKSEL_XOSC_gc
//! Phase-Locked Loop
#define SYSCLK_SRC_PLL       CLK_SCLKSEL_PLL_gc
#if XMEGA_E
//! Internal 8 MHz RC oscillator
# define SYSCLK_SRC_RC8MHZ   CLK_SCLKSEL_RC8M_gc
#endif
//@}

//! \name Prescaler A Setting (relative to CLKsys)
//@{
#define SYSCLK_PSADIV_1      CLK_PSADIV_1_gc      //!< Do not prescale
#define SYSCLK_PSADIV_2      CLK_PSADIV_2_gc      //!< Prescale CLKper4 by 2
#define SYSCLK_PSADIV_4      CLK_PSADIV_4_gc      //!< Prescale CLKper4 by 4
#define SYSCLK_PSADIV_8      CLK_PSADIV_8_gc      //!< Prescale CLKper4 by 8
#define SYSCLK_PSADIV_16     CLK_PSADIV_16_gc     //!< Prescale CLKper4 by 16
#define SYSCLK_PSADIV_32     CLK_PSADIV_32_gc     //!< Prescale CLKper4 by 32
#define SYSCLK_PSADIV_64     CLK_PSADIV_64_gc     //!< Prescale CLKper4 by 64
#define SYSCLK_PSADIV_128    CLK_PSADIV_128_gc    //!< Prescale CLKper4 by 128
#define SYSCLK_PSADIV_256    CLK_PSADIV_256_gc    //!< Prescale CLKper4 by 256
#define SYSCLK_PSADIV_512    CLK_PSADIV_512_gc    //!< Prescale CLKper4 by 512

#if XMEGA_E
# define SYSCLK_PSADIV_6      CLK_PSADIV_6_gc      //!< Prescale CLKper4 by 6
# define SYSCLK_PSADIV_10     CLK_PSADIV_10_gc     //!< Prescale CLKper4 by 10
# define SYSCLK_PSADIV_12     CLK_PSADIV_12_gc     //!< Prescale CLKper4 by 12
# define SYSCLK_PSADIV_24     CLK_PSADIV_24_gc     //!< Prescale CLKper4 by 24
# define SYSCLK_PSADIV_48     CLK_PSADIV_48_gc     //!< Prescale CLKper4 by 48
#endif
//@}

//! \name Prescaler B and C Setting (relative to CLKper4)
//@{
//! Do not prescale
#define SYSCLK_PSBCDIV_1_1   CLK_PSBCDIV_1_1_gc
//! Prescale CLKper and CLKcpu by 2
#define SYSCLK_PSBCDIV_1_2   CLK_PSBCDIV_1_2_gc
//! Prescale CLKper2, CLKper and CLKcpu by 4
#define SYSCLK_PSBCDIV_4_1   CLK_PSBCDIV_4_1_gc
//! Prescale CLKper2 by 2, CLKper and CLKcpu by 4
#define SYSCLK_PSBCDIV_2_2   CLK_PSBCDIV_2_2_gc
//@}

//! \name System Clock Port Numbers
enum sysclk_port_id {
	SYSCLK_PORT_GEN,   //!< Devices not associated with a specific port.
	SYSCLK_PORT_A,     //!< Devices on PORTA
	SYSCLK_PORT_B,     //!< Devices on PORTB
	SYSCLK_PORT_C,     //!< Devices on PORTC
	SYSCLK_PORT_D,     //!< Devices on PORTD
	SYSCLK_PORT_E,     //!< Devices on PORTE
	SYSCLK_PORT_F,     //!< Devices on PORTF
};

/*! \name Clocks not associated with any port
 *
 * \note See the datasheet for available modules in the device.
 */
//@{
#define SYSCLK_DMA        PR_DMA_bm     //!< DMA Controller
#define SYSCLK_EDMA       PR_EDMA_bm    //!< EDMA Controller
#define SYSCLK_EVSYS      PR_EVSYS_bm   //!< Event System
#define SYSCLK_RTC        PR_RTC_bm     //!< Real-Time Counter
#define SYSCLK_EBI        PR_EBI_bm     //!< Ext Bus Interface
#define SYSCLK_AES        PR_AES_bm     //!< AES Module
#define SYSCLK_USB        PR_USB_bm     //!< USB Module
#define SYSCLK_XCL        PR_XCL_bm     //!< USB Module
//@}

/*! \name Clocks on PORTA and PORTB
 *
 * \note See the datasheet for available modules in the device.
 */
//@{
#define SYSCLK_AC         PR_AC_bm      //!< Analog Comparator
#define SYSCLK_ADC        PR_ADC_bm     //!< A/D Converter
#define SYSCLK_DAC        PR_DAC_bm     //!< D/A Converter
//@}

/*! \name Clocks on PORTC, PORTD, PORTE and PORTF
 *
 * \note See the datasheet for available modules in the device.
 */
//@{
#define SYSCLK_TC0        PR_TC0_bm      //!< Timer/Counter 0
#define SYSCLK_TC1        PR_TC1_bm      //!< Timer/Counter 1
#define SYSCLK_TC4        PR_TC4_bm      //!< Timer/Counter 0
#define SYSCLK_TC5        PR_TC5_bm      //!< Timer/Counter 1
#define SYSCLK_HIRES      PR_HIRES_bm    //!< Hi-Res Extension
#define SYSCLK_SPI        PR_SPI_bm      //!< SPI controller
#define SYSCLK_USART0     PR_USART0_bm   //!< USART 0
#define SYSCLK_USART1     PR_USART1_bm   //!< USART 1
#define SYSCLK_TWI        PR_TWI_bm      //!< TWI controller
//@}

/**
 * \name RTC clock source identifiers
 *
 * @{
 */

/** 1kHz from internal ULP oscillator. Low precision */
#define SYSCLK_RTCSRC_ULP     CLK_RTCSRC_ULP_gc
/** 1.024kHz from 32.768kHz crystal oscillator TOSC */
#define SYSCLK_RTCSRC_TOSC    CLK_RTCSRC_TOSC_gc
/** 1.024kHz from 32.768kHz internal RC oscillator */
#define SYSCLK_RTCSRC_RCOSC   CLK_RTCSRC_RCOSC_gc
/** 32.768kHz from crystal oscillator TOSC */
#define SYSCLK_RTCSRC_TOSC32  CLK_RTCSRC_TOSC32_gc
/** 32.768kHz from internal RC oscillator */
#define SYSCLK_RTCSRC_RCOSC32 CLK_RTCSRC_RCOSC32_gc
/** External clock on TOSC1 */
#define SYSCLK_RTCSRC_EXTCLK  CLK_RTCSRC_EXTCLK_gc

/** @} */

#if XMEGA_AU || XMEGA_B || XMEGA_C
//! \name USB Clock Sources
//@{
//! Internal 32 MHz RC oscillator
#define USBCLK_SRC_RCOSC    0
//! Phase-Locked Loop
#define USBCLK_SRC_PLL      1
//@}

/**
 * \def CONFIG_USBCLK_SOURCE
 * \brief Configuration symbol for the USB clock source
 *
 * If the device features an USB module, and this is intended to be used, this
 * symbol must be defined with the clock source configuration.
 *
 * Define this as one of the \c USBCLK_SRC_xxx definitions. If the PLL is
 * selected, it must be configured to run at 48 MHz. If the 32 MHz RC oscillator
 * is selected, it must be tuned to 48 MHz by means of the DFLL.
 */
#ifdef __DOXYGEN__
# define CONFIG_USBCLK_SOURCE
#endif

#endif // XMEGA_AU || XMEGA_B || XMEGA_C

#ifndef __ASSEMBLY__

/**
 * \name Querying the system clock and its derived clocks
 */
//@{

/**
 * \brief Return the current rate in Hz of the main system clock
 *
 * \todo This function assumes that the main clock source never changes
 * once it's been set up, and that PLL0 always runs at the compile-time
 * configured default rate. While this is probably the most common
 * configuration, which we want to support as a special case for
 * performance reasons, we will at some point need to support more
 * dynamic setups as well.
 *
 * \return Frequency of the main system clock, in Hz.
 */
static inline uint32_t sysclk_get_main_hz(void)
{
	switch (CONFIG_SYSCLK_SOURCE) {
	case SYSCLK_SRC_RC2MHZ:
		return 2000000UL;
#if XMEGA_E
	case SYSCLK_SRC_RC8MHZ:
		return 8000000UL;
#endif
	case SYSCLK_SRC_RC32MHZ:
#ifdef CONFIG_OSC_RC32_CAL
		return CONFIG_OSC_RC32_CAL;
#else
		return 32000000UL;
#endif

	case SYSCLK_SRC_RC32KHZ:
		return 32768UL;

#ifdef BOARD_XOSC_HZ
	case SYSCLK_SRC_XOSC:
		return BOARD_XOSC_HZ;
#endif

#ifdef CONFIG_PLL0_SOURCE
	case SYSCLK_SRC_PLL:
		return pll_get_default_rate(0);
#endif

	default:
		//unhandled_case(CONFIG_SYSCLK_SOURCE);
		return 0;
	}
}

/**
 * \brief Return the current rate in Hz of clk_PER4.
 *
 * This clock can run up to four times faster than the CPU clock.
 *
 * \return Frequency of the clk_PER4 clock, in Hz.
 */
static inline uint32_t sysclk_get_per4_hz(void)
{
	uint8_t shift = 0;

#if XMEGA_E
	if (CONFIG_SYSCLK_PSADIV > SYSCLK_PSADIV_512) {
		switch (CONFIG_SYSCLK_PSADIV) {
			case SYSCLK_PSADIV_6:
				return sysclk_get_main_hz() / 6;
			case SYSCLK_PSADIV_10:
				return sysclk_get_main_hz() / 10;
			case SYSCLK_PSADIV_12:
				return sysclk_get_main_hz() / 12;
			case SYSCLK_PSADIV_24:
				return sysclk_get_main_hz() / 24;
			case SYSCLK_PSADIV_48:
				return sysclk_get_main_hz() / 48;
			default:
				//unhandled_case;
				return 0;
		}
	}
#endif
	if (CONFIG_SYSCLK_PSADIV & (1U << CLK_PSADIV_gp)) {
		shift = (CONFIG_SYSCLK_PSADIV >> (1 + CLK_PSADIV_gp)) + 1;
	}

	return sysclk_get_main_hz() >> shift;
}

/**
 * \brief Return the current rate in Hz of clk_PER2.
 *
 * This clock can run up to two times faster than the CPU clock.
 *
 * \return Frequency of the clk_PER2 clock, in Hz.
 */
static inline uint32_t sysclk_get_per2_hz(void)
{
	switch (CONFIG_SYSCLK_PSBCDIV) {
	case SYSCLK_PSBCDIV_1_1: /* Fall through */
	case SYSCLK_PSBCDIV_1_2:
		return sysclk_get_per4_hz();

	case SYSCLK_PSBCDIV_4_1:
		return sysclk_get_per4_hz() / 4;

	case SYSCLK_PSBCDIV_2_2:
		return sysclk_get_per4_hz() / 2;

	default:
		//unhandled_case(CONFIG_SYSCLK_PSBCDIV);
		return 0;
	}
}

/**
 * \brief Return the current rate in Hz of clk_PER.
 *
 * This clock always runs at the same rate as the CPU clock unless the divider
 * is set.
 *
 * \return Frequency of the clk_PER clock, in Hz.
 */
static inline uint32_t sysclk_get_per_hz(void)
{
	if (CONFIG_SYSCLK_PSBCDIV & (1U << CLK_PSBCDIV_gp))
		return sysclk_get_per2_hz() / 2;
	else
		return sysclk_get_per2_hz();
}

/**
 * \brief Return the current rate in Hz of the CPU clock.
 *
 * \return Frequency of the CPU clock, in Hz.
 */
static inline uint32_t sysclk_get_cpu_hz(void)
{
	return sysclk_get_per_hz();
}

/**
 * \brief Retrieves the current rate in Hz of the Peripheral Bus clock attached
 *        to the specified peripheral.
 *
 * \param module Pointer to the module's base address.
 *
 * \return Frequency of the bus attached to the specified peripheral, in Hz.
 */
static inline uint32_t sysclk_get_peripheral_bus_hz(const volatile void *module)
{
	if (module == NULL) {
		Assert(false);
		return 0;
	}
#ifdef AES
	else if (module == &AES) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef EBI
	else if (module == &EBI) {
		return sysclk_get_per2_hz();
	}
#endif
#ifdef RTC
	else if (module == &RTC) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef EVSYS
	else if (module == &EVSYS) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef DMA
	else if (module == &DMA) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef EDMA
	else if (module == &EDMA) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef ACA
	else if (module == &ACA) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef ACB
	else if (module == &ACB) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef ADCA
	else if (module == &ADCA) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef ADCB
	else if (module == &ADCB) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef DACA
	else if (module == &DACA) {
		return sysclk_get_per_hz();
	}
#endif
// Workaround for bad XMEGA D header file
#if !XMEGA_D
#ifdef DACB
	else if (module == &DACB) {
		return sysclk_get_per_hz();
	}
#endif
#endif // Workaround end
#ifdef FAULTC0
	else if (module == &FAULTC0) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef FAULTC1
	else if (module == &FAULTC1) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TCC0
	else if (module == &TCC0) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TCD0
	else if (module == &TCD0) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TCE0
	else if (module == &TCE0) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TCF0
	else if (module == &TCF0) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TCC1
	else if (module == &TCC1) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TCD1
	else if (module == &TCD1) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TCE1
	else if (module == &TCE1) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TCF1
	else if (module == &TCF1) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TCC4
	else if (module == &TCC4) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TCC5
	else if (module == &TCC5) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TCD4
	else if (module == &TCD4) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TCD5
	else if (module == &TCD5) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef HIRESC
	else if (module == &HIRESC) {
		return sysclk_get_per4_hz();
	}
#endif
#ifdef HIRESD
	else if (module == &HIRESD) {
		return sysclk_get_per4_hz();
	}
#endif
#ifdef HIRESE
	else if (module == &HIRESE) {
		return sysclk_get_per4_hz();
	}
#endif
#ifdef HIRESF
	else if (module == &HIRESF) {
		return sysclk_get_per4_hz();
	}
#endif
#ifdef SPIC
	else if (module == &SPIC) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef SPID
	else if (module == &SPID) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef SPIE
	else if (module == &SPIE) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef SPIF
	else if (module == &SPIF) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef USARTC0
	else if (module == &USARTC0) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef USARTD0
	else if (module == &USARTD0) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef USARTE0
	else if (module == &USARTE0) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef USARTF0
	else if (module == &USARTF0) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef USARTC1
	else if (module == &USARTC1) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef USARTD1
	else if (module == &USARTD1) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef USARTE1
	else if (module == &USARTE1) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef USARTF1
	else if (module == &USARTF1) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TWIC
	else if (module == &TWIC) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TWID
	else if (module == &TWID) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TWIE
	else if (module == &TWIE) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef TWIF
	else if (module == &TWIF) {
		return sysclk_get_per_hz();
	}
#endif
#ifdef XCL
	else if (module == &XCL) {
		return sysclk_get_per_hz();
	}
#endif
	else {
		Assert(false);
		return 0;
	}
}

//@}

//! \name Enabling and disabling synchronous clocks
//@{

/**
 * \brief Enable the clock to peripheral \a id on port \a port
 *
 * \param port ID of the port to which the module is connected (one of
 * the \c SYSCLK_PORT_* definitions).
 * \param id The ID (bitmask) of the peripheral module to be enabled.
 */
extern void sysclk_enable_module(enum sysclk_port_id port, uint8_t id);

/**
 * \brief Disable the clock to peripheral \a id on port \a port
 *
 * \param port ID of the port to which the module is connected (one of
 * the \c SYSCLK_PORT_* definitions).
 * \param id The ID (bitmask) of the peripheral module to be disabled.
 */
extern void sysclk_disable_module(enum sysclk_port_id port, uint8_t id);

/**
 * \brief Enable a peripheral's clock from its base address.
 *
 *  Enables the clock to a peripheral, given its base address. If the peripheral
 *  has an associated clock on the HSB bus, this will be enabled also.
 *
 * \param module Pointer to the module's base address.
 */
static inline void sysclk_enable_peripheral_clock(const volatile void *module)
{
	if (module == NULL) {
		Assert(false);
	}
#ifdef AES
	else if (module == &AES) {
		sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_AES);
	}
#endif
#ifdef EBI
	else if (module == &EBI) {
		sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_EBI);
	}
#endif
#ifdef RTC
	else if (module == &RTC) {
		sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_RTC);
	}
#endif
#ifdef EVSYS
	else if (module == &EVSYS) {
		sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_EVSYS);
	}
#endif
#ifdef DMA
	else if (module == &DMA) {
		sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_DMA);
	}
#endif
#ifdef EDMA
	else if (module == &EDMA) {
		sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_EDMA);
	}
#endif
#ifdef ACA
	else if (module == &ACA) {
		sysclk_enable_module(SYSCLK_PORT_A, SYSCLK_AC);
	}
#endif
#ifdef ACB
	else if (module == &ACB) {
		sysclk_enable_module(SYSCLK_PORT_B, SYSCLK_AC);
	}
#endif
#ifdef ADCA
	else if (module == &ADCA) {
		sysclk_enable_module(SYSCLK_PORT_A, SYSCLK_ADC);
	}
#endif
#ifdef ADCB
	else if (module == &ADCB) {
		sysclk_enable_module(SYSCLK_PORT_B, SYSCLK_ADC);
	}
#endif
#ifdef DACA
	else if (module == &DACA) {
		sysclk_enable_module(SYSCLK_PORT_A, SYSCLK_DAC);
	}
#endif
// Workaround for bad XMEGA D header file
#if !XMEGA_D
#ifdef DACB
	else if (module == &DACB) {
		sysclk_enable_module(SYSCLK_PORT_B, SYSCLK_DAC);
	}
#endif
#endif // Workaround end
#ifdef TCC0
	else if (module == &TCC0) {
		sysclk_enable_module(SYSCLK_PORT_C, SYSCLK_TC0);
	}
#endif
#ifdef TCD0
	else if (module == &TCD0) {
		sysclk_enable_module(SYSCLK_PORT_D, SYSCLK_TC0);
	}
#endif
#ifdef TCE0
	else if (module == &TCE0) {
		sysclk_enable_module(SYSCLK_PORT_E, SYSCLK_TC0);
	}
#endif
#ifdef TCF0
	else if (module == &TCF0) {
		sysclk_enable_module(SYSCLK_PORT_F, SYSCLK_TC0);
	}
#endif
#ifdef TCC1
	else if (module == &TCC1) {
		sysclk_enable_module(SYSCLK_PORT_C, SYSCLK_TC1);
	}
#endif
#ifdef TCD1
	else if (module == &TCD1) {
		sysclk_enable_module(SYSCLK_PORT_D, SYSCLK_TC1);
	}
#endif
#ifdef TCE1
	else if (module == &TCE1) {
		sysclk_enable_module(SYSCLK_PORT_E, SYSCLK_TC1);
	}
#endif
#ifdef TCF1
	else if (module == &TCF1) {
		sysclk_enable_module(SYSCLK_PORT_F, SYSCLK_TC1);
	}
#endif
#ifdef TCC4
	else if (module == &TCC4) {
		sysclk_enable_module(SYSCLK_PORT_C, SYSCLK_TC4);
	}
#endif
#ifdef TCC5
	else if (module == &TCC5) {
		sysclk_enable_module(SYSCLK_PORT_C, SYSCLK_TC5);
	}
#endif
#ifdef TCD4
	else if (module == &TCD4) {
		sysclk_enable_module(SYSCLK_PORT_D, SYSCLK_TC4);
	}
#endif
#ifdef TCD5
	else if (module == &TCD5) {
		sysclk_enable_module(SYSCLK_PORT_D, SYSCLK_TC5);
	}
#endif
#ifdef HIRESC
	else if (module == &HIRESC) {
			sysclk_enable_module(SYSCLK_PORT_C, SYSCLK_HIRES);
	}
#endif
#ifdef HIRESD
	else if (module == &HIRESD) {
		sysclk_enable_module(SYSCLK_PORT_D, SYSCLK_HIRES);
	}
#endif
#ifdef HIRESE
	else if (module == &HIRESE) {
		sysclk_enable_module(SYSCLK_PORT_E, SYSCLK_HIRES);
	}
#endif
#ifdef HIRESF
	else if (module == &HIRESF) {
		sysclk_enable_module(SYSCLK_PORT_F, SYSCLK_HIRES);
	}
#endif
#ifdef SPIC
	else if (module == &SPIC) {
		sysclk_enable_module(SYSCLK_PORT_C, SYSCLK_SPI);
	}
#endif
#ifdef SPID
	else if (module == &SPID) {
		sysclk_enable_module(SYSCLK_PORT_D, SYSCLK_SPI);
	}
#endif
#ifdef SPIE
	else if (module == &SPIE) {
		sysclk_enable_module(SYSCLK_PORT_E, SYSCLK_SPI);
	}
#endif
#ifdef SPIF
	else if (module == &SPIF) {
		sysclk_enable_module(SYSCLK_PORT_F, SYSCLK_SPI);
	}
#endif
#ifdef USARTC0
	else if (module == &USARTC0) {
		sysclk_enable_module(SYSCLK_PORT_C, SYSCLK_USART0);
	}
#endif
#ifdef USARTD0
	else if (module == &USARTD0) {
		sysclk_enable_module(SYSCLK_PORT_D, SYSCLK_USART0);
	}
#endif
#ifdef USARTE0
	else if (module == &USARTE0) {
		sysclk_enable_module(SYSCLK_PORT_E, SYSCLK_USART0);
	}
#endif
#ifdef USARTF0
	else if (module == &USARTF0) {
		sysclk_enable_module(SYSCLK_PORT_F, SYSCLK_USART0);
	}
#endif
#ifdef USARTC1
	else if (module == &USARTC1) {
		sysclk_enable_module(SYSCLK_PORT_C, SYSCLK_USART1);
	}
#endif
#ifdef USARTD1
	else if (module == &USARTD1) {
		sysclk_enable_module(SYSCLK_PORT_D, SYSCLK_USART1);
	}
#endif
#ifdef USARTE1
	else if (module == &USARTE1) {
		sysclk_enable_module(SYSCLK_PORT_E, SYSCLK_USART1);
	}
#endif
#ifdef USARTF1
	else if (module == &USARTF1) {
		sysclk_enable_module(SYSCLK_PORT_F, SYSCLK_USART1);
	}
#endif
#ifdef TWIC
	else if (module == &TWIC) {
		sysclk_enable_module(SYSCLK_PORT_C, SYSCLK_TWI);
	}
#endif
#ifdef TWID
	else if (module == &TWID) {
		sysclk_enable_module(SYSCLK_PORT_D, SYSCLK_TWI);
	}
#endif
#ifdef TWIE
	else if (module == &TWIE) {
		sysclk_enable_module(SYSCLK_PORT_E, SYSCLK_TWI);
	}
#endif
#ifdef TWIF
	else if (module == &TWIF) {
		sysclk_enable_module(SYSCLK_PORT_F, SYSCLK_TWI);
	}
#endif
#ifdef XCL
	else if (module == &XCL) {
		sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_XCL);
	}
#endif
	else {
		Assert(false);
	}
}

/**
 * \brief Disable a peripheral's clock from its base address.
 *
 *  Disables the clock to a peripheral, given its base address. If the peripheral
 *  has an associated clock on the HSB bus, this will be disabled also.
 *
 * \param module Pointer to the module's base address.
 */
static inline void sysclk_disable_peripheral_clock(const volatile void *module)
{
	if (module == NULL) {
		Assert(false);
	}
#ifdef AES
	else if (module == &AES) {
		sysclk_disable_module(SYSCLK_PORT_GEN, SYSCLK_AES);
	}
#endif
#ifdef EBI
	else if (module == &EBI) {
		sysclk_disable_module(SYSCLK_PORT_GEN, SYSCLK_EBI);
	}
#endif
#ifdef RTC
	else if (module == &RTC) {
		sysclk_disable_module(SYSCLK_PORT_GEN, SYSCLK_RTC);
	}
#endif
#ifdef EVSYS
	else if (module == &EVSYS) {
		sysclk_disable_module(SYSCLK_PORT_GEN, SYSCLK_EVSYS);
	}
#endif
#ifdef DMA
	else if (module == &DMA) {
		sysclk_disable_module(SYSCLK_PORT_GEN, SYSCLK_DMA);
	}
#endif
#ifdef EDMA
	else if (module == &EDMA) {
		sysclk_disable_module(SYSCLK_PORT_GEN, SYSCLK_EDMA);
	}
#endif
#ifdef ACA
	else if (module == &ACA) {
		sysclk_disable_module(SYSCLK_PORT_A, SYSCLK_AC);
	}
#endif
#ifdef ACB
	else if (module == &ACB) {
		sysclk_disable_module(SYSCLK_PORT_B, SYSCLK_AC);
	}
#endif
#ifdef ADCA
	else if (module == &ADCA) {
		sysclk_disable_module(SYSCLK_PORT_A, SYSCLK_ADC);
	}
#endif
#ifdef ADCB
	else if (module == &ADCB) {
		sysclk_disable_module(SYSCLK_PORT_B, SYSCLK_ADC);
	}
#endif
#ifdef DACA
	else if (module == &DACA) {
		sysclk_disable_module(SYSCLK_PORT_A, SYSCLK_DAC);
	}
#endif
// Workaround for bad XMEGA D header file
#if !XMEGA_D
#ifdef DACB
	else if (module == &DACB) {
		sysclk_disable_module(SYSCLK_PORT_B, SYSCLK_DAC);
	}
#endif
#endif // Workaround end
#ifdef TCC0
	else if (module == &TCC0) {
		sysclk_disable_module(SYSCLK_PORT_C, SYSCLK_TC0);
	}
#endif
#ifdef TCD0
	else if (module == &TCD0) {
		sysclk_disable_module(SYSCLK_PORT_D, SYSCLK_TC0);
	}
#endif
#ifdef TCE0
	else if (module == &TCE0) {
		sysclk_disable_module(SYSCLK_PORT_E, SYSCLK_TC0);
	}
#endif
#ifdef TCF0
	else if (module == &TCF0) {
		sysclk_disable_module(SYSCLK_PORT_F, SYSCLK_TC0);
	}
#endif
#ifdef TCC1
	else if (module == &TCC1) {
		sysclk_disable_module(SYSCLK_PORT_C, SYSCLK_TC1);
	}
#endif
#ifdef TCD1
	else if (module == &TCD1) {
		sysclk_disable_module(SYSCLK_PORT_D, SYSCLK_TC1);
	}
#endif
#ifdef TCE1
	else if (module == &TCE1) {
		sysclk_disable_module(SYSCLK_PORT_E, SYSCLK_TC1);
	}
#endif
#ifdef TCF1
	else if (module == &TCF1) {
		sysclk_disable_module(SYSCLK_PORT_F, SYSCLK_TC1);
	}
#endif
#ifdef TCC4
	else if (module == &TCC4) {
		sysclk_disable_module(SYSCLK_PORT_C, SYSCLK_TC4);
	}
#endif
#ifdef TCC5
	else if (module == &TCC5) {
		sysclk_disable_module(SYSCLK_PORT_C, SYSCLK_TC5);
	}
#endif
#ifdef TCD4
	else if (module == &TCD4) {
		sysclk_disable_module(SYSCLK_PORT_D, SYSCLK_TC4);
	}
#endif
#ifdef TCD5
	else if (module == &TCD5) {
		sysclk_disable_module(SYSCLK_PORT_D, SYSCLK_TC5);
	}
#endif
#ifdef HIRESC
	else if (module == &HIRESC) {
			sysclk_disable_module(SYSCLK_PORT_C, SYSCLK_HIRES);
	}
#endif
#ifdef HIRESD
	else if (module == &HIRESD) {
		sysclk_disable_module(SYSCLK_PORT_D, SYSCLK_HIRES);
	}
#endif
#ifdef HIRESE
	else if (module == &HIRESE) {
		sysclk_disable_module(SYSCLK_PORT_E, SYSCLK_HIRES);
	}
#endif
#ifdef HIRESF
	else if (module == &HIRESF) {
		sysclk_disable_module(SYSCLK_PORT_F, SYSCLK_HIRES);
	}
#endif
#ifdef SPIC
	else if (module == &SPIC) {
		sysclk_disable_module(SYSCLK_PORT_C, SYSCLK_SPI);
	}
#endif
#ifdef SPID
	else if (module == &SPID) {
		sysclk_disable_module(SYSCLK_PORT_D, SYSCLK_SPI);
	}
#endif
#ifdef SPIE
	else if (module == &SPIE) {
		sysclk_disable_module(SYSCLK_PORT_E, SYSCLK_SPI);
	}
#endif
#ifdef SPIF
	else if (module == &SPIF) {
		sysclk_disable_module(SYSCLK_PORT_F, SYSCLK_SPI);
	}
#endif
#ifdef USARTC0
	else if (module == &USARTC0) {
		sysclk_disable_module(SYSCLK_PORT_C, SYSCLK_USART0);
	}
#endif
#ifdef USARTD0
	else if (module == &USARTD0) {
		sysclk_disable_module(SYSCLK_PORT_D, SYSCLK_USART0);
	}
#endif
#ifdef USARTE0
	else if (module == &USARTE0) {
		sysclk_disable_module(SYSCLK_PORT_E, SYSCLK_USART0);
	}
#endif
#ifdef USARTF0
	else if (module == &USARTF0) {
		sysclk_disable_module(SYSCLK_PORT_F, SYSCLK_USART0);
	}
#endif
#ifdef USARTC1
	else if (module == &USARTC1) {
		sysclk_disable_module(SYSCLK_PORT_C, SYSCLK_USART1);
	}
#endif
#ifdef USARTD1
	else if (module == &USARTD1) {
		sysclk_disable_module(SYSCLK_PORT_D, SYSCLK_USART1);
	}
#endif
#ifdef USARTE1
	else if (module == &USARTE1) {
		sysclk_disable_module(SYSCLK_PORT_E, SYSCLK_USART1);
	}
#endif
#ifdef USARTF1
	else if (module == &USARTF1) {
		sysclk_disable_module(SYSCLK_PORT_F, SYSCLK_USART1);
	}
#endif
#ifdef TWIC
	else if (module == &TWIC) {
		sysclk_disable_module(SYSCLK_PORT_C, SYSCLK_TWI);
	}
#endif
#ifdef TWID
	else if (module == &TWID) {
		sysclk_disable_module(SYSCLK_PORT_D, SYSCLK_TWI);
	}
#endif
#ifdef TWIE
	else if (module == &TWIE) {
		sysclk_disable_module(SYSCLK_PORT_E, SYSCLK_TWI);
	}
#endif
#ifdef TWIF
	else if (module == &TWIF) {
		sysclk_disable_module(SYSCLK_PORT_F, SYSCLK_TWI);
	}
#endif
#ifdef XCL
	else if (module == &XCL) {
		sysclk_disable_module(SYSCLK_PORT_GEN, SYSCLK_XCL);
	}
#endif
	else {
		Assert(false);
	}
}

/**
 * \brief Check if the synchronous clock is enabled for a module
 *
 * \param port ID of the port to which the module is connected (one of
 * the \c SYSCLK_PORT_* definitions).
 * \param id The ID (bitmask) of the peripheral module to check (one of
 * the \c SYSCLK_* module definitions).
 *
 * \retval true If the clock for module \a id on \a port is enabled.
 * \retval false If the clock for module \a id on \a port is disabled.
 */
static inline bool sysclk_module_is_enabled(enum sysclk_port_id port,
		uint8_t id)
{
	uint8_t mask = *((uint8_t *)&PR.PRGEN + port);
	return (mask & id) == 0;
}

#if XMEGA_AU || XMEGA_B || XMEGA_C || defined(__DOXYGEN__)
# if defined(CONFIG_USBCLK_SOURCE) || defined(__DOXYGEN__)
#  if (CONFIG_USBCLK_SOURCE == USBCLK_SRC_RCOSC)
#   define USBCLK_STARTUP_TIMEOUT         1
#  elif (CONFIG_USBCLK_SOURCE == USBCLK_SRC_PLL)
#   if (CONFIG_PLL0_SOURCE == PLL_SRC_XOSC)
#    define USBCLK_STARTUP_TIMEOUT        XOSC_STARTUP_TIMEOUT
#   elif (CONFIG_PLL0_SOURCE == PLL_SRC_RC32MHZ)
#    define USBCLK_STARTUP_TIMEOUT        1
#   elif (CONFIG_PLL0_SOURCE == PLL_SRC_RC2MHZ)
#    define USBCLK_STARTUP_TIMEOUT        1
#   else
#    error Unknow value for CONFIG_PLL0_SOURCE, see conf_clock.h.
#   endif
#  endif
# else /* CONFIG_USBCLK_SOURCE not defined */
#  define CONFIG_USBCLK_SOURCE USBCLK_SRC_RCOSC
#  define USBCLK_STARTUP_TIMEOUT         1
# endif /* CONFIG_USBCLK_SOURCE */
void sysclk_enable_usb(uint8_t frequency);
void sysclk_disable_usb(void);
#endif /* XMEGA_AU || XMEGA_B || XMEGA_C */
//@}

//! \name System Clock Source and Prescaler configuration
//@{

/**
 * \brief Set system clock prescaler configuration
 *
 * This function will change the system clock prescaler configuration to
 * match the parameters.
 *
 * \note The parameters to this function are device-specific.
 *
 * \param psadiv The prescaler A setting (one of the \c SYSCLK_PSADIV_*
 * definitions). This determines the clkPER4 frequency.
 * \param psbcdiv The prescaler B and C settings (one of the \c SYSCLK_PSBCDIV_*
 * definitions). These determine the clkPER2, clkPER and clkCPU frequencies.
 */
static inline void sysclk_set_prescalers(uint8_t psadiv, uint8_t psbcdiv)
{
	ccp_write_io((uint8_t *)&CLK.PSCTRL, psadiv | psbcdiv);
}

/**
 * \brief Change the source of the main system clock.
 *
 * \param src The new system clock source. Must be one of the constants
 * from the <em>System Clock Sources</em> section.
 */
static inline void sysclk_set_source(uint8_t src)
{
	ccp_write_io((uint8_t *)&CLK.CTRL, src);
}

/**
 * \brief Lock the system clock configuration
 *
 * This function will lock the current system clock source and prescaler
 * configuration, preventing any further changes.
 */
static inline void sysclk_lock(void)
{
	ccp_write_io((uint8_t *)&CLK.LOCK, CLK_LOCK_bm);
}

//@}

/**
 * \name RTC clock source control
 * @{
 */

/**
 * \brief Enable RTC clock with specified clock source
 *
 * \param id RTC clock source ID. Select from SYSCLK_RTCSRC_ULP,
 *           SYSCLK_RTCSRC_RCOSC, SYSCLK_RTCSRC_TOSC, SYSCLK_RTCSRC_RCOSC32,
 *           SYSCLK_RTCSRC_TOSC32 or SYSCLK_RTCSRC_EXTCLK
 */
static inline void sysclk_rtcsrc_enable(uint8_t id)
{
	Assert((id & ~CLK_RTCSRC_gm) == 0);

	switch (id) {
	case SYSCLK_RTCSRC_RCOSC:
#if !XMEGA_A && !XMEGA_D
	case SYSCLK_RTCSRC_RCOSC32:
#endif
		osc_enable(OSC_ID_RC32KHZ);
		osc_wait_ready(OSC_ID_RC32KHZ);
		break;
	case SYSCLK_RTCSRC_TOSC:
	case SYSCLK_RTCSRC_TOSC32:
#if !XMEGA_A && !XMEGA_D
	case SYSCLK_RTCSRC_EXTCLK:
#endif
		osc_enable(OSC_ID_XOSC);
		osc_wait_ready(OSC_ID_XOSC);
		break;
	}

	CLK.RTCCTRL = id | CLK_RTCEN_bm;
}

/**
 * \brief Disable RTC clock
 */
static inline void sysclk_rtcsrc_disable(void)
{
	CLK.RTCCTRL = 0;
}

/** @} */

//! \name System Clock Initialization
//@{

extern void sysclk_init(void);

//@}

#endif /* !__ASSEMBLY__ */

//! @}

#ifdef __cplusplus
}
#endif

#endif /* XMEGA_SYSCLK_H_INCLUDED */
