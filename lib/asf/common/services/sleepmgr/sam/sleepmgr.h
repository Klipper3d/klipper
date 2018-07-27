/**
 * \file
 *
 * \brief SAM3/SAM4 Sleep manager implementation.
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

#ifndef SAM_SLEEPMGR_INCLUDED
#define SAM_SLEEPMGR_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler.h>
#include <conf_sleepmgr.h>
#include <sleep.h>
#include <interrupt.h>

/**
 * \weakgroup sleepmgr_group
 * @{
 */
enum sleepmgr_mode {
	//! Active mode.
	SLEEPMGR_ACTIVE = 0,
	/*! WFE sleep mode.
	 *  Potential Wake Up sources:
	 *  fast startup events (USB, RTC, RTT, WKUPs),
	 *  interrupt, and events. */
	SLEEPMGR_SLEEP_WFE,
	/*! WFI sleep mode.
	 * Potential Wake Up sources: fast startup events and interrupt. */
	SLEEPMGR_SLEEP_WFI,
	/*! Wait mode, wakeup fast (in 3ms).
	 *  XTAL is not disabled when sleep.
	 *  Potential Wake Up sources: fast startup events */
	SLEEPMGR_WAIT_FAST,
	/*! Wait mode.
	 *  Potential Wake Up sources: fast startup events */
	SLEEPMGR_WAIT,
#if (!(SAMG51 || SAMG53 || SAMG54))
	//! Backup mode. Potential Wake Up sources: WKUPs, SM, RTT, RTC.
	SLEEPMGR_BACKUP,
#endif
	SLEEPMGR_NR_OF_MODES,
};

/**
 * \internal
 * \name Internal arrays
 * @{
 */
#if defined(CONFIG_SLEEPMGR_ENABLE) || defined(__DOXYGEN__)
//! Sleep mode lock counters
extern uint8_t sleepmgr_locks[];
#endif /* CONFIG_SLEEPMGR_ENABLE */
//! @}


static inline void sleepmgr_sleep(const enum sleepmgr_mode sleep_mode)
{
	Assert(sleep_mode != SLEEPMGR_ACTIVE);
#ifdef CONFIG_SLEEPMGR_ENABLE
	cpu_irq_disable();

	// Atomically enable the global interrupts and enter the sleep mode.
	pmc_sleep(sleep_mode);
#else
	UNUSED(sleep_mode);
	cpu_irq_enable();
#endif /* CONFIG_SLEEPMGR_ENABLE */

}

//! @}

#ifdef __cplusplus
}
#endif

#endif /* SAM_SLEEPMGR_INCLUDED */
