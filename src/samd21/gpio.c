// samd21 gpio functions
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "internal.h" // gpio_peripheral
#include "samd21.h" // PORT

void
gpio_peripheral(char bank, uint32_t bit, char ptype, uint32_t pull_up)
{
    int group = bank == 'A' ? 0 : 1;
    PortGroup *pg = &PORT->Group[group];
    if (ptype) {
        volatile uint8_t *pmux = &pg->PMUX[bit/2].reg;
        uint8_t shift = (bit & 1) ? 4 : 0, mask = ~(0xf << shift);
        *pmux = (*pmux & mask) | ((ptype - 'A') << shift);
    }
    pg->PINCFG[bit].reg = ((ptype ? PORT_PINCFG_PMUXEN : 0)
                           | (pull_up ? PORT_PINCFG_PULLEN : 0));
}
