// GPIO functions on lpc176x
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "LPC17xx.h" // LPC_PINCON
#include "internal.h" // gpio_peripheral
#include "board/irq.h" // irq_save

// Set the mode and extended function of a pin
void
gpio_peripheral(int bank, int pin, int func, int pullup)
{
    uint32_t bank_pos = bank * 2, pin_pos = pin * 2;
    if (pin_pos >= 32) {
        pin_pos -= 32;
        bank_pos++;
    }
    uint32_t sel_bits = (func & 0x03) << pin_pos, mask = ~(0x03 << pin_pos);
    uint32_t mode_bits = (pullup ? 0x00 : 0x02) << pin_pos;
    volatile uint32_t *pinsel = &LPC_PINCON->PINSEL0;
    volatile uint32_t *pinmode = &LPC_PINCON->PINMODE0;
    irqstatus_t flag = irq_save();
    pinsel[bank_pos] = (pinsel[bank_pos] & mask) | sel_bits;
    pinmode[bank_pos] = (pinmode[bank_pos] & mask) | mode_bits;
    irq_restore(flag);
}
