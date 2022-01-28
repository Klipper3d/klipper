// Code to setup peripheral clocks on the SAMD21
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // DECL_CONSTANT_STR
#include "compiler.h" // DIV_ROUND_CLOSEST
#include "internal.h" // enable_pclock

// The "generic clock generators" that are configured
#define CLKGEN_MAIN 0
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

#if CONFIG_CLOCK_REF_X32K
DECL_CONSTANT_STR("RESERVE_PINS_crystal", "PA0,PA1");
#endif

// Initialize the clocks using an external 32K crystal
static void
clock_init_32k(void)
{
    // Enable external 32Khz crystal
    uint32_t val = (SYSCTRL_XOSC32K_STARTUP(6)
                    | SYSCTRL_XOSC32K_XTALEN | SYSCTRL_XOSC32K_EN32K);
    SYSCTRL->XOSC32K.reg = val;
    SYSCTRL->XOSC32K.reg = val | SYSCTRL_XOSC32K_ENABLE;
    while (!(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_XOSC32KRDY))
        ;

    // Generate 48Mhz clock on DPLL (with XOSC32K as reference)
    SYSCTRL->DPLLCTRLA.reg = 0;
    uint32_t mul = DIV_ROUND_CLOSEST(FREQ_MAIN, FREQ_32K);
    SYSCTRL->DPLLRATIO.reg = SYSCTRL_DPLLRATIO_LDR(mul - 1);
    SYSCTRL->DPLLCTRLB.reg = SYSCTRL_DPLLCTRLB_LBYPASS;
    SYSCTRL->DPLLCTRLA.reg = SYSCTRL_DPLLCTRLA_ENABLE;
    uint32_t mask = SYSCTRL_DPLLSTATUS_CLKRDY | SYSCTRL_DPLLSTATUS_LOCK;
    while ((SYSCTRL->DPLLSTATUS.reg & mask) != mask)
        ;

    // Switch main clock to DPLL clock
    gen_clock(CLKGEN_MAIN, GCLK_GENCTRL_SRC_DPLL96M);
}

// Initialize clocks from factory calibrated internal clock
static void
clock_init_internal(void)
{
    // Configure DFLL48M clock in open loop mode
    SYSCTRL->DFLLCTRL.reg = 0;
    uint32_t coarse = GET_FUSE(FUSES_DFLL48M_COARSE_CAL);
    uint32_t fine = GET_FUSE(FUSES_DFLL48M_FINE_CAL);
    SYSCTRL->DFLLVAL.reg = (SYSCTRL_DFLLVAL_COARSE(coarse)
                            | SYSCTRL_DFLLVAL_FINE(fine));
    if (CONFIG_USBSERIAL) {
        // Enable USB clock recovery mode
        uint32_t mul = DIV_ROUND_CLOSEST(FREQ_MAIN, 1000);
        SYSCTRL->DFLLMUL.reg = (SYSCTRL_DFLLMUL_FSTEP(10)
                                | SYSCTRL_DFLLMUL_MUL(mul));
        SYSCTRL->DFLLCTRL.reg = (SYSCTRL_DFLLCTRL_MODE | SYSCTRL_DFLLCTRL_USBCRM
                                 | SYSCTRL_DFLLCTRL_CCDIS
                                 | SYSCTRL_DFLLCTRL_ENABLE);
    } else {
        SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE;
    }
    while (!(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY))
        ;

    // Switch main clock to DFLL48M clock
    gen_clock(CLKGEN_MAIN, GCLK_GENCTRL_SRC_DFLL48M);
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

    // Init clocks
    if (CONFIG_CLOCK_REF_X32K)
        clock_init_32k();
    else
        clock_init_internal();
}
