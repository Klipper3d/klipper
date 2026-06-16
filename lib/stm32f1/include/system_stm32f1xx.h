/**
  ******************************************************************************
  * @file    system_stm32f1xx.h
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer System Header File.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f10x_system
  * @{
  */  
  
/**
  * @brief Define to prevent recursive inclusion
  */
#ifndef __SYSTEM_STM32F10X_H
#define __SYSTEM_STM32F10X_H

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup STM32F10x_System_Includes
  * @{
  */

/**
  * @}
  */


/** @addtogroup STM32F10x_System_Exported_types
  * @{
  */

extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */
extern const uint8_t  AHBPrescTable[16U];  /*!< AHB prescalers table values */
extern const uint8_t  APBPrescTable[8U];   /*!< APB prescalers table values */

/**
  * @}
  */

/** @addtogroup STM32F10x_System_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F10x_System_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F10x_System_Exported_Functions
  * @{
  */
  
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_STM32F10X_H */

/**
  * @}
  */
  
/**
  * @}
  */
