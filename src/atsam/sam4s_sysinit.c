// This code is from lib/sam4e/gcc/system_sam4e.c - it is unclear why
// it is not defined in the Atmel sam4s cmsis code.

#include "internal.h"

/* Clock Settings (120MHz) */
#define SYS_BOARD_OSCOUNT   (CKGR_MOR_MOSCXTST(0x8U))
#define SYS_BOARD_PLLAR     (CKGR_PLLAR_ONE                             \
                             | CKGR_PLLAR_MULA(0x13U)                   \
                             | CKGR_PLLAR_PLLACOUNT(0x3fU)              \
                             | CKGR_PLLAR_DIVA(0x1U))
#define SYS_BOARD_MCKR      (PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK)

/* Key to unlock MOR register */
#define SYS_CKGR_MOR_KEY_VALUE CKGR_MOR_KEY(0x37)

uint32_t SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;

void SystemInit( void )
{
    /* Set FWS according to SYS_BOARD_MCKR configuration */
    EFC0->EEFC_FMR = EEFC_FMR_FWS(5) | EEFC_FMR_CLOE;

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
}
