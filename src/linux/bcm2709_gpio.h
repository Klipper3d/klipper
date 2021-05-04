// Efficient support via direct memory access for Raspberry Pi 2/3/4
//
// Copyright (C) 2021 Klaus Schwartz <klaus@eraga.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.


// Raspberry Pi 2 A+/B/B+
// Raspberry Pi 3 A+/B/B+
// Raspberry Pi Zero (W)

#ifndef KLIPPER_BCM2709_GPIO_H
#define KLIPPER_BCM2709_GPIO_H

//#define GPIO_PERI_BASE_OLD 0x20000000
#define GPIO_PERI_BASE 0x3F000000
#define GPIO_PULL *(gpio+37)

static uint8_t gpioToPUDCLK [] = {
        38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,
        38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,
        39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,
        39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,
};

#define GPIO_PULLCLK(g) *(gpio+gpioToPUDCLK[g])

#endif //KLIPPER_BCM2709_GPIO_H
