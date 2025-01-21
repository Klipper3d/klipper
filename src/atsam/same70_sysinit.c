// This code is from lib/sam4e/gcc/system_sam4e.c and modified for the SAM E70

#include <string.h> // memset
#include "internal.h"

/* Clock Settings (300MHz) */
#define SYS_BOARD_OSCOUNT   (CKGR_MOR_MOSCXTST(0x8U))
#define SYS_BOARD_PLLAR     (CKGR_PLLAR_ONE                             \
                             | CKGR_PLLAR_MULA(0x18U)                   \
                             | CKGR_PLLAR_PLLACOUNT(0x3fU)              \
                             | CKGR_PLLAR_DIVA_BYPASS)
#define SYS_BOARD_MCKR      (PMC_MCKR_MDIV_PCK_DIV2 | PMC_MCKR_CSS_PLLA_CLK)
#define RST_PARAMS ((0xA5 << RSTC_CR_KEY_Pos) | RSTC_CR_PROCRST)
#define GPNVM_TCM_MASK ((1 << 7) | (1 << 8))

/* Key to unlock MOR register */
#define SYS_CKGR_MOR_KEY_VALUE CKGR_MOR_KEY(0x37)

extern uint32_t _text_size;
extern uint32_t _dtcm_bss_start, _dtcm_bss_end;

void
SystemInit( void )
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

    /* Configure PCK6 for TC use */
    PMC->PMC_PCK[6] = PMC_PCK_CSS_MCK | PMC_PCK_PRES(2);
    while ( !(PMC->PMC_SR & PMC_SR_PCKRDY6) )
    {
    }
    PMC->PMC_SCER |= PMC_SCER_PCK6;

    /* Check Tightly Coupled Memory (TCM) bits. */
    EFC->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_GGPB);
    while ((EFC->EEFC_FSR & EEFC_FSR_FRDY) == 0)
        ;
    if ((EFC->EEFC_FRR & GPNVM_TCM_MASK) != GPNVM_TCM_MASK)
    {
        /* Configure TCM sizes to 128KiB (set GPNVM7 and GPNVM8). */
        EFC->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_SGPB
                        | EEFC_FCR_FARG(7));
        while ((EFC->EEFC_FSR & EEFC_FSR_FRDY) == 0)
            ;
        EFC->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD| EEFC_FCR_FCMD_SGPB
                        | EEFC_FCR_FARG(8));
        while ((EFC->EEFC_FSR & EEFC_FSR_FRDY) == 0)
            ;
        /* Reboot required, but bits are set now and we will not
         * return down this path. */
        __DSB();
        __ISB();
        RSTC->RSTC_CR = RST_PARAMS;
        for (;;)
            ;
    }

    /* Clear Data Tightly Coupled Memory (DTCM) bss segment - this has to happen
     *  after we check that the DTCM is enabled. */
    memset(&_dtcm_bss_start, 0, (&_dtcm_bss_end - &_dtcm_bss_start) * 4);

    /* DMA copy flash to Instruction Tightly Coupled Memory (ITCM). Just use
     *  channel 0 since we have not done anything yet. */
    enable_pclock(ID_XDMAC);
    /* Clear pending interrupts. */
    (void)REG_XDMAC_CIS0;
    REG_XDMAC_CSA0 = CONFIG_ARMCM_ITCM_FLASH_MIRROR_START;
    REG_XDMAC_CDA0 = CONFIG_FLASH_APPLICATION_ADDRESS;
    REG_XDMAC_CUBC0 = XDMAC_CUBC_UBLEN((int)&_text_size);

    REG_XDMAC_CC0 =
        XDMAC_CC_TYPE_MEM_TRAN | XDMAC_CC_MBSIZE_SINGLE |
        XDMAC_CC_CSIZE_CHK_1 | XDMAC_CC_DWIDTH_WORD |
        XDMAC_CC_SAM_INCREMENTED_AM | XDMAC_CC_DAM_INCREMENTED_AM |
        XDMAC_CC_SIF_AHB_IF1 | XDMAC_CC_DIF_AHB_IF0;

    REG_XDMAC_CNDA0 = 0;
    REG_XDMAC_CNDC0 = 0;
    REG_XDMAC_CBC0 = 0;
    REG_XDMAC_CDS_MSP0 = 0;
    REG_XDMAC_CSUS0 = 0;
    REG_XDMAC_CDUS0 = 0;

    REG_XDMAC_CIE0 = XDMAC_CIE_BIE;

    __DSB();
    __ISB();
    XDMAC->XDMAC_GE = XDMAC_GE_EN0;
    while ( XDMAC->XDMAC_GS & XDMAC_GS_ST0 )
        ;

    while ( !(REG_XDMAC_CIS0 & XDMAC_CIS_BIS) )
        ;

    /* Enable ITCM. DTCM is enabled by default. */
    __DSB();
    __ISB();
    SCB->ITCMCR = (SCB_ITCMCR_EN_Msk | SCB_ITCMCR_RMW_Msk
                    | SCB_ITCMCR_RETEN_Msk);
    __DSB();
    __ISB();

    /* Use data cache rather than DTCM for two reasons:
     *  1. It is hard to flash this device with GPNVM bits enabled.
     *  2. It is about as fast. */
    SCB_EnableDCache();
}
