/**
 * \file
 *
 * \brief Global interrupt management for SAM D20, SAM3 and SAM4 (NVIC based)
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

#include "interrupt_sam_nvic.h"

#if !defined(__DOXYGEN__)
/* Deprecated - global flag to determine the global interrupt state. Required by
 * QTouch library, however new applications should use cpu_irq_is_enabled()
 * which probes the true global interrupt state from the CPU special registers.
 */
volatile bool g_interrupt_enabled = true;
#endif

void cpu_irq_enter_critical(void)
{
	if (cpu_irq_critical_section_counter == 0) {
		if (cpu_irq_is_enabled()) {
			cpu_irq_disable();
			cpu_irq_prev_interrupt_state = true;
		} else {
			/* Make sure the to save the prev state as false */
			cpu_irq_prev_interrupt_state = false;
		}

	}

	cpu_irq_critical_section_counter++;
}

void cpu_irq_leave_critical(void)
{
	/* Check if the user is trying to leave a critical section when not in a critical section */
	Assert(cpu_irq_critical_section_counter > 0);

	cpu_irq_critical_section_counter--;

	/* Only enable global interrupts when the counter reaches 0 and the state of the global interrupt flag
	   was enabled when entering critical state */
	if ((cpu_irq_critical_section_counter == 0) && (cpu_irq_prev_interrupt_state)) {
		cpu_irq_enable();
	}
}

