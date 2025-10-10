/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file hc32f460_i2s.h
 **
 ** A detailed description is available at
 ** @link I2sGroup Inter-IC Sound Bus description @endlink
 **
 **   - 2018-10-28  CDT  First version for Device Driver Library of I2S.
 **
 ******************************************************************************/

#ifndef __HC32F460_I2S_H__
#define __HC32F460_I2S_H__

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
 ** \defgroup I2sGroup Inter-IC Sound(I2S)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief I2S function
 ******************************************************************************/
typedef enum en_i2s_func
{
    TxEn = 0u,                              ///< Transfer enable function
    TxIntEn = 1u,                           ///< Transfer interrupt enable function
    RxEn = 2u,                              ///< receive enable function
    RxIntEn = 3u,                           ///< receive interrupt enable function
    ErrIntEn = 4u,                          ///< error interrupt enable function
}en_i2s_func_t;


/**
 *******************************************************************************
 ** \brief I2S status flag
 ******************************************************************************/
typedef enum en_i2s_std
{
    TxBufAlarmFlag = 0u,
    RxBufAlarmFlag = 1u,
    TxBufEmptFlag = 2u,
    TxBufFullFlag = 3u,
    RxBufEmptFlag = 4u,
    RxBufFullFlag = 5u,
}en_i2s_std_t;

/**
 *******************************************************************************
 ** \brief I2S clr flag
 ******************************************************************************/
typedef enum en_i2s_err_flag
{
    ClrTxErrFlag = 0u,
    ClrRxErrFlag = 1u,
}en_i2s_err_flag_t;
/**
 *******************************************************************************
 ** \brief I2S mode
 ******************************************************************************/
typedef enum en_i2s_mode
{
    I2sMaster = 0u,                     ///< I2S Master mode
    I2sSlave = 1u,                      ///< I2S Slave mode
}en_i2s_mode_t;

/**
 *******************************************************************************
 ** \brief I2S full duplex mode
 ******************************************************************************/
typedef enum en_i2s_full_duplex_mode
{
    I2s_HalfDuplex = 0u,                 ///< I2S half duplex
    I2s_FullDuplex = 1u,                 ///< I2S full duplex
}en_i2s_full_duplex_mode_t;

/**
 *******************************************************************************
 ** \brief I2S standard
 ******************************************************************************/
typedef enum en_i2s_standard
{
    Std_Philips = 0u,                   ///< I2S Philips standard
    Std_MSBJust = 1u,                   ///< I2S MSB justified standart
    Std_LSBJust = 2u,                   ///< I2S LSB justified standart
    Std_PCM     = 3u,                   ///< I2S PCM standart
}en_i2s_standard_t;

/**
 *******************************************************************************
 ** \brief I2S channel data length
 ******************************************************************************/
typedef enum en_i2s_ch_len
{
    I2s_ChLen_16Bit = 0u,
    I2s_ChLen_32Bit = 1u,
}en_i2s_ch_len_t;

/**
 *******************************************************************************
 ** \brief I2S data length
 ******************************************************************************/
typedef enum en_i2s_data_len
{
    I2s_DataLen_16Bit = 0u,
    I2s_DataLen_24Bit = 1u,
    I2s_DataLen_32Bit = 2u,
}en_i2s_data_len_t;

/**
 *******************************************************************************
 ** \brief I2S configuration structure
 ******************************************************************************/
typedef struct stc_i2s_config
{
    en_i2s_mode_t               enMode;              ///< I2S mode
    en_i2s_full_duplex_mode_t   enFullDuplexMode;    ///< I2S full duplex mode
    uint32_t                    u32I2sInterClkFreq;  ///< I2S internal clock frequency
    en_i2s_standard_t           enStandrad;          ///< I2S standard
    en_i2s_data_len_t           enDataBits;          ///< I2S data format, data bits
    en_i2s_ch_len_t             enChanelLen;         ///< I2S channel length
    en_functional_state_t       enMcoOutEn;          ///< I2S MCK output config
    en_functional_state_t       enExckEn;            ///< I2S EXCK function config
    uint32_t                    u32AudioFreq;        ///< I2S audio frequecy
}stc_i2s_config_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* define audio frequency */
#define I2S_AudioFreq_192k                          (192000ul)
#define I2S_AudioFreq_96k                           (96000ul)
#define I2S_AudioFreq_48k                           (48000ul)
#define I2S_AudioFreq_44k                           (44100ul)
#define I2S_AudioFreq_32k                           (32000ul)
#define I2S_AudioFreq_22k                           (22050ul)
#define I2S_AudioFreq_16k                           (16000ul)
#define I2S_AudioFreq_11k                           (11025ul)
#define I2S_AudioFreq_8k                            (8000ul)
#define I2S_AudioFreq_Default                       (2ul)

/* if use external clock open this define */
#define I2S_EXTERNAL_CLOCK_VAL                      (12288000ul)

/* 0,1 or 2 config for tx or tx buffer interrupt warning level */
#define RXBUF_IRQ_WL                                (1ul)
#define TXBUF_IRQ_WL                                (1ul)

/* 0: Short frame synchronization; 1: Long frame synchronization */
#define PCM_SYNC_FRAME                              (0ul)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
en_result_t I2s_Init(M4_I2S_TypeDef* pstcI2sReg, const stc_i2s_config_t* pstcI2sCfg);
void I2S_SendData(M4_I2S_TypeDef* pstcI2sReg, uint32_t u32Data);
uint32_t I2S_RevData(const M4_I2S_TypeDef* pstcI2sReg);
void I2S_FuncCmd(M4_I2S_TypeDef* pstcI2sReg, en_i2s_func_t enFunc, en_functional_state_t enNewState);
en_flag_status_t I2S_GetStatus(M4_I2S_TypeDef* pstcI2sReg, en_i2s_std_t enStd);
en_flag_status_t I2S_GetErrFlag(M4_I2S_TypeDef* pstcI2sReg, en_i2s_err_flag_t enErrFlag);
void I2S_ClrErrFlag(M4_I2S_TypeDef* pstcI2sReg, en_i2s_err_flag_t enErrFlag);
en_result_t I2s_DeInit(M4_I2S_TypeDef* pstcI2sReg);

//@} // I2sGroup

#ifdef __cplusplus
}
#endif

#endif /* __HC32F460_I2S_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
