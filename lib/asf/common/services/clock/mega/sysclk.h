/**
 * \file
 *
 * \brief Chip-specific system clock management functions
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
#ifndef MEGA_SYSCLK_H_INCLUDED
#define MEGA_SYSCLK_H_INCLUDED

#include <board.h>
#include <compiler.h>
#include <parts.h>

/* Include clock configuration for the project. */
#include <conf_clock.h>

#ifdef __cplusplus
extern "C" {
#endif
#define ASM __asm__

/* CONFIG_SYSCLK_PSDIV  to use default if not defined*/
#ifndef CONFIG_SYSCLK_PSDIV
# define CONFIG_SYSCLK_PSDIV    SYSCLK_PSDIV_8
#endif

/* ! \name Prescaler Setting (relative to CLKsys) */
/* @{ */
#define SYSCLK_PSDIV_1      0   /* !< Do not prescale */
#define SYSCLK_PSDIV_2      1   /* !< Prescale CLKper4 by 2 */
#define SYSCLK_PSDIV_4      2   /* !< Prescale CLKper4 by 4 */
#define SYSCLK_PSDIV_8      3   /* !< Prescale CLKper4 by 8 */
#define SYSCLK_PSDIV_16     4   /* !< Prescale CLKper4 by 16 */
#define SYSCLK_PSDIV_32     5   /* !< Prescale CLKper4 by 32 */
#define SYSCLK_PSDIV_64     6   /* !< Prescale CLKper4 by 64 */
#define SYSCLK_PSDIV_128    7   /* !< Prescale CLKper4 by 128 */
#define SYSCLK_PSDIV_256    8   /* !< Prescale CLKper4 by 256 */

/* @} */

#if MEGA_RF || MEGA_XX0_1 || MEGA_XX4 || MEGA_XX4_A

#define NUMBER_OF_POWER_REG       2
/*Starting Address for power reduction*/
#define POWER_REG_ADD             PRR0

/* ! \name Power Reduction  Clock Port Numbers */
enum power_red_id {
	POWER_RED_REG0,     /* !< Devices on PRR0 */
#if !MEGA_XX4 || !MEGA_XX4_A || MEGA_XX0_1
	POWER_RED_REG1,     /* !< Devices on PRR1 */
#endif
};
#endif

/****************************************************
 * Given a dummy type but not used for these groups
 * to support for otherthen megaRF device.
 **************************************************/
#if MEGA_XX8 || MEGA_XX8_A || MEGA_UNSPECIFIED
#if AVR8_PART_IS_DEFINED(ATmega328PB) || MEGA_UNCATEGORIZED
#define NUMBER_OF_POWER_REG       2
/*Starting Address for power reduction*/
#define POWER_REG_ADD             PRR0
/* ! \name Power Reduction  Clock Port Numbers */
enum power_red_id {
	POWER_RED_REG0,     /* !< Devices on PRR */
};
#else
#define NUMBER_OF_POWER_REG       1
/*Starting Address for power reduction*/
#define POWER_REG_ADD             PRR

/* ! \name Power Reduction  Clock Port Numbers */
enum power_red_id {
	POWER_RED_REG0,     /* !< Devices on PRR */
};
#endif
#endif

/* Bit mask for the power reduction register based on */
/*   MCU ARCH.                                        */
#if MEGA_RF
/*Bit mask for PRR2 */

#define PRRAM0_bm                       1 << PRRAM0
#define PRRAM1_bm                       1 << PRRAM1
#define PRRAM2_bm                       1 << PRRAM2
#define PRRAM3_bm                       1 << PRRAM3
#endif

/*Bit mask for the power reduction 0 or PRR*/
#if !MEGA_XX_UN0 && !MEGA_XX_UN0
#define PRADC_bm                        1 << PRADC
#define PRUSART0_bm                     1 << PRUSART0
#define PRSPI_bm                        1 << PRSPI
#define PRTIM1_bm                       1 << PRTIM1
#endif

#if MEGA_RF
#define PRPGA_bm                        1 << PRPGA
#endif

#if !MEGA_UNSPECIFIED
#define PRTIM0_bm                       1 << PRTIM0
#define PRTIM2_bm                       1 << PRTIM2
#if AVR8_PART_IS_DEFINED(ATmega328PB) || AVR8_PART_IS_DEFINED(ATmega324PB)
#define PRTWI_bm                        1 << PRTWI0
#else
#define PRTWI_bm                        1 << PRTWI
#endif
#endif

#if MEGA_XX_UN2
#define PRLCD_bm                        1 << PRLCD
#endif

/*Bit mask for  PRR1  */
#ifdef PRR1
#if (MEGA_XX4 || MEGA_XX4_A)
#define PRTIM3_bm                       1 << PRTIM3
#endif
#endif

#if MEGA_RF || MEGA_XX4 || MEGA_XX4_A
#define PRUSART1_bm                     1 << PRUSART1
#define PRUSART2_bm                     1 << PRUSART2
#define PRUSART3_bm                     1 << PRUSART3
#define PRTIM3_bm                       1 << PRTIM3
#define PRTIM4_bm                       1 << PRTIM4
#define PRTIM5_bm                       1 << PRTIM5
#endif

#if MEGA_RF
#define PRTRX24_bm                      1 << PRTRX24
#endif

/**
 * \name Querying the system clock and its derived clocks
 */
/* @{ */

/**
 * \brief Return the current rate in Hz of the main system clock
 * To know the clock value at what frequency the main clock is running
 * \return Frequency of the main system clock, in Hz.
 * \todo : please initialize the SYSCLK_SOURCE in conf_clock.h file for
 * configured source clock using fuses.
 * \eg. #define SYSCLK_SOURCE SYSCLK_SRC_RC16MHZ to use internal RC
 * oscillator for clock source.
 */
static inline uint32_t sysclk_get_main_hz(void)
{
	switch (SYSCLK_SOURCE) {
	case SYSCLK_SRC_RC16MHZ:
		return 16000000UL;

	case SYSCLK_SRC_RC128KHZ:
		return 128000UL;

#if MEGA_RF
	case SYSCLK_SRC_TRS16MHZ:
		return 16000000UL;
#endif
#ifdef BOARD_EXTERNAL_CLK
	case SYSCLK_SRC_EXTERNAL:
		return BOARD_EXTERNAL_CLK;
#endif
	default:

		return 1000000UL;
	}
}

/**
 * \brief Return the current rate in Hz of source clock in Hz.
 *
 * This clock always runs at the same rate as the CPU clock unless the divider
 * is set.
 *
 * \return Frequency of the system clock, in Hz.
 */
static inline uint32_t sysclk_get_source_clock_hz(void)
{
	switch (CONFIG_SYSCLK_PSDIV) {
	case SYSCLK_PSDIV_1: /* Fall through */
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 2;
		} else {
			return sysclk_get_main_hz();
		}

	case SYSCLK_PSDIV_2:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 4;
		} else {
			return sysclk_get_main_hz() / 2;
		}

	case SYSCLK_PSDIV_4:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 8;
		} else {
			return sysclk_get_main_hz() / 4;
		}

	case SYSCLK_PSDIV_8:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 16;
		} else {
			return sysclk_get_main_hz() / 8;
		}

	case SYSCLK_PSDIV_16:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 32;
		} else {
			return sysclk_get_main_hz() / 16;
		}

	case SYSCLK_PSDIV_32:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 64;
		} else {
			return sysclk_get_main_hz() / 32;
		}

	case SYSCLK_PSDIV_64:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 128;
		} else {
			return sysclk_get_main_hz() / 64;
		}

	case SYSCLK_PSDIV_128:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 256;
		} else {
			return sysclk_get_main_hz() / 128;
		}

	case SYSCLK_PSDIV_256:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 512;
		} else {
			return sysclk_get_main_hz() / 256;
		}

	default:
		/*Invalide case*/
		return 0;
	}
}

