/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file hc32f460_exint_nmi_swi.c
 **
 ** A detailed description is available at
 ** @link ExintNmiSwiGroup Exint/Nmi/Swi description @endlink
 **
 **   - 2018-10-17 CDT First version for Device Driver Library of exint, Nmi, SW interrupt.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f460_exint_nmi_swi.h"
#include "hc32f460_utility.h"

/**
 *******************************************************************************
 ** \addtogroup ExintNmiSwiGroup
 ******************************************************************************/
//@{
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
 /*! Parameter validity check for external interrupt channel. */
#define IS_VALID_CH(x)                                                          \
(   ((x) == ExtiCh00)                           ||                              \
    ((x) == ExtiCh01)                           ||                              \
    ((x) == ExtiCh02)                           ||                              \
    ((x) == ExtiCh03)                           ||                              \
    ((x) == ExtiCh04)                           ||                              \
    ((x) == ExtiCh05)                           ||                              \
    ((x) == ExtiCh06)                           ||                              \
    ((x) == ExtiCh07)                           ||                              \
    ((x) == ExtiCh08)                           ||                              \
    ((x) == ExtiCh09)                           ||                              \
    ((x) == ExtiCh10)                           ||                              \
    ((x) == ExtiCh11)                           ||                              \
    ((x) == ExtiCh12)                           ||                              \
    ((x) == ExtiCh13)                           ||                              \
    ((x) == ExtiCh14)                           ||                              \
    ((x) == ExtiCh15))

/*! Parameter validity check for null pointer. */
#define IS_NULL_POINT(x) (NULL != (x))

/*! Parameter validity check for external interrupt trigger method. */
#define IS_VALID_LEVEL(x)                                                       \
(   ((x) == ExIntLowLevel)                      ||                              \
    ((x) == ExIntBothEdge)                      ||                              \
    ((x) == ExIntRisingEdge)                    ||                              \
    ((x) == ExIntFallingEdge))

/*! Parameter validity check for NMI interrupt source. */
#define IS_VALID_NMI_SRC(x)                                                     \
(   ((x) == NmiSrcNmi)                          ||                              \
    ((x) == NmiSrcSwdt)                         ||                              \
    ((x) == NmiSrcVdu1)                         ||                              \
    ((x) == NmiSrcVdu2)                         ||                              \
    ((x) == NmiSrcXtalStop)                     ||                              \
    ((x) == NmiSrcSramPE)                       ||                              \
    ((x) == NmiSrcSramDE)                       ||                              \
    ((x) == NmiSrcMpu)                          ||                              \
    ((x) == NmiSrcWdt))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static func_ptr_t pfnNmiCallback;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief External Int initialization
 **
 ** \param [in]  pstcExtiConfig         EXTI configure structure
 **
 ** \retval Ok                          EXTI initialized
 **
 ******************************************************************************/
en_result_t EXINT_Init(const stc_exint_config_t *pstcExtiConfig)
{
    stc_intc_eirqcr_field_t *EIRQCRx;

    DDL_ASSERT(IS_VALID_CH(pstcExtiConfig->enExitCh));

    EIRQCRx = (stc_intc_eirqcr_field_t *)((uint32_t)(&M4_INTC->EIRQCR0) +      \
                                          (uint32_t)(4ul * (uint32_t)(pstcExtiConfig->enExitCh)));

    /* Set filter function */
    EIRQCRx->EFEN = pstcExtiConfig->enFilterEn;
    EIRQCRx->EISMPCLK = pstcExtiConfig->enFltClk;

    /* Set detection level */
    EIRQCRx->EIRQTRG = pstcExtiConfig->enExtiLvl;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get External interrupt request flag
 **
 ** \param [in]  enExint                NMI Int source, This parameter can be
 **                                     any value of @ref en_exti_ch_t
 **
 ** \retval Set                         Corresponding Ex.Int request flag be set
 **         Reset                       Corresponding Ex.Int request flag not be set
 **
 ******************************************************************************/
en_int_status_t EXINT_IrqFlgGet(en_exti_ch_t enExint)
{
    en_int_status_t enRet;
    DDL_ASSERT(IS_VALID_CH(enExint));

    enRet = (1u == !!(M4_INTC->EIFR & (1ul<<enExint)) ? Set : Reset);

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Clear External interrupt request flag
 **
 ** \param [in]  enExint                Ext. interrupt channel, This parameter can be
 **                                     any value of @ref en_exti_ch_t
 **
 ** \retval Ok                          Interrupt source be cleared
 **
 ******************************************************************************/
en_result_t EXINT_IrqFlgClr(en_exti_ch_t enExint)
{
    M4_INTC->EICFR |= (uint32_t)(1ul << enExint);
    return Ok;
}

/**
 *******************************************************************************
 ** \brief NMI initialization
 **
 ** \param [in]  pstcNmiConfig          NMI configure structure
 **
 ** \retval Ok                          NMI initialized
 **         ErrorInvalidParameter       NMI configuration pointer is null
 **
 ******************************************************************************/
en_result_t NMI_Init(const stc_nmi_config_t *pstcNmiConfig)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pstcNmiConfig)
    {
        /* NMI callback function  */
        pfnNmiCallback = pstcNmiConfig->pfnNmiCallback;
        /* Set filter function */
        M4_INTC->NMICR_f.NFEN = pstcNmiConfig->enFilterEn;
        /* Set filter clock */
        M4_INTC->NMICR_f.NSMPCLK = pstcNmiConfig->enFilterClk;
        /* Set detection level */
        M4_INTC->NMICR_f.NMITRG = pstcNmiConfig->enNmiLvl;
        /* Set NMI source */
        M4_INTC->NMIENR = (uint32_t)pstcNmiConfig->u16NmiSrc;
        enRet = Ok;
    }
    return enRet;
}

