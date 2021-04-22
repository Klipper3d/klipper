//
// Created by Klaus Schwartz on 20/04/2021.
//
// Raspberry Pi 2 A+/B/B+
// Raspberry Pi 3 A+/B/B+
// Raspberry Pi Zero (W)

#ifndef KLIPPER_GPIO_BCM2835_H
#define KLIPPER_GPIO_BCM2835_H

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

#endif //KLIPPER_GPIO_BCM2835_H
