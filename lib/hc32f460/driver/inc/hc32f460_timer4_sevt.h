/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file hc32f460_timer4_sevt.h
 **
 ** A detailed description is available at
 ** @link Timer4SevtGroup Timer4SEVT description @endlink
 **
 **   - 2018-11-02 CDT First version for Device Driver Library of Timer4SEVT.
 **
 ******************************************************************************/
#ifndef __HC32F460_TIMER4_SEVT_H__
#define __HC32F460_TIMER4_SEVT_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup Timer4SevtGroup Timer4 Special Event(Timer4SEVT)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Timer4 SEVT channel enumeration
 **
 ******************************************************************************/
typedef enum en_timer4_sevt_ch
{
    Timer4SevtCh0 = 0u,                 ///< Timer4 SEVT channel:0
    Timer4SevtCh1 = 1u,                 ///< Timer4 SEVT channel:1
    Timer4SevtCh2 = 2u,                 ///< Timer4 SEVT channel:2
    Timer4SevtCh3 = 3u,                 ///< Timer4 SEVT channel:3
    Timer4SevtCh4 = 4u,                 ///< Timer4 SEVT channel:4
    Timer4SevtCh5 = 5u,                 ///< Timer4 SEVT channel:5
} en_timer4_sevt_ch_t;

/**
 *******************************************************************************
 ** \brief Timer4 Special-EVT buffer type of SCCR and SCMR
 **
 ******************************************************************************/
typedef enum en_timer4_sevt_buf
{
    SevtBufDisable                        = 0u, ///< Disable Timer4 Special-EVT buffer function
    SevtBufCntZero                        = 1u, ///< Timer4 Special-EVT transfer buffer register of SCCR and SCMR when counter value of Cnt connected= 0x0000
    SevtBufCntPeak                        = 2u, ///< Timer4 Special-EVT transfer buffer register of SCCR and SCMR when counter value of Cnt connected= TCCP
    SevtBufCntZeroOrCntPeak               = 3u, ///< Timer4 Special-EVT transfer buffer register of SCCR and SCMR both when counter value of Cnt connected= 0x0000 and TCCP
    SevtBufCntZeroZicZero                 = 4u, ///< Timer4 Special-EVT transfer buffer register of SCCR and SCMR when counter value of Cnt connected= 0x0000 and zero value detection mask counter value is 0
    SevtBufCntPeakPicZero                 = 5u, ///< Timer4 Special-EVT transfer buffer register of SCCR and SCMR when counter value of Cnt connected= peak and peak value detection mask counter value is 0
    SevtBufCntZeroZicZeroOrCntPeakPicZero = 6u, ///< Timer4 Special-EVT transfer buffer register of SCCR and SCMR when counter value of Cnt connected= 0x0000 and zero value detection mask counter value is 0
                                                ///< or counter value of CNT connected= peak and peak value detection mask counter value is 0
} en_timer4_sevt_buf_t;

/**
 *******************************************************************************
 ** \brief Timer4 Special-EVT output trigger signal type
 **
 ******************************************************************************/
typedef enum en_timer4_sevt_trigger_evt
{
    SevtTrgEvtSCMUH = 0u,               ///< Timer4 Special-EVT Event: TMR4_Ux_SCMUH
    SevtTrgEvtSCMUL = 1u,               ///< Timer4 Special-EVT Event: TMR4_Ux_SCMUL
    SevtTrgEvtSCMVH = 2u,               ///< Timer4 Special-EVT Event: TMR4_Ux_SCMVH
    SevtTrgEvtSCMVL = 3u,               ///< Timer4 Special-EVT Event: TMR4_Ux_SCMVL
    SevtTrgEvtSCMWH = 4u,               ///< Timer4 Special-EVT Event: TMR4_Ux_SCMWH
    SevtTrgEvtSCMWL = 5u,               ///< Timer4 Special-EVT Event: TMR4_Ux_SCMWL
} en_timer4_sevt_trigger_evt_t;

/**
 *******************************************************************************
 ** \brief Timer4 Special-EVT OCCRx select type
 **
 ******************************************************************************/
typedef enum en_timer4_sevt_occr_sel
{
    SevtSelOCCRxh = 0u,                 ///< Select OCCRxh of high channel
    SevtSelOCCRxl = 1u,                 ///< Select OCCRxl of low channel
} en_timer4_sevt_occr_sel_t;

/**
 *******************************************************************************
 ** \brief Timer4 Special-EVT running mode
 **
 ******************************************************************************/
typedef enum en_timer4_sevt_mode
{
    SevtCompareTrigMode = 0u,           ///< Select Timer4 Special-EVT compare mode
    SevtDelayTrigMode   = 1u,           ///< Select Timer4 Special-EVT delay mode
} en_timer4_sevt_mode_t;

/**
 *******************************************************************************
 ** \brief Timer4 SEVT mask times enumeration
 **
 ******************************************************************************/