/**
 *******************************************************************************
 ** \brief De-Init Non-Maskable Interrupt (NMI)
 **
 ** \param  None
 **
 ** \retval Ok                          NMI De-initialized
 **
 ******************************************************************************/
en_result_t NMI_DeInit(void)
{
    /* Set internal data */
    pfnNmiCallback = NULL;

    /* clear NMI control register */
    M4_INTC->NMICR = 0u;

    /* clear NMI enable register */
    M4_INTC->NMIENR = 0u;

    /* clear all NMI flags */
    M4_INTC->NMIFR = 0u;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get NMI interrupt request flag
 **
 ** \param [in]  enNmiSrc               NMI Int source, This parameter can be
 **                                     any value of @ref en_nmi_src_t
 **
 ** \retval Set                         Corresponding NMI flag be set
 **         Reset                       Corresponding NMI flag not be set
 **
 ******************************************************************************/
en_int_status_t NMI_IrqFlgGet(en_nmi_src_t enNmiSrc)
{
    DDL_ASSERT(IS_VALID_NMI_SRC(enNmiSrc));

    en_int_status_t enRet = Reset;
    switch (enNmiSrc)
    {
        case NmiSrcNmi:
            enRet = (en_int_status_t)(M4_INTC->NMIFR_f.NMIFR);
            break;
        case NmiSrcSwdt:
            enRet = (en_int_status_t)(M4_INTC->NMIFR_f.SWDTFR);
            break;
        case NmiSrcVdu1:
            enRet = (en_int_status_t)(M4_INTC->NMIFR_f.PVD1FR);
            break;
        case NmiSrcVdu2:
            enRet = (en_int_status_t)(M4_INTC->NMIFR_f.PVD2FR);
            break;
        case NmiSrcXtalStop:
            enRet = (en_int_status_t)(M4_INTC->NMIFR_f.XTALSTPFR);
            break;
        case NmiSrcSramPE:
            enRet = (en_int_status_t)(M4_INTC->NMIFR_f.REPFR);
            break;
        case NmiSrcSramDE:
            enRet = (en_int_status_t)(M4_INTC->NMIFR_f.RECCFR);
            break;
        case NmiSrcMpu:
            enRet = (en_int_status_t)(M4_INTC->NMIFR_f.BUSMFR);
            break;
        case NmiSrcWdt:
            enRet = (en_int_status_t)(M4_INTC->NMIFR_f.WDTFR);
            break;
        default:
            break;
    }
    return enRet;
}

/**
 *******************************************************************************
 ** \brief Clear NMI interrupt request flag
 **
 ** \param [in]  u16NmiSrc              NMI Int source, This parameter can be
 **                                     any composited value of @ref en_nmi_src_t
 **
 ** \retval Ok                          Interrupt source be cleared
 **
 ******************************************************************************/
en_result_t NMI_IrqFlgClr(uint16_t u16NmiSrc)
{
    M4_INTC->NMICFR |= u16NmiSrc;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief ISR for NMI
 **
 ******************************************************************************/
void NMI_IrqHandler(void)
{
    DDL_ASSERT(IS_NULL_POINT(pfnNmiCallback));

    pfnNmiCallback();
}

/**
 *******************************************************************************
 ** \brief Enable Softeware Interrupt (SWI)
 **
 *  \param [in]  u32SwiCh               This parameter can be any composited
 *                                      value of @ref en_swi_ch_t
 **
 ** \retval Ok                          SWI initialized
 **
 ******************************************************************************/
en_result_t SWI_Enable(uint32_t u32SwiCh)
{
    M4_INTC->SWIER |= u32SwiCh;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief De-Init Softeware Interrupt (SWI)
 **
 *  \param [in]  u32SwiCh               This parameter can be any composited
 *                                      value of @ref en_swi_ch_t
 **
 ** \retval Ok                          SWI de-initialized
 **
 ******************************************************************************/
en_result_t SWI_Disable(uint32_t u32SwiCh)
{
    /* clear software interrupt enable register */
    M4_INTC->SWIER &=  ~u32SwiCh;

    return Ok;
}

//@} // ExintNmiSwiGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
