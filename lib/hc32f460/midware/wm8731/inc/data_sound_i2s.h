/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file data_sound_i2s.h
 **
 ** Sound data Module
 **
 **   - 2018-11-13  CDT First version for Device Driver Library
 **
 ******************************************************************************/

#ifndef __DATA_SOUND_I2S_H__
#define __DATA_SOUND_I2S_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Global pre-processor symbols/macros ('define')                            */
/*****************************************************************************/

/******************************************************************************
 * Global definitions
 ******************************************************************************/

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/
extern uint32_t u32WavLen_8k;
extern uint32_t u32WavLen_44k;
extern const uint8_t au8PixieDustSoundI2s_441[];
extern const uint8_t au8PixieDustSoundI2s_8[];

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __DATA_SOUND_I2S_H__ */
/*****************************************************************************/
/* EOF (not truncated)                                                       */
/*****************************************************************************/