/**
 * \brief Return the current rate in Hz of the CPU clock.
 *
 * \return Frequency of the CPU clock, in Hz.
 */
static inline uint32_t sysclk_get_cpu_hz(void)
{
	return sysclk_get_source_clock_hz();
}

/**
 * \brief Return the current rate in Hz of the clock from internal oscillator
 * \return Frequency of the CPU clock, in Hz.
 */
static inline uint32_t sysclk_get_rc_osc_hz(void)
{
	switch (CONFIG_SYSCLK_PSDIV) {
	case SYSCLK_PSDIV_1:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 2;
		}

	case SYSCLK_PSDIV_2:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 4;
		}

	case SYSCLK_PSDIV_4:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 8;
		}

	case SYSCLK_PSDIV_8:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 16;
		}

	case SYSCLK_PSDIV_16:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 32;
		}

	case SYSCLK_PSDIV_32:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 64;
		}

	case SYSCLK_PSDIV_64:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 128;
		}

	case SYSCLK_PSDIV_128:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 256;
		}

	case SYSCLK_PSDIV_256:
		if (SYSCLK_SOURCE == SYSCLK_SRC_RC16MHZ ||
				SYSCLK_SOURCE == SYSCLK_SRC_RC128KHZ) {
			return sysclk_get_main_hz() / 512;
		}

	default:
		return 0;
	}
}

