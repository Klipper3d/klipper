// Code to setup peripheral clocks on the SAMD21
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "compiler.h" // DIV_ROUND_CLOSEST
#include "internal.h" // enable_pclock
#include "samd21.h" // GCLK

void
enable_pclock(uint32_t clock_id, uint32_t pmask)
{
    GCLK->CLKCTRL.reg = (GCLK_CLKCTRL_ID(clock_id) | GCLK_CLKCTRL_GEN_GCLK0
                         | GCLK_CLKCTRL_CLKEN);
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
        ;
    PM->APBCMASK.reg |= pmask;
}

#define CLK_MAIN 0
#define CLK_32K 1
#define MCLK_DFLL48M 0

#define CLK_32K_FREQ 32768

void
SystemInit(void)
{
    // Setup flash to work with 48Mhz clock
    NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_RWS_HALF;

    // Enable external 32Khz crystal
    uint32_t val = (SYSCTRL_XOSC32K_STARTUP(6)
                    | SYSCTRL_XOSC32K_XTALEN | SYSCTRL_XOSC32K_EN32K);
    SYSCTRL->XOSC32K.reg = val;
    SYSCTRL->XOSC32K.reg = val | SYSCTRL_XOSC32K_ENABLE;
    while (!(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_XOSC32KRDY))
        ;

    // Reset GCLK
    GCLK->CTRL.reg = GCLK_CTRL_SWRST;
    while (GCLK->CTRL.reg & GCLK_CTRL_SWRST)
        ;

    // Route 32Khz clock to DFLL48M
    GCLK->GENDIV.reg = GCLK_GENDIV_ID(CLK_32K);
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_ID(CLK_32K)
                         | GCLK_GENCTRL_SRC_XOSC32K | GCLK_GENCTRL_GENEN);
    GCLK->CLKCTRL.reg = (GCLK_CLKCTRL_ID(MCLK_DFLL48M)
                         | GCLK_CLKCTRL_GEN(CLK_32K) | GCLK_CLKCTRL_CLKEN);

    // Configure DFLL48M clock
    SYSCTRL->DFLLCTRL.reg = 0;
    uint32_t mul = DIV_ROUND_CLOSEST(CONFIG_CLOCK_FREQ, CLK_32K_FREQ);
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
    GCLK->GENDIV.reg = GCLK_GENDIV_ID(CLK_MAIN);
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_ID(CLK_MAIN)
                         | GCLK_GENCTRL_SRC_DFLL48M
                         | GCLK_GENCTRL_IDC | GCLK_GENCTRL_GENEN);
}
