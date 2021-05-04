// Efficient support via direct memory access for Raspberry Pi 2/3/4
//
// Copyright (C) 2021 Klaus Schwartz <klaus@eraga.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.


// Raspberry Pi 4

#ifndef KLIPPER_GPIO_BCM2835_H
#define KLIPPER_GPIO_BCM2835_H

#define GPIO_PERI_BASE 0xFE000000
#define GPIO_PULL *(gpio+57)

#endif //KLIPPER_GPIO_BCM2835_H
