/**
 * \file
 *
 * \brief Watchdog Timer (WDT) driver for SAM4L.
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

#ifndef WDT_SAM4L_H_INCLUDED
#define WDT_SAM4L_H_INCLUDED

#include "compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup group_sam_drivers_wdt_sam4l WDT_SAM4L - SAM4L Watchdog Timer
 *
 * Driver for the WDT (Watchdog Timer). This driver provides access to the main
 * features of the WDT controller.
 * The Watchdog Timer can be used to prevent system lock-up if the software
 * becomes trapped in a deadlock. It can  generate a general reset or a
 * processor reset only.
 *
 * See \ref sam_wdt_sam4l_quickstart.
 *
 * @{
 */

/**
 * \brief Watchdog Timer period configuration enum.
 *
 * Enum for the possible period settings of the Watchdog timer module, for
 * values requiring a period as a number of Watchdog timer clock ticks.
 *
 * Formula for Ttimeout = 2pow(PSEL+1) / Fclk_cnt
 * Formula for Ttimeban = 2pow(TBAN+1) / Fclk_cnt
 */
enum wdt_period {
	WDT_PERIOD_NONE           = 0,
	WDT_PERIOD_MIN_CLK        = 7,
	WDT_PERIOD_256_CLK        = 7,   /* PSEL = TBAN = 7 */
	WDT_PERIOD_512_CLK        = 8,   /* PSEL = TBAN = 8 */
	WDT_PERIOD_1024_CLK       = 9,   /* PSEL = TBAN = 9 */
	WDT_PERIOD_2048_CLK       = 10,  /* PSEL = TBAN = 10 */
	WDT_PERIOD_4096_CLK       = 11,  /* PSEL = TBAN = 11 */
	WDT_PERIOD_8192_CLK       = 12,  /* PSEL = TBAN = 12 */
	WDT_PERIOD_16384_CLK      = 13,  /* PSEL = TBAN = 13 */
	WDT_PERIOD_32768_CLK      = 14,  /* PSEL = TBAN = 14 */
	WDT_PERIOD_65536_CLK      = 15,  /* PSEL = TBAN = 15 */
	WDT_PERIOD_131072_CLK     = 16,  /* PSEL = TBAN = 16 */
	WDT_PERIOD_262144_CLK     = 17,  /* PSEL = TBAN = 17 */
	WDT_PERIOD_524288_CLK     = 18,  /* PSEL = TBAN = 18 */
	WDT_PERIOD_1048576_CLK    = 19,  /* PSEL = TBAN = 19 */
	WDT_PERIOD_2097152_CLK    = 20,  /* PSEL = TBAN = 20 */
	WDT_PERIOD_4194304_CLK    = 21,  /* PSEL = TBAN = 21 */
	WDT_PERIOD_8388608_CLK    = 22,  /* PSEL = TBAN = 22 */
	WDT_PERIOD_16777216_CLK   = 23,  /* PSEL = TBAN = 23 */
	WDT_PERIOD_33554432_CLK   = 24,  /* PSEL = TBAN = 24 */
	WDT_PERIOD_67108864_CLK   = 25,  /* PSEL = TBAN = 25 */
	WDT_PERIOD_134217728_CLK  = 26,  /* PSEL = TBAN = 26 */
	WDT_PERIOD_268435456_CLK  = 27,  /* PSEL = TBAN = 27 */
	WDT_PERIOD_536870912_CLK  = 28,  /* PSEL = TBAN = 28 */
	WDT_PERIOD_1073741824_CLK = 29,  /* PSEL = TBAN = 29 */
	WDT_PERIOD_2147483648_CLK = 30,  /* PSEL = TBAN = 30 */
	WDT_PERIOD_4294967296_CLK = 31,  /* PSEL = TBAN = 31 */
	WDT_PERIOD_MAX_CLK        = 31,
};

/**
 * \brief Watchdog Timer configuration structure.
 *
 * Configuration structure for a Watchdog Timer instance. This
 * structure could be initialized by the \ref wdt_get_config_defaults()
 * function before being modified by the user application.
 */
struct wdt_config {
	/** Clock source select */
	uint32_t clk_src;
/** Select the system RC oscillator (RCSYS) as clock source */
#define WDT_CLK_SRC_RCSYS    0
/** Select the 32KHz oscillator as clock source. */
#define WDT_CLK_SRC_32K      WDT_CTRL_CSSEL