typedef enum en_timer4_sevt_mask
{
    Timer4SevtMask0  = 0u,              ///< Mask 0 time.
    Timer4SevtMask1  = 1u,              ///< Mask 1 times.
    Timer4SevtMask2  = 2u,              ///< Mask 2 times.
    Timer4SevtMask3  = 3u,              ///< Mask 3 times.
    Timer4SevtMask4  = 4u,              ///< Mask 4 times.
    Timer4SevtMask5  = 5u,              ///< Mask 5 times.
    Timer4SevtMask6  = 6u,              ///< Mask 6 times.
    Timer4SevtMask7  = 7u,              ///< Mask 7 times.
    Timer4SevtMask8  = 8u,              ///< Mask 8 times.
    Timer4SevtMask9  = 9u,              ///< Mask 9 times.
    Timer4SevtMask10 = 10u,             ///< Mask 10 times.
    Timer4SevtMask11 = 11u,             ///< Mask 11 times.
    Timer4SevtMask12 = 12u,             ///< Mask 12 times.
    Timer4SevtMask13 = 13u,             ///< Mask 13 times.
    Timer4SevtMask14 = 14u,             ///< Mask 14 times.
    Timer4SevtMask15 = 15u,             ///< Mask 15 times.
} en_timer4_sevt_mask_t;

/**
 *******************************************************************************
 ** \brief Timer4 Special-EVT trigger condition
 **
 ******************************************************************************/
typedef struct stc_timer4_sevt_trigger_cond
{
    en_functional_state_t enDownMatchCmd;   ///< Enable: Send trigger signal when match with SCCR&SCMR and Timer4CNT count down.
                                            ///< Disable: Don't send trigger signal when match with SCCR&SCMR and Timer4CNT count down.

    en_functional_state_t enPeakMatchCmd;   ///< Enable: Send trigger signal when match with SCCR&SCMR and Timer4CNT count peak.
                                            ///< Disable: Don't send trigger signal when match with SCCR&SCMR and Timer4CNT count peak.

    en_functional_state_t enUpMatchCmd;     ///< Enable: Send trigger signal when match with SCCR&SCMR and Timer4CNT count up.
                                            ///< Disable: Don't send trigger signal when match with SCCR&SCMR and Timer4CNT count up.

    en_functional_state_t enZeroMatchCmd;   ///< Enable: Send trigger signal when match with SCCR&SCMR and Timer4CNT count zero.
                                            ///< Disable: Don't send trigger signal when match with SCCR&SCMR and Timer4CNT count zero.
} stc_timer4_sevt_trigger_cond_t;

/**
 *******************************************************************************
 ** \brief Timer4 Special-EVT initialization configuration
 **
 ******************************************************************************/
typedef struct stc_timer4_sevt_init
{
    en_timer4_sevt_buf_t         enBuf;             ///< Configure Timer4 Special-EVT Buffer transfer type and this parameter can be a value of @ref en_timer4_sevt_buf_t

    en_timer4_sevt_mode_t        enMode;            ///< Configure Timer4 Special-EVT Running mode and this parameter can be a value of @ref en_timer4_sevt_mode_t

    en_timer4_sevt_occr_sel_t    enOccrSel;         ///< Select Timer4 Special-EVT Occr channel and this parameter can be a value of @ref en_timer4_sevt_occr_sel_t

    en_timer4_sevt_trigger_evt_t enTrigEvt;         ///< Configure Timer4 Special-EVT Trigger type and this parameter can be a value of @ref en_timer4_sevt_trigger_evt_t

    en_functional_state_t        enCmpAmcZicCmd;    ///< Enable:  Comparison is performed with the CNT zero interrupt mask counter.
                                                    ///< Disable: Comparison is not performed with the CNT zero interrupt mask counter.
    en_functional_state_t        enCmpAmcPicCmd;    ///< Enable:  Comparison is performed with the CNT peak interrupt mask counter.
                                                    ///< Disable: Comparison is not performed with the CNT peak interrupt mask counter.
    en_timer4_sevt_mask_t        enMaskTimes;       ///< Specifies start time by the value to be compared with the CNT interrupt mask counter. And this parameter can be a value of @ref en_timer4_sevt_mask_t
} stc_timer4_sevt_init_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
en_result_t TIMER4_SEVT_Init(M4_TMR4_TypeDef *TMR4x,
                                en_timer4_sevt_ch_t enCh,
                                const stc_timer4_sevt_init_t *pstcInitCfg);
en_result_t TIMER4_SEVT_DeInit(M4_TMR4_TypeDef *TMR4x,
                                en_timer4_sevt_ch_t enCh);
en_result_t TIMER4_SEVT_SetTriggerEvent(M4_TMR4_TypeDef *TMR4x,
                                en_timer4_sevt_ch_t enCh,
                                en_timer4_sevt_trigger_evt_t enTrgEvt);
en_result_t TIMER4_SEVT_SetTriggerCond(M4_TMR4_TypeDef *TMR4x,
                                en_timer4_sevt_ch_t enCh,
                                const stc_timer4_sevt_trigger_cond_t *pstcTrigCond);
en_result_t TIMER4_SEVT_WriteSCCR(M4_TMR4_TypeDef *TMR4x,
                                en_timer4_sevt_ch_t enCh,
                                uint16_t u16SccrVal);
uint16_t TIMER4_SEVT_ReadSCCR(M4_TMR4_TypeDef *TMR4x,
                                en_timer4_sevt_ch_t enCh);

//@} // Timer4SevtGroup

#ifdef __cplusplus
}
#endif

#endif /* __HC32F460_TIMER4_SEVT_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
