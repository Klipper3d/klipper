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

#include "pio.h"
#include "pio_handler.h"

/**
 * Maximum number of interrupt sources that can be defined. This
 * constant can be increased, but the current value is the smallest possible one
 * that will be compatible with all existing projects.
 */
#define MAX_INTERRUPT_SOURCES       7

/**
 * Describes a PIO interrupt source, including the PIO instance triggering the
 * interrupt and the associated interrupt handler.
 */
struct s_interrupt_source {
	uint32_t id;
	uint32_t mask;
	uint32_t attr;

	/* Interrupt handler. */
	void (*handler) (const uint32_t, const uint32_t);
};


/* List of interrupt sources. */
static struct s_interrupt_source gs_interrupt_sources[MAX_INTERRUPT_SOURCES];

/* Number of currently defined interrupt sources. */
static uint32_t gs_ul_nb_sources = 0;

#if (SAM3S || SAM4S || SAM4E)
/* PIO Capture handler */
static void (*pio_capture_handler)(Pio *) = NULL;
extern uint32_t pio_capture_enable_flag;
#endif

/**
 * \brief Process an interrupt request on the given PIO controller.
 *
 * \param p_pio PIO controller base address.
 * \param ul_id PIO controller ID.
 */
void pio_handler_process(Pio *p_pio, uint32_t ul_id)
{
	uint32_t status;
	uint32_t i;

	/* Read PIO controller status */
	status = pio_get_interrupt_status(p_pio);
	status &= pio_get_interrupt_mask(p_pio);

	/* Check pending events */
	if (status != 0) {
		/* Find triggering source */
		i = 0;
		while (status != 0) {
			/* Source is configured on the same controller */
			if (gs_interrupt_sources[i].id == ul_id) {
				/* Source has PIOs whose statuses have changed */
				if ((status & gs_interrupt_sources[i].mask) != 0) {
					gs_interrupt_sources[i].handler(gs_interrupt_sources[i].id,
							gs_interrupt_sources[i].mask);
					status &= ~(gs_interrupt_sources[i].mask);
				}
			}
			i++;
			if (i >= MAX_INTERRUPT_SOURCES) {
				break;
			}
		}
	}

	/* Check capture events */
#if (SAM3S || SAM4S || SAM4E)
	if (pio_capture_enable_flag) {
		if (pio_capture_handler) {
			pio_capture_handler(p_pio);
		}
	}
#endif
}

/**
 * \brief Set an interrupt handler for the provided pins.
 * The provided handler will be called with the triggering pin as its parameter
 * as soon as an interrupt is detected.
 *
 * \param p_pio PIO controller base address.
 * \param ul_id PIO ID.
 * \param ul_mask Pins (bit mask) to configure.
 * \param ul_attr Pins attribute to configure.
 * \param p_handler Interrupt handler function pointer.
 *
 * \return 0 if successful, 1 if the maximum number of sources has been defined.
 */
uint32_t pio_handler_set(Pio *p_pio, uint32_t ul_id, uint32_t ul_mask,
		uint32_t ul_attr, void (*p_handler) (uint32_t, uint32_t))
{
    uint8_t i;
	struct s_interrupt_source *pSource;

	if (gs_ul_nb_sources >= MAX_INTERRUPT_SOURCES)
		return 1;

    /* Check interrupt for this pin, if already defined, redefine it. */
	for (i = 0; i <= gs_ul_nb_sources; i++) {
		pSource = &(gs_interrupt_sources[i]);
		if (pSource->id == ul_id && pSource->mask == ul_mask) {
			break;
		}
	}

	/* Define new source */
	pSource->id = ul_id;
	pSource->mask = ul_mask;
	pSource->attr = ul_attr;
	pSource->handler = p_handler;
	if (i == gs_ul_nb_sources + 1) {
		gs_ul_nb_sources++;
	}

	/* Configure interrupt mode */
	pio_configure_interrupt(p_pio, ul_mask, ul_attr);

	return 0;
}

