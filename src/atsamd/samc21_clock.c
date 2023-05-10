// Code to setup peripheral clocks on the SAMC21
//
// Copyright (C) 2018-2023  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2023  Luke Vuksta <wulfstawulfsta@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // DECL_CONSTANT_STR
#include "compiler.h" // DIV_ROUND_CLOSEST
#include "internal.h" // enable_pclock

// The "generic clock generators" that are configured
#define CLKGEN_MAIN 0

#define FREQ_MAIN CONFIG_CLOCK_FREQ
#define FREQ_48M 48000000

// Configure a clock generator using a given source as input
static inline void
gen_clock(uint32_t clkgen_id, uint32_t flags)
{
    GCLK->GENCTRL[clkgen_id].reg = flags | GCLK_GENCTRL_GENEN;
    while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(1 << clkgen_id))
        ;
}

// Route a peripheral clock to a given clkgen
static inline void
route_pclock(uint32_t pclk_id, uint32_t clkgen_id)
{
    uint32_t val = GCLK_PCHCTRL_GEN(clkgen_id) | GCLK_PCHCTRL_CHEN;
    // Don't do anything if already enabled (Timer Counter).
    if (GCLK->PCHCTRL[pclk_id].reg != val) {
        GCLK->PCHCTRL[pclk_id].reg = val;
        while (GCLK->PCHCTRL[pclk_id].reg != val)
            ;
    }
}

// Enable a peripheral clock and power to that peripheral
void
enable_pclock(uint32_t pclk_id, int32_t pm_id)
{
    route_pclock(pclk_id, CLKGEN_MAIN);
    if (pm_id >= 0) {
        uint32_t pm_port = pm_id / 32, pm_bit = 1 << (pm_id % 32);
        (&MCLK->APBAMASK.reg)[pm_port] |= pm_bit;
    }
}

// Return the frequency of the given peripheral clock
uint32_t
get_pclock_frequency(uint32_t pclk_id)
{
    return FREQ_MAIN;
}

// Configure a dpll to a given clock multiplier
static void
config_dpll(uint32_t mul, uint32_t ctrlb)
{
    OSCCTRL->DPLLCTRLA.reg = 0;
    while (OSCCTRL->DPLLSYNCBUSY.reg & OSCCTRL_DPLLSYNCBUSY_ENABLE)
        ;
    OSCCTRL->DPLLRATIO.reg = OSCCTRL_DPLLRATIO_LDR(mul - 1);
    while (OSCCTRL->DPLLSYNCBUSY.reg & OSCCTRL_DPLLSYNCBUSY_DPLLRATIO)
        ;
    OSCCTRL->DPLLCTRLB.reg = ctrlb | OSCCTRL_DPLLCTRLB_LBYPASS;
    OSCCTRL->DPLLCTRLA.reg = OSCCTRL_DPLLCTRLA_ENABLE;
    uint32_t mask = OSCCTRL_DPLLSTATUS_CLKRDY | OSCCTRL_DPLLSTATUS_LOCK;
    while ((OSCCTRL->DPLLSTATUS.reg & mask) != mask)
        ;
}

#if CONFIG_CLOCK_REF_X12M
DECL_CONSTANT_STR("RESERVE_PINS_crystal", "PA14,PA15");
#endif

// Initialize the clocks using an external 12M crystal
static void
clock_init_12m(void)
{
    // Enable XOSC1
    uint32_t freq_xosc = 12000000;
    uint32_t val = (OSCCTRL_XOSCCTRL_ENABLE | OSCCTRL_XOSCCTRL_XTALEN
                    | OSCCTRL_XOSCCTRL_GAIN(3) | OSCCTRL_XOSCCTRL_AMPGC);
    OSCCTRL->XOSCCTRL.reg = val;
    while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_XOSCRDY))
        ;

    // Generate 48Mhz clock on PLL (with XOSC as reference)
    uint32_t p0div = 24, p0mul = DIV_ROUND_CLOSEST(FREQ_MAIN, freq_xosc/p0div);
    uint32_t p0ctrlb = OSCCTRL_DPLLCTRLB_DIV(p0div / 2 - 1);
    config_dpll(p0mul, p0ctrlb | OSCCTRL_DPLLCTRLB_REFCLK(1));

    // Switch main clock to 48Mhz PLL
    gen_clock(CLKGEN_MAIN, GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DPLL96M_Val));
}


#if CONFIG_CLOCK_REF_X25M
DECL_CONSTANT_STR("RESERVE_PINS_crystal", "PA14,PA15");
#endif

// Initialize the clocks using an external 25M crystal
static void
clock_init_25m(void)
{
    // Enable XOSC1
    uint32_t freq_xosc = 25000000;
    uint32_t val = (OSCCTRL_XOSCCTRL_ENABLE | OSCCTRL_XOSCCTRL_XTALEN
                    | OSCCTRL_XOSCCTRL_GAIN(4) | OSCCTRL_XOSCCTRL_AMPGC);
    OSCCTRL->XOSCCTRL.reg = val;
    while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_XOSCRDY))
        ;

    // Generate 48Mhz clock on PLL (with XOSC as reference)
    uint32_t p0div = 50, p0mul = DIV_ROUND_CLOSEST(FREQ_MAIN, freq_xosc/p0div);
    uint32_t p0ctrlb = OSCCTRL_DPLLCTRLB_DIV(p0div / 2 - 1);
    config_dpll(p0mul, p0ctrlb | OSCCTRL_DPLLCTRLB_REFCLK(1));

    // Switch main clock to 48Mhz PLL
    gen_clock(CLKGEN_MAIN, GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DPLL96M_Val));
}

void
SystemInit(void)
{
    // Set NVM wait states for 48MHz. This might need to be set higher if
    // running at a lower voltage.
    NVMCTRL->CTRLB.reg |= (NVMCTRL_CTRLB_SLEEPPRM_WAKEUPINSTANT
                    | NVMCTRL_CTRLB_RWS(2));

    OSCCTRL->OSC48MDIV.reg = OSCCTRL_OSC48MDIV_DIV(0);
    while (OSCCTRL->OSC48MSYNCBUSY.reg & OSCCTRL_OSC48MSYNCBUSY_OSC48MDIV)
        ;

    // Reset GCLK
    GCLK->CTRLA.reg = GCLK_CTRLA_SWRST;
    while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_SWRST)
        ;

    // Init clocks
    if (CONFIG_CLOCK_REF_X12M)
        clock_init_12m();
    else
        clock_init_25m();
}
