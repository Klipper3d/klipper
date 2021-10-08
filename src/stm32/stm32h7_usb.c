// STM32 usb
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_USBSERIAL

DECL_CONSTANT_STR("RESERVE_PINS_USB", "PA11,PA12");
DECL_CONSTANT_STR("RESERVE_PINS_USB1", "PB14,PB15");

void init_stm32h7_usb2_fs(void) {
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_USB2OTGHSEN);

    while ((OTG->GRSTCTL & USB_OTG_GRSTCTL_AHBIDL) == 0);

    // Configure USB in full-speed device mode
    OTG->GUSBCFG = (USB_OTG_GUSBCFG_FDMOD | USB_OTG_GUSBCFG_PHYSEL
                    | (6 << USB_OTG_GUSBCFG_TRDT_Pos));
    OTGD->DCFG |= (3 << USB_OTG_DCFG_DSPD_Pos);
    OTG->GOTGCTL = USB_OTG_GOTGCTL_BVALOEN | USB_OTG_GOTGCTL_BVALOVAL;

    // Route pins
    gpio_peripheral(GPIO('A', 11), GPIO_FUNCTION(10), 0);
    gpio_peripheral(GPIO('A', 12), GPIO_FUNCTION(10), 0);
}

void init_stm32h7_usb1_fs(void) {
    // ADD USB1 HS device
}

void init_stm32h7_usb(void)
{
    if (READ_BIT(PWR->CR3, PWR_CR3_USB33RDY) != (PWR_CR3_USB33RDY) ? 1 : 0) {
        SET_BIT(PWR->CR3, PWR_CR3_USB33DEN);
    }

    init_stm32h7_usb2_fs();
}
