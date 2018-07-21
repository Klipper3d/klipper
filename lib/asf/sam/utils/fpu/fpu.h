/**
 * \file
 *
 * \brief FPU support for SAM.
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

#ifndef _FPU_H_INCLUDED_
#define _FPU_H_INCLUDED_

#include <compiler.h>

/** Address for ARM CPACR */
#define ADDR_CPACR 0xE000ED88

/** CPACR Register */
#define REG_CPACR  (*((volatile uint32_t *)ADDR_CPACR))

/**
 * \brief Enable FPU
 */
__always_inline static void fpu_enable(void)
{
	irqflags_t flags;
	flags = cpu_irq_save();
	REG_CPACR |=  (0xFu << 20);
	__DSB();
	__ISB();
	cpu_irq_restore(flags);
}

/**
 * \brief Disable FPU
 */
__always_inline static void fpu_disable(void)
{
	irqflags_t flags;
	flags = cpu_irq_save();
	REG_CPACR &= ~(0xFu << 20);
	__DSB();
	__ISB();
	cpu_irq_restore(flags);
}

/**
 * \brief Check if FPU is enabled
 *
 * \return Return ture if FPU is enabled, otherwise return false.
 */
__always_inline static bool fpu_is_enabled(void)
{
	return (REG_CPACR & (0xFu << 20));
}

#endif /* _FPU_H_INCLUDED_ */
