/**
 * \file
 *
 * \brief Parallel Input/Output (PIO) interrupt handler for SAM.
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

#ifndef PIO_HANDLER_H_INCLUDED
#define PIO_HANDLER_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void pio_handler_process(Pio *p_pio, uint32_t ul_id);
void pio_handler_set_priority(Pio *p_pio, IRQn_Type ul_irqn, uint32_t ul_priority);
uint32_t pio_handler_set(Pio *p_pio, uint32_t ul_id, uint32_t ul_mask,
		uint32_t ul_attr, void (*p_handler) (uint32_t, uint32_t));
uint32_t pio_handler_set_pin(uint32_t ul_pin, uint32_t ul_flag,
		void (*p_handler) (uint32_t, uint32_t));

#if (SAM3S || SAM4S || SAM4E)
void pio_capture_handler_set(void (*p_handler)(Pio *));
#endif

#ifdef __cplusplus
}
#endif

#endif /* PIO_HANDLER_H_INCLUDED */