	/** WDT mode */
	uint32_t wdt_mode;
/** Basic mode  */
#define WDT_MODE_BASIC    0
/** Window mode  */
#define WDT_MODE_WINDOW   WDT_CTRL_MODE

	/** WDT interrupt mode */
	uint32_t wdt_int;
/** WDT Interrupt Mode is disabled.  */
#define WDT_INT_DIS    0
/** WDT Interrupt Mode is enabled.  */
#define WDT_INT_EN    WDT_CTRL_IM

	/** Number of CLK_CNT until the WDT expires. */
	enum wdt_period timeout_period;
	/** Number of CLK_CNT until the reset window opens. */
	enum wdt_period window_period;
	/** Disable flash calibration redone after a watchdog reset. */
	bool disable_flash_cali;
	/** Disable the watchdog after a watchdog reset. */
	bool disable_wdt_after_reset;

	/**
	 * If set, the watchdog will be enabled and locked to the current
	 * configuration (SFV bit).
	 *
	 * \note Only a reset unlocks the SFV bit.
	 */
	bool always_on;
};

/**
 * \brief Watchdog Timer driver software instance structure.
 *
 * Device instance structure for a Watchdog Timer driver instance. This
 * structure should be initialized by the \ref wdt_init() function to
 * associate the instance with a particular hardware module of the device.
 */
struct wdt_dev_inst {
	/** Base address of the WDT module. */
	Wdt *hw_dev;
	/** Pointer to WDT configuration structure. */
	struct wdt_config  *wdt_cfg;
};

void wdt_get_config_defaults(struct wdt_config *const cfg);

bool wdt_init(
		struct wdt_dev_inst *const dev_inst,
		Wdt *const wdt,
		struct wdt_config *const cfg);

void wdt_enable(struct wdt_dev_inst *const dev_inst);

void wdt_disable(struct wdt_dev_inst *const dev_inst);

void wdt_clear(struct wdt_dev_inst *const dev_inst);

/**
 * \brief Get the watchdog timer status.
 *
 * \return Bitmask of watchdog timer status.
 */
static inline uint32_t wdt_get_status(struct wdt_dev_inst *const dev_inst)
{
	Wdt *wdt = dev_inst->hw_dev;

	return wdt->WDT_SR;
}

bool wdt_reset_mcu(void);

/**
 * \brief Get the watchdog interrupt mask.
 *
 * \return Bitmask of watchdog interrupt mask.
 */
static inline uint32_t wdt_get_interrupt_mask(struct wdt_dev_inst *const dev_inst)
{
	Wdt *wdt = dev_inst->hw_dev;

	return wdt->WDT_IMR;
}

/**
 * \brief Get the watchdog interrupt status.
 *
 * \return Bitmask of watchdog interrupt status.
 */
static inline uint32_t wdt_get_interrupt_status(struct wdt_dev_inst *const dev_inst)
{
	Wdt *wdt = dev_inst->hw_dev;

	return wdt->WDT_ISR;
}

/**
 * \brief Enable the WDT module interrupt.
 *
 * \param dev_inst    Device structure pointer.
 */
static inline void wdt_enable_interrupt(struct wdt_dev_inst *const dev_inst)
{
	Wdt *wdt = dev_inst->hw_dev;

	wdt->WDT_IER = WDT_IER_WINT;
}

/**
 * \brief Disable the WDT module interrupt.
 *
 * \param dev_inst    Device structure pointer.
 */
static inline void wdt_disable_interrupt(struct wdt_dev_inst *const dev_inst)
{
	Wdt *wdt = dev_inst->hw_dev;

	wdt->WDT_IDR = WDT_IDR_WINT;
}

/**
 * \brief Clear the WDT module interrupt status.
 *
 * \param dev_inst    Device structure pointer.
 */