/* ! \name Enabling and disabling synchronous clocks */
/* @{ */

/**
 * \brief Enable the clock to peripheral \a id on port \a port
 *
 * \param port ID of the port to which the module is connected (one of
 * the \c power_red_id *definitions).
 * \param id The ID (bitmask) of the peripheral module to be disabled.*
 */
extern void sysclk_enable_module(enum power_red_id port, uint8_t id);

/**
 * \brief Disable the clock to peripheral \a id on port \a port
 *
 * \param port ID of the port to which the module is connected (one of
 * the \c power_red_id *definitions).
 * \param id The ID (bit mask) of the peripheral module to be disabled.
 */
extern void sysclk_disable_module(enum power_red_id port, uint8_t id);

/**
 * \brief Enable a peripherals clock from its base address.
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

#if !MEGA_XX_UN0 && !MEGA_XX_UN1
	else if (module == &ADC) {
		sysclk_enable_module(POWER_RED_REG0, PRADC_bm);
#if MEGA_RF
		sysclk_enable_module(POWER_RED_REG0, PRPGA_bm);
#endif
	}
#if !MEGA_UNCATEGORIZED
	else if (module == &UCSR0A) {
		sysclk_enable_module(POWER_RED_REG0, PRUSART0_bm);
	}
#endif
#if MEGA_RF
	else if (module == &SPCR) {
		sysclk_enable_module(POWER_RED_REG0, PRSPI_bm);
	}
#endif

	else if (module == &TCCR1A) {
		sysclk_enable_module(POWER_RED_REG0, PRTIM1_bm);
	}

#if MEGA_XX_UN2
	else if (module == &LCDCRA) {
		sysclk_enable_module(POWER_RED_REG0, PRLCD_bm);
	}
#endif

#if !MEGA_XX_UN2 && !MEGA_UNCATEGORIZED
	else if (module == &TCCR0A) {
		sysclk_enable_module(POWER_RED_REG0, PRTIM0_bm);
	} else if (module == &TCCR2A) {
		sysclk_enable_module(POWER_RED_REG0, PRTIM2_bm);
#if AVR8_PART_IS_DEFINED(ATmega328PB) || AVR8_PART_IS_DEFINED(ATmega324PB)
	} else if (module == &TWBR0) {
		sysclk_enable_module(POWER_RED_REG0, PRTWI_bm);
	}
#else
	} else if (module == &TWBR) {
		sysclk_enable_module(POWER_RED_REG0, PRTWI_bm);
	}
#endif
#endif

#if MEGA_RF
	else if (module == &UCSR1A) {
		sysclk_enable_module(POWER_RED_REG1, PRUSART1_bm);
	} else if (module == &TCCR3A) {
		sysclk_enable_module(POWER_RED_REG1, PRTIM3_bm);
	} else if (module == &TCCR4A) {
		sysclk_enable_module(POWER_RED_REG1, PRTIM4_bm);
	} else if (module == &TCCR5A) {
		sysclk_enable_module(POWER_RED_REG1, PRTIM5_bm);
	} else if (module == &TRX_CTRL_0) {
		sysclk_enable_module(POWER_RED_REG1, PRTRX24_bm);
	}
#endif
#endif
	else {
		Assert(false);
	}
}

/**
 * \brief Disable a peripheral's clock from its base address.
 *
 *  Disables the clock to a peripheral, given its base address.
 *
 * \param module Pointer to the module's base address.
 */
