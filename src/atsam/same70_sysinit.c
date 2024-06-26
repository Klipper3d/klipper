// This code is from lib/sam4e/gcc/system_sam4e.c and modified for the SAM E70

#include "internal.h"

/* Clock Settings (300MHz) */
#define SYS_BOARD_OSCOUNT   (CKGR_MOR_MOSCXTST(0x8U))
#define SYS_BOARD_PLLAR     (CKGR_PLLAR_ONE                             \
                             | CKGR_PLLAR_MULA(0x18U)                   \
                             | CKGR_PLLAR_PLLACOUNT(0x3fU)              \
                             | CKGR_PLLAR_DIVA_BYPASS)
#define SYS_BOARD_MCKR      (PMC_MCKR_MDIV_PCK_DIV2 | PMC_MCKR_CSS_PLLA_CLK)

/* Key to unlock MOR register */
#define SYS_CKGR_MOR_KEY_VALUE CKGR_MOR_KEY(0x37)

uint32_t SystemCoreClock = CHIP_FREQ_MAINCK_RC_12MHZ;

void SystemInit( void )
{
    /* Set FWS according to SYS_BOARD_MCKR configuration */
    EFC->EEFC_FMR = EEFC_FMR_FWS(6) | EEFC_FMR_CLOE;

    /* Initialize main oscillator */
    if ( !(PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) )
    {
        PMC->CKGR_MOR = (SYS_CKGR_MOR_KEY_VALUE | SYS_BOARD_OSCOUNT
                         | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN);

        while ( !(PMC->PMC_SR & PMC_SR_MOSCXTS) )
        {
        }
    }

    /* Switch to 3-20MHz Xtal oscillator */
    PMC->CKGR_MOR = (SYS_CKGR_MOR_KEY_VALUE | SYS_BOARD_OSCOUNT
                     | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN
                     | CKGR_MOR_MOSCSEL);

    while ( !(PMC->PMC_SR & PMC_SR_MOSCSELS) )
    {
    }

    PMC->PMC_MCKR = ((PMC->PMC_MCKR & ~(uint32_t)PMC_MCKR_CSS_Msk)
                     | PMC_MCKR_CSS_MAIN_CLK);

    while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) )
    {
    }

    /* Initialize PLLA */
    PMC->CKGR_PLLAR = SYS_BOARD_PLLAR;
    while ( !(PMC->PMC_SR & PMC_SR_LOCKA) )
    {
    }

    /* Switch to main clock */
    PMC->PMC_MCKR = ((SYS_BOARD_MCKR & ~PMC_MCKR_CSS_Msk)
                     | PMC_MCKR_CSS_MAIN_CLK);
    while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) )
    {
    }

    /* Switch to PLLA */
    PMC->PMC_MCKR = SYS_BOARD_MCKR;
    while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) )
    {
    }

    SystemCoreClock = CHIP_FREQ_CPU_MAX;

    // Configure PCK6 for TC use
    PMC->PMC_PCK[6] = PMC_PCK_CSS_MCK | PMC_PCK_PRES(2);
    while ( !(PMC->PMC_SR & PMC_SR_PCKRDY6) )
    {
    }
    PMC->PMC_SCER |= PMC_SCER_PCK6;
}
