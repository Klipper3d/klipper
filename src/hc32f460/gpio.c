// GPIO functions on HC32F460
//
// Copyright (C) 2022  Steven Gotthardt <gotthardt@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h>     // ffs
#include "board/irq.h"  // irq_save
#include "command.h"    // DECL_ENUMERATION_RANGE
#include "board/gpio.h"       // gpio_out_setup
#include "internal.h"
#include "sched.h"      // sched_shutdown

#include "hc32f460_gpio.h"


// 64pin package
DECL_ENUMERATION_RANGE("pin", "PA0", GPIO('A', 0), 16);
DECL_ENUMERATION_RANGE("pin", "PB0", GPIO('B', 0), 16);
DECL_ENUMERATION_RANGE("pin", "PC0", GPIO('C', 0), 16);
DECL_ENUMERATION_RANGE("pin", "PD0", GPIO('D', 0), 16);
DECL_ENUMERATION_RANGE("pin", "PE0", GPIO('E', 0), 16);
DECL_ENUMERATION_RANGE("pin", "PH2", PortH * 16 + 2, 1);    // H: special case


// HC32F460 ports are in one M4_PORT - offset by 0x10
// eg toggle: M4_PORT->POTRA + 0x10 => M4_PORT->POTRB
// 'gpio' is port (0-4) * 16 + pinPosition (0-15)
#define POTR_OFFSET offsetof(M4_PORT_TypeDef, POTRA)    // output flip
#define PODR_OFFSET offsetof(M4_PORT_TypeDef, PODRA)    // output data
#define PIDR_OFFSET offsetof(M4_PORT_TypeDef, PIDRA)    // input data
#define POSR_OFFSET offsetof(M4_PORT_TypeDef, POSRA)    // output set
#define PORR_OFFSET offsetof(M4_PORT_TypeDef, PORRA)    // output reset
#define PORT_OFFSET offsetof(M4_PORT_TypeDef, PIDRB)    // space between PORTS



void
gpio_peripheral(uint32_t gpio, int func, int pull_up)
{
    stc_port_init_t stcPortInit;

    irqstatus_t flag = irq_save();

    stcPortInit.enPinMode       = func;
    stcPortInit.enLatch         = Disable;
    stcPortInit.enExInt         = Disable;
    stcPortInit.enInvert        = Disable;
    stcPortInit.enPullUp        = pull_up ? Enable : Disable;
    stcPortInit.enPinDrv        = Pin_Drv_L;
    stcPortInit.enPinOType      = Pin_OType_Cmos;
    stcPortInit.enPinSubFunc    = Disable;

    // make the port GPIO and disable the sub functionality
    PORT_SetFunc(GPIO2PORT(gpio), GPIO2BIT(gpio), Func_Gpio, Disable);
    PORT_Init(GPIO2PORT(gpio), GPIO2BIT(gpio), &stcPortInit);

    irq_restore(flag);
}


struct gpio_out
gpio_out_setup(uint32_t gpio, uint32_t val)
{
    uint32_t port = (uint32_t)M4_PORT + GPIO2PORT(gpio) * PORT_OFFSET;
    struct gpio_out g =
        { .gpio = gpio, .portAddress = port, .bitMask = GPIO2BIT(gpio) };
    gpio_out_reset(g, val);

    return g;
}


void
gpio_out_reset(struct gpio_out g, uint32_t val)
{
    irqstatus_t flag = irq_save();
    if (val)
    {
        uint16_t *POSRx = (uint16_t *)(g.portAddress + POSR_OFFSET);
        *POSRx = g.bitMask;
    }
    else
    {
        uint16_t *PORRx = (uint16_t *)(g.portAddress + PORR_OFFSET);
        *PORRx = g.bitMask;
    }

    gpio_peripheral(g.gpio, Pin_Mode_Out, 0);
    irq_restore(flag);
}


void
gpio_out_toggle_noirq(struct gpio_out g)
{
    uint16_t *POTRx = (uint16_t *)(g.portAddress  + POTR_OFFSET);
    *POTRx = g.bitMask;
}


void
gpio_out_toggle(struct gpio_out g)
{
    irqstatus_t flag = irq_save();
    gpio_out_toggle_noirq(g);
    irq_restore(flag);
}


void
gpio_out_write(struct gpio_out g, uint32_t val)
{
    if (val)
    {
        uint16_t *POSRx = (uint16_t *)(g.portAddress + POSR_OFFSET);
        *POSRx = g.bitMask;
    }
    else
    {
        uint16_t *PORRx = (uint16_t *)(g.portAddress + PORR_OFFSET);
        *PORRx = g.bitMask;
    }
}


struct gpio_in
gpio_in_setup(uint32_t gpio, int32_t pull_up)
{
    uint32_t port = (uint32_t)M4_PORT + GPIO2PORT(gpio) * PORT_OFFSET;

    struct gpio_in g =
        { .gpio = gpio, .portAddress = port, .bitMask = GPIO2BIT(gpio) };
    gpio_in_reset(g, pull_up);

    return g;
}


void
gpio_in_reset(struct gpio_in g, int32_t pull_up)
{
    irqstatus_t flag = irq_save();
    gpio_peripheral(g.gpio, Pin_Mode_In, pull_up);
    irq_restore(flag);
}


uint8_t
gpio_in_read(struct gpio_in g)
{
    uint16_t *PIDRx = (uint16_t *)(g.portAddress + PIDR_OFFSET);
    return !!(*PIDRx & g.bitMask);
}
