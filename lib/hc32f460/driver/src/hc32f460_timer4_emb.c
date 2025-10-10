/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file hc32f460_timer4_emb.c
 **
 ** A detailed description is available at
 ** @link Timer4EmbGroup Timer4EMB description @endlink
 **
 **   - 2018-11-02 CDT First version for Device Driver Library of Timer4EMB.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f460_timer4_emb.h"
#include "hc32f460_utility.h"

/**
 *******************************************************************************
 ** \addtogroup Timer4EmbGroup
 ******************************************************************************/

//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*!< Parameter validity check for Timer4 unit  */
#define IS_VALID_TIMER4(__TMRx__)                                              \
(   (M4_TMR41 == (__TMRx__))                ||                                 \
    (M4_TMR42 == (__TMRx__))                ||                                 \
    (M4_TMR43 == (__TMRx__)))

/*!< Parameter valid check for EMB HOLD mode. */
#define IS_VALID_EMB_HOLD_MODE(x)                                              \
(   (EmbHoldPwm == (x))                     ||                                 \
    (EmbChangePwm == (x)))

/*!< Parameter valid check for EMB state. */
#define IS_VALID_EMB_STATE(x)                                                  \
(   (EmbTrigPwmOutputHiz == (x))            ||                                 \
    (EmbTrigPwmOutputNormal == (x))         ||                                 \
    (EmbTrigPwmOutputLowLevel == (x))       ||                                 \
    (EmbTrigPwmOutputHighLevel == (x)))

/*!< Timer4x ECER register address. */
#define TMR4_ECERx(__TMRx__)                                                   \
( (M4_TMR41 == (__TMRx__)) ? &M4_TMR4_CR->ECER1 :                              \
         ((M4_TMR42 == (__TMRx__)) ? &M4_TMR4_CR->ECER2 : &M4_TMR4_CR->ECER3))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief Initialize Timer4 EMB
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] pstcInitCfg             The pointer of EMB configure structure
 ** \arg This parameter detail refer @ref stc_timer4_emb_init_t
 **
 ** \retval Ok                          Initialize successfully
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - TMR4x is invalid
 **                                     - pstcInitCfg == NULL
 **
 ******************************************************************************/
en_result_t TIMER4_EMB_Init(M4_TMR4_TypeDef *TMR4x,
                                const stc_timer4_emb_init_t *pstcInitCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x && pstcInitCfg pointer */
    if ((IS_VALID_TIMER4(TMR4x)) && (NULL != pstcInitCfg))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_EMB_STATE(pstcInitCfg->enEmbState));
        DDL_ASSERT(IS_VALID_EMB_HOLD_MODE(pstcInitCfg->enPwmHold));

        /* Set EMB HOLD mode */
        TMR4x->ECSR_f.HOLD = (uint16_t)(pstcInitCfg->enPwmHold);

        /* Set EMB state */
        *(__IO uint32_t *)TMR4_ECERx(TMR4x) = (uint32_t)(pstcInitCfg->enEmbState);

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief De-initialize Timer4 EMB
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 **
 ** \retval Ok                          De-Initialize successfully
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_EMB_DeInit(M4_TMR4_TypeDef *TMR4x)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Set reset value(0x0000) to register ESCR */
        TMR4x->ECSR = 0u;

        /* Set reset value(0x0000) to register ECER */
        *(__IO uint32_t *)TMR4_ECERx(TMR4x) = (uint32_t)0ul;

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set Timer4 EMB HOLD mode
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enHoldMode              EMB HOLD mode
 ** \arg EmbChangePwm                   Don't hold PWM output when EMB signal occurs
 ** \arg EmbHoldPwm                     Hold PWM output when EMB signal occurs
 **
 ** \retval Ok                          Set successfully
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_EMB_SetHoldMode(M4_TMR4_TypeDef *TMR4x,
                                en_timer4_emb_hold_mode_t enHoldMode)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_EMB_HOLD_MODE(enHoldMode));

        /* Set EMB HOLD mode */
        TMR4x->ECSR_f.HOLD = (uint16_t)enHoldMode;

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Timer4 EMB HOLD mode
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 **
 ** \retval EmbChangePwm                Don't hold PWM output when EMB signal occurs
 ** \retval EmbHoldPwm                  Hold PWM output when EMB signal occurs
 **
 ******************************************************************************/
en_timer4_emb_hold_mode_t TIMER4_EMB_GetHoldMode(M4_TMR4_TypeDef *TMR4x)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));

    return (en_timer4_emb_hold_mode_t)(TMR4x->ECSR_f.HOLD);
}

/**
 *******************************************************************************
 ** \brief Set Timer4 EMB state
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enEmbState              EMB state
 ** \arg EmbTrigPwmOutputNormal         PWM output signal normally.
 ** \arg EmbTrigPwmOutputHiz            PWM output Hiz signal.
 ** \arg EmbTrigPwmOutputLowLevel       PWM output low level signal.
 ** \arg EmbTrigPwmOutputHighLevel      PWM output high level signal.
 **
 ** \retval Ok                          Set successfully
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_EMB_SetState(const M4_TMR4_TypeDef *TMR4x,
                                en_timer4_emb_state_t enEmbState)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_EMB_STATE(enEmbState));

        /* Set EMB state */
        *(__IO uint32_t *)TMR4_ECERx(TMR4x) = (uint32_t)enEmbState;

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Timer4 EMB state
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 **
 ** \retval EmbTrigPwmOutputNormal      PWM output signal normally.
 ** \retval EmbTrigPwmOutputHiz         PWM output Hiz signal.
 ** \retval EmbTrigPwmOutputLowLevel    PWM output low level signal.
 ** \retval EmbTrigPwmOutputHighLevel   PWM output high level signal.
 **
 ******************************************************************************/
en_timer4_emb_state_t TIMER4_EMB_GetState(const M4_TMR4_TypeDef *TMR4x)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));

    return *(__IO en_timer4_emb_state_t *)TMR4_ECERx(TMR4x);
}

//@} // Timer4EmbGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