static inline void wdt_clear_interrupt(struct wdt_dev_inst *const dev_inst)
{
	Wdt *wdt = dev_inst->hw_dev;

	wdt->WDT_ICR = WDT_ICR_WINT;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

/**
 * \page sam_wdt_sam4l_quickstart Quickstart guide for SAM4L watchdog driver
 *
 * This is the quickstart guide for the \ref group_sam_drivers_wdt_sam4l
 * "SAM4L watchdog driver", with step-by-step instructions on how to
 * configure and use the driver in a selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section wdt_sam4l_use_cases Use cases
 * - \ref wdt_sam4l_basic_use_case
 * - \ref wdt_sam4l_reset_mcu
 *
 * \section wdt_sam4l_basic_use_case Basic use case
 * In this basic use case, the watchdog will use the RCSYS as the source clock
 * and the timeout period will be configured as 0.57 second. Then the watchdog
 * will be kicked every 100ms.
 *
 * \subsection sam_wdt_sam4l_quickstart_prereq Prerequisites
 * - \ref sysclk_group
 *
 * \section wdt_sam4l_basic_use_case_setup Setup Steps
 * \note The watchdog clock (CLK_WDT) is enabled at reset and the default
 * source is system RC oscillator(RCSYS). If you want to use 32KHz oscillator
 * as watchdog clock, please make sure it is enabled first:
 * \code
	// Enable WDT clock source if need
	if (BPM->BPM_PMCON & BPM_PMCON_CK32S) {
	    // Enable 32K RC oscillator
	    if (!osc_is_ready(OSC_ID_RC32K)) {
	        osc_enable(OSC_ID_RC32K);
	        osc_wait_ready(OSC_ID_RC32K);
	    }
	} else {
	    // Enable external OSC32 oscillator
	    if (!osc_is_ready(OSC_ID_OSC32)) {
	        osc_enable(OSC_ID_OSC32);
	        osc_wait_ready(OSC_ID_OSC32);
	    }
	}
\endcode
 *
 * \subsection wdt_sam4l_basic_use_case_setup_code Setup Example Code
 * Add the following code in the application C-file to setup watchdog:
 * \code
	    // WDT instance
	    struct wdt_dev_inst g_wdt_inst;

	    // WDT configuration
	    struct wdt_config   g_wdt_cfg;

	    // Initialize and enable the watchdog
	    wdt_get_config_defaults(&g_wdt_cfg);
	    g_wdt_cfg.timeout_period = WDT_PERIOD_65536_CLK;
	    wdt_init(&g_wdt_inst, WDT, &g_wdt_cfg);
\endcode
 *
 * \subsection wdt_sam4l_basic_use_case_setup_workflow Setup Workflow
 * -# Create variables to store the watchdog instance and configuration:
 *   \code
	struct wdt_dev_inst g_wdt_inst;
	struct wdt_config   g_wdt_cfg;
\endcode
 * -# Get default configuration but change timeout period to 0.57s
 *  (Ttimeout = 2pow(PSEL+1) / Fclk_cnt = 65535 / 115000).
 *  \code
	wdt_get_config_defaults(&g_wdt_cfg);
	g_wdt_cfg.timeout_period = WDT_PERIOD_65536_CLK;
\endcode
 * -# Initialize the watchdog:
 *  \code
	wdt_init(&g_wdt_inst, WDT, &g_wdt_cfg);
\endcode
 *
 * \section wdt_sam4l_basic_use_case_usage Usage Steps
 * \subsection wdt_sam4l_basic_use_example_code Usage Example Code
 * Add to, e.g., main loop in application C-file:
 * \code
	   wdt_enable(&g_wdt_inst);

	   while (1) {
	        wdt_clear(&g_wdt_inst);
	        // delay 100ms
	   }
\endcode
 *
 * \subsection wdt_sam4l_basic_use_workflow Usage Workflow
 * -# Enable the watchdog:
 *  \code wdt_enable(&g_wdt_inst); \endcode
 * -# Kick the watchdog in every 100ms:
 *  \code
	while (1) {
	     wdt_clear(&g_wdt_inst);
	     // delay 100ms
	}
\endcode
 * -# A daemon program is created now. The MCU tasks can be added after
 * the code of wdt_clear() then the tasks are under the monitor of watchdog.
 * If the tasks lasted more than 0.57 second, the options of the watchdog
 * timeout period should be adjusted accordingly.
 *
 * \section wdt_sam4l_reset_mcu Reset MCU by the WDT
 * We can reset MCU by generating a WDT reset as soon as possible.
 * \code
	 bool ret;

	 ret = wdt_reset_mcu();
\endcode
 */

#endif /* WDT_SAM4L_H_INCLUDED */
