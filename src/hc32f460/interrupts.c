// Interrupt support for HC32F460
// The library interrupt support is huge and redefines systick
//
// Copyright (C) 2022  Steven Gotthardt <gotthardt@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "hc32f460.h"

#define IRQ_PRIORITY_DEFAULT 15u

/* the interrupts on the hc32f460 can be 're-assigned'
    The author can choose the irqType (IRQ000_Handler, etc...)
     that the source (irqSrc) triggers
     */

void IrqRegistration(en_int_src_t irqSrc, IRQn_Type irqType)
{
    stc_intc_sel_field_t *stcIntSel = (stc_intc_sel_field_t *)
        ((uint32_t)(&M4_INTC->SEL0) + (4u * irqType));

    // what is the source of the selected interrupt? (USART, etc...)
    stcIntSel->INTSEL = irqSrc;

    // set priority and enable
    NVIC_SetPriority(irqType, IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(irqType);
    NVIC_EnableIRQ(irqType);
}
