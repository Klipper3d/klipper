/**
 * \file
 *
 * \brief AVR XMEGA Sleep manager implementation
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
#ifndef XMEGA_SLEEPMGR_H
#define XMEGA_SLEEPMGR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler.h>
#include <conf_sleepmgr.h>
#include <sleep.h>

/**
 * \weakgroup sleepmgr_group
 * @{
 */

enum sleepmgr_mode {
	//! Active mode.
	SLEEPMGR_ACTIVE = 0,
	//! Idle mode.
	SLEEPMGR_IDLE,
	//! Extended Standby mode.
	SLEEPMGR_ESTDBY,
	//! Power Save mode.
	SLEEPMGR_PSAVE,
	//! Standby mode.
	SLEEPMGR_STDBY,
	//! Power Down mode.
	SLEEPMGR_PDOWN,
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
/**
 * \brief Look-up table with sleep mode configurations
 * \note This is located in program memory (Flash) as it is constant.
 */
extern enum SLEEP_SMODE_enum sleepmgr_configs[];
#endif /* CONFIG_SLEEPMGR_ENABLE */
//! @}

static inline void sleepmgr_sleep(const enum sleepmgr_mode sleep_mode)
{
	Assert(sleep_mode != SLEEPMGR_ACTIVE);
#ifdef CONFIG_SLEEPMGR_ENABLE
	sleep_set_mode(sleepmgr_configs[sleep_mode-1]);
	sleep_enable();

	cpu_irq_enable();
	sleep_enter();

	sleep_disable();
#else
	cpu_irq_enable();
#endif /* CONFIG_SLEEPMGR_ENABLE */

}

//! @}

#ifdef __cplusplus
}
#endif

#endif /* XMEGA_SLEEPMGR_H */