static inline void sysclk_disable_peripheral_clock(const volatile void *module)
{
	if (module == NULL) {
		Assert(false);
	}

#if !MEGA_XX_UN0 && !MEGA_XX_UN1
	else if (module == &ADC) {
		sysclk_disable_module(POWER_RED_REG0, PRADC_bm);
#if MEGA_RF
		sysclk_disable_module(POWER_RED_REG0, PRPGA_bm);
#endif
	} 
#if !MEGA_UNCATEGORIZED
	else if (module == &UCSR0A) {
		sysclk_disable_module(POWER_RED_REG0, PRUSART0_bm);
	}
#endif
#if MEGA_RF
	else if (module == &SPCR) {
		sysclk_disable_module(POWER_RED_REG0, PRSPI_bm);
	}
#endif
	else if (module == &TCCR1A) {
		sysclk_disable_module(POWER_RED_REG0, PRTIM1_bm);
	}

#if MEGA_XX_UN2
	else if (module == &LCDCRA) {
		sysclk_disable_module(POWER_RED_REG0, PRLCD_bm);
	}
#endif
#if !MEGA_XX_UN2 && !MEGA_UNCATEGORIZED
	else if (module == &TCCR0A) {
		sysclk_disable_module(POWER_RED_REG0, PRTIM0_bm);
	} else if (module == &TCCR2A) {
		sysclk_disable_module(POWER_RED_REG0, PRTIM2_bm);
#if AVR8_PART_IS_DEFINED(ATmega328PB) || AVR8_PART_IS_DEFINED(ATmega324PB)
	} else if (module == &TWBR0) {
		sysclk_disable_module(POWER_RED_REG0, PRTWI_bm);
	}
#else
	} else if (module == &TWBR) {
		sysclk_disable_module(POWER_RED_REG0, PRTWI_bm);
	}
#endif
#endif

#if MEGA_RF
	else if (module == &UCSR1A) {
		sysclk_disable_module(POWER_RED_REG1, PRUSART1_bm);
	} else if (module == &TCCR3A) {
		sysclk_disable_module(POWER_RED_REG1, PRTIM3_bm);
	} else if (module == &TCCR4A) {
		sysclk_disable_module(POWER_RED_REG1, PRTIM4_bm);
	} else if (module == &TCCR5A) {
		sysclk_disable_module(POWER_RED_REG1, PRTIM5_bm);
	} else if (module == &TRX_CTRL_0) {
		sysclk_disable_module(POWER_RED_REG1, PRTRX24_bm);
	}
#endif
#endif
	else {
		Assert(false);
	}
}

/**
 * \brief Set system clock prescaler configuration
 *
 * This function will change the system clock prescaler configuration to
 * match the parameters.
 *
 * \note The parameters to this function are device-specific.
 *
 * \param psbcdiv The prescaler  settings (one of the \c SYSCLK_PSCDIV_*
 * definitions). These determine the clkIO, clkADC and clkCPU frequencies.
 * Note: Prescaler setting is not working with the brain dead un optimised code
 * e.g. avr-gcc -00
 */

