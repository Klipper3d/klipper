// Code to setup peripheral clocks on the SAMD21
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "compiler.h" // DIV_ROUND_CLOSEST
#include "internal.h" // enable_pclock
#include "samd21.h" // GCLK

// The "generic clock generators" that are configured
#define CLKGEN_MAIN 0
#define CLKGEN_32K 1
#define CLKGEN_ULP32K 2

#define FREQ_MAIN 48000000
#define FREQ_32K 32768

// Configure a clock generator using a given source as input
static inline void
gen_clock(uint32_t clkgen_id, uint32_t flags)
{
    GCLK->GENDIV.reg = GCLK_GENDIV_ID(clkgen_id);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(clkgen_id) | flags | GCLK_GENCTRL_GENEN;
}

// Route a peripheral clock to a given clkgen
static inline void
route_pclock(uint32_t pclk_id, uint32_t clkgen_id)
{
    GCLK->CLKCTRL.reg = (GCLK_CLKCTRL_ID(pclk_id)
                         | GCLK_CLKCTRL_GEN(clkgen_id) | GCLK_CLKCTRL_CLKEN);
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
        ;
}

// Enable a peripheral clock and power to that peripheral
void
enable_pclock(uint32_t pclk_id, uint32_t pm_id)
{
    route_pclock(pclk_id, CLKGEN_MAIN);
    uint32_t pm_port = pm_id / 32, pm_bit = 1 << (pm_id % 32);
    (&PM->APBAMASK.reg)[pm_port] |= pm_bit;
}

// Return the frequency of the given peripheral clock
uint32_t
get_pclock_frequency(uint32_t pclk_id)
{
    return FREQ_MAIN;
}

void
SystemInit(void)
{
    // Setup flash to work with 48Mhz clock
    NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_RWS_HALF;

    // Reset GCLK
    GCLK->CTRL.reg = GCLK_CTRL_SWRST;
    while (GCLK->CTRL.reg & GCLK_CTRL_SWRST)
        ;

    // Enable external 32Khz crystal and route to CLKGEN_32K
    uint32_t val = (SYSCTRL_XOSC32K_STARTUP(6)
                    | SYSCTRL_XOSC32K_XTALEN | SYSCTRL_XOSC32K_EN32K);
    SYSCTRL->XOSC32K.reg = val;
    SYSCTRL->XOSC32K.reg = val | SYSCTRL_XOSC32K_ENABLE;
    while (!(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_XOSC32KRDY))
        ;
    gen_clock(CLKGEN_32K, GCLK_GENCTRL_SRC_XOSC32K);

    // Configure DFLL48M clock (with CLKGEN_32K as reference)
    route_pclock(SYSCTRL_GCLK_ID_DFLL48, CLKGEN_32K);
    SYSCTRL->DFLLCTRL.reg = 0;
    uint32_t mul = DIV_ROUND_CLOSEST(FREQ_MAIN, FREQ_32K);
    SYSCTRL->DFLLMUL.reg = (SYSCTRL_DFLLMUL_CSTEP(31)
                            | SYSCTRL_DFLLMUL_FSTEP(511)
                            | SYSCTRL_DFLLMUL_MUL(mul));
    SYSCTRL->DFLLCTRL.reg = (SYSCTRL_DFLLCTRL_MODE | SYSCTRL_DFLLCTRL_WAITLOCK
                             | SYSCTRL_DFLLCTRL_QLDIS
                             | SYSCTRL_DFLLCTRL_ENABLE);
    uint32_t ready = (SYSCTRL_PCLKSR_DFLLRDY | SYSCTRL_PCLKSR_DFLLLCKC
                      | SYSCTRL_PCLKSR_DFLLLCKF);
    while ((SYSCTRL->PCLKSR.reg & ready) != ready)
        ;

    // Switch main clock to DFLL48M clock
    gen_clock(CLKGEN_MAIN, GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_IDC);
}