#if (SAM3S || SAM4S || SAM4E)
/**
 * \brief Set a capture interrupt handler for all PIO.
 *
 * The handler will be called with the triggering PIO as its parameter
 * as soon as an interrupt is detected.
 *
 * \param p_handler Interrupt handler function pointer.
 *
 */
void pio_capture_handler_set(void (*p_handler)(Pio *))
{
	pio_capture_handler = p_handler;
}
#endif

#ifdef ID_PIOA
/**
 * \brief Set an interrupt handler for the specified pin.
 * The provided handler will be called with the triggering pin as its parameter
 * as soon as an interrupt is detected.
 *
 * \param ul_pin Pin index to configure.
 * \param ul_flag Pin flag.
 * \param p_handler Interrupt handler function pointer.
 *
 * \return 0 if successful, 1 if the maximum number of sources has been defined.
 */
uint32_t pio_handler_set_pin(uint32_t ul_pin, uint32_t ul_flag,
		void (*p_handler) (uint32_t, uint32_t))
{
	Pio *p_pio = pio_get_pin_group(ul_pin);
	uint32_t group_id =  pio_get_pin_group_id(ul_pin);
	uint32_t group_mask = pio_get_pin_group_mask(ul_pin);

	return pio_handler_set(p_pio, group_id, group_mask, ul_flag, p_handler);
}

/**
 * \brief Parallel IO Controller A interrupt handler.
 * Redefined PIOA interrupt handler for NVIC interrupt table.
 */
void PIOA_Handler(void)
{
	pio_handler_process(PIOA, ID_PIOA);
}
#endif

#ifdef ID_PIOB
/**
 * \brief Parallel IO Controller B interrupt handler
 * Redefined PIOB interrupt handler for NVIC interrupt table.
 */
void PIOB_Handler(void)
{
    pio_handler_process(PIOB, ID_PIOB);
}
#endif

#ifdef ID_PIOC
/**
 * \brief Parallel IO Controller C interrupt handler.
 * Redefined PIOC interrupt handler for NVIC interrupt table.
 */
void PIOC_Handler(void)
{
	pio_handler_process(PIOC, ID_PIOC);
}
#endif

#ifdef ID_PIOD
/**
 * \brief Parallel IO Controller D interrupt handler.
 * Redefined PIOD interrupt handler for NVIC interrupt table.
 */
void PIOD_Handler(void)
{
	pio_handler_process(PIOD, ID_PIOD);
}
#endif

#ifdef ID_PIOE
/**
 * \brief Parallel IO Controller E interrupt handler.
 * Redefined PIOE interrupt handler for NVIC interrupt table.
 */
void PIOE_Handler(void)
{
	pio_handler_process(PIOE, ID_PIOE);
}
#endif

#ifdef ID_PIOF
/**
 * \brief Parallel IO Controller F interrupt handler.
 * Redefined PIOF interrupt handler for NVIC interrupt table.
 */
void PIOF_Handler(void)
{
	pio_handler_process(PIOF, ID_PIOF);
}
#endif

/**
 * \brief Initialize PIO interrupt management logic.
 *
 * \param p_pio PIO controller base address.
 * \param ul_irqn NVIC line number.
 * \param ul_priority PIO controller interrupts priority.
 */
void pio_handler_set_priority(Pio *p_pio, IRQn_Type ul_irqn, uint32_t ul_priority)
{
	uint32_t bitmask = 0;

	bitmask = pio_get_interrupt_mask(p_pio);
	pio_disable_interrupt(p_pio, 0xFFFFFFFF);
	pio_get_interrupt_status(p_pio);
	NVIC_DisableIRQ(ul_irqn);
	NVIC_ClearPendingIRQ(ul_irqn);
	NVIC_SetPriority(ul_irqn, ul_priority);
	NVIC_EnableIRQ(ul_irqn);
	pio_enable_interrupt(p_pio, bitmask);
}