static inline void sysclk_set_prescalers(uint8_t psdiv)
{
#if !MEGA_UNSPECIFIED
	irqflags_t flags = cpu_irq_save();

	ASM(
			"push r21                    \n\t"

			"ldi  r21, 0x80              \n\t" /* CLKPR = 1 <<
	                                                    * CLKPCE   */
			"sts  0x0061, r21            \n\t"

#if (CONFIG_SYSCLK_PSDIV == SYSCLK_PSDIV_1)
			"ldi  r21, 0x00              \n\t" /* divider = 0; //
	                                                    * RC-Oscillator/2 */
#endif
#if (CONFIG_SYSCLK_PSDIV == SYSCLK_PSDIV_2)
			"ldi  r21, 0x01              \n\t" /* divider = 2; //
	                                                    * RC-Oscillator/4 */
#endif
#if (CONFIG_SYSCLK_PSDIV == SYSCLK_PSDIV_4)
			"ldi  r21, 0x02              \n\t" /* divider = 4; //
	                                                    * RC-Oscillator/8 */
#endif
#if (CONFIG_SYSCLK_PSDIV == SYSCLK_PSDIV_8)
			"ldi  r21, 0x03              \n\t" /* divider = 8; //
	                                                    * RC-Oscillator/16
	                                                    **/
#endif
#if (CONFIG_SYSCLK_PSDIV == SYSCLK_PSDIV_16)
			"ldi  r21, 0x04              \n\t" /* divider = 16; //
	                                                   * RC-Oscillator/32*/
#endif
#if (CONFIG_SYSCLK_PSDIV == SYSCLK_PSDIV_32)
			"ldi  r21, 0x05              \n\t" /* divider = 32; //
	                                                   * RC-Oscillator/64*/
#endif
#if (CONFIG_SYSCLK_PSDIV == SYSCLK_PSDIV_64)
			"ldi  r21, 0x06              \n\t" /* divider = 64;//
	                                                    *
	                                                    *RC-Oscillator/128*/
#endif
#if (CONFIG_SYSCLK_PSDIV == SYSCLK_PSDIV_128)
			"ldi  r21, 0x07              \n\t" /* divider =
	                                                    *
	                                                    *128;//RC-Oscillator/256*/
#endif
#if (CONFIG_SYSCLK_PSDIV == SYSCLK_PSDIV_256)
			"ldi  r21, 0x08             \n\t" /* divider =
	                                                   *
	                                                   *256;//RC-Oscillator/512*/
#endif
			"sts  0x0061, r21            \n\t" /* CLKPR = divider
	                                                    *      */

			"pop r21                     \n\t"
			);

	cpu_irq_restore(flags);
#endif
}

/**
 * \brief Retrieves the current rate in Hz of the Peripheral Bus clock attached
 *  to the specified peripheral.
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
	} else if (module == &ADC) {
		return sysclk_get_source_clock_hz();
	}

#if !MEGA_UNSPECIFIED
	else if (module == &UCSR0A) {
		return sysclk_get_source_clock_hz();
	}
#endif

#if MEGA_RF
	else if (module == &SPCR) {
		return sysclk_get_source_clock_hz();
	}
#endif
	else if (module == &TCCR1A) {
		return sysclk_get_source_clock_hz();
	}
#if !MEGA_UNSPECIFIED
	else if (module == &TCCR0A) {
		return sysclk_get_source_clock_hz();
	} else if (module == &TCCR2A) {
		return sysclk_get_source_clock_hz();
	} else if (module == &UCSR0A) {
		return sysclk_get_source_clock_hz();
#if AVR8_PART_IS_DEFINED(ATmega328PB) || AVR8_PART_IS_DEFINED(ATmega324PB)
	} else if (module == &TWBR0) {
		sysclk_disable_module(POWER_RED_REG0, PRTWI_bm);
	}
#else
	} else if (module == &TWBR) {
		return sysclk_get_source_clock_hz();
	}
#endif
#endif

#if MEGA_RF
	else if (module == &TCCR3A) {
		return sysclk_get_source_clock_hz();
	} else if (module == &TCCR4A) {
		return sysclk_get_source_clock_hz();
	} else if (module == &TCCR5A) {
		return sysclk_get_source_clock_hz();
	} else if (module == &TRX_CTRL_0) {
		return sysclk_get_source_clock_hz();
	} else if (module == &DRTRAM0) {
		return sysclk_get_source_clock_hz();
	} else if (module == &DRTRAM1) {
		return sysclk_get_source_clock_hz();
	} else if (module == &DRTRAM2) {
		return sysclk_get_source_clock_hz();
	} else if (module == &DRTRAM3) {
		return sysclk_get_source_clock_hz();
	}
#endif
	else {
		Assert(false);
		return 0;
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
static inline bool sysclk_module_is_enabled(enum power_red_id port,
		uint8_t id)
{
#if !MEGA_UNSPECIFIED && !MEGA_XX
	uint8_t *reg = (uint8_t *)&(POWER_REG_ADD);
	return (*(reg + port) & id) == 0;
#endif
}

/* ! \name System Clock Initialization */
/* @{ */

extern void sysclk_init(void);

/* ! @} */

#ifdef __cplusplus
}
#endif

#endif /* MEGA_SYSCLK_H_INCLUDED */
