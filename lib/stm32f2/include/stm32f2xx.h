/**
  ******************************************************************************
  * @file    stm32f2xx.h
  * @author  MCD Application Team
  * @brief   CMSIS STM32F2xx Device Peripheral Access Layer Header File. 
  *
  *          The file is the unique include file that the application programmer
  *          is using in the C source code, usually in main.c. This file contains:
  *           - Configuration section that allows to select:
  *              - The STM32F2xx device used in the target application
  *              - To use or not the peripheral's drivers in application code(i.e. 
  *                code will be based on direct access to peripheral's registers 
  *                rather than drivers API), this option is controlled by 
  *                "#define USE_HAL_DRIVER"
  *  
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

/** @addtogroup stm32f2xx
  * @{
  */
    
#ifndef __STM32F2xx_H
#define __STM32F2xx_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
   
/** @addtogroup Library_configuration_section
  * @{
  */

/**
  * @brief STM32 Family
  */
#if !defined (STM32F2)
#define STM32F2
#endif /* STM32F2 */
   
/* Uncomment the line below according to the target STM32 device used in your
   application 
  */
#if !defined (STM32F205xx) && !defined (STM32F215xx) && !defined (STM32F207xx) && !defined (STM32F217xx)

  /* #define STM32F205xx */   /*!< STM32F205RG, STM32F205VG, STM32F205ZG, STM32F205RF, STM32F205VF, STM32F205ZF,
                                   STM32F205RE, STM32F205VE, STM32F205ZE, STM32F205RC, STM32F205VC, STM32F205ZC,
                                   STM32F205RB and STM32F205VB Devices */
  /* #define STM32F215xx */   /*!< STM32F215RG, STM32F215VG, STM32F215ZG, STM32F215RE, STM32F215VE and STM32F215ZE Devices */
  /* #define STM32F207xx */   /*!< STM32F207VG, STM32F207ZG, STM32F207IG, STM32F207VF, STM32F207ZF, STM32F207IF,
                                   STM32F207VE, STM32F207ZE, STM32F207IE, STM32F207VC, STM32F207ZC and STM32F207IC Devices */
  /* #define STM32F217xx */   /*!< STM32F217VG, STM32F217ZG, STM32F217IG, STM32F217VE, STM32F217ZE and STM32F217IE Devices */
    
#endif
   
/*  Tip: To avoid modifying this file each time you need to switch between these
        devices, you can define the device in your toolchain compiler preprocessor.
  */
#if !defined  (USE_HAL_DRIVER)
/**
 * @brief Comment the line below if you will not use the peripherals drivers.
   In this case, these drivers will not be included and the application code will 
   be based on direct access to peripherals registers 
   */
  /*#define USE_HAL_DRIVER */
#endif /* USE_HAL_DRIVER */

/**
  * @brief CMSIS Device version number
  */
#define __STM32F2xx_CMSIS_VERSION_MAIN   (0x02U) /*!< [31:24] main version */
#define __STM32F2xx_CMSIS_VERSION_SUB1   (0x02U) /*!< [23:16] sub1 version */
#define __STM32F2xx_CMSIS_VERSION_SUB2   (0x06U) /*!< [15:8]  sub2 version */
#define __STM32F2xx_CMSIS_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */
#define __STM32F2xx_CMSIS_VERSION        ((__STM32F2xx_CMSIS_VERSION_MAIN << 24)\
                                         |(__STM32F2xx_CMSIS_VERSION_SUB1 << 16)\
                                         |(__STM32F2xx_CMSIS_VERSION_SUB2 << 8 )\
                                         |(__STM32F2xx_CMSIS_VERSION))

/**
  * @}
  */

/** @addtogroup Device_Included
  * @{
  */

#if defined(STM32F205xx)
  #include "stm32f205xx.h"
#elif defined(STM32F215xx)
  #include "stm32f215xx.h"
#elif defined(STM32F207xx)
  #include "stm32f207xx.h"
#elif defined(STM32F217xx)
  #include "stm32f217xx.h"
#else
 #error "Please select first the target STM32F2xx device used in your application (in stm32f2xx.h file)"
#endif

/**
  * @}
  */

/** @addtogroup Exported_types
  * @{
  */ 
typedef enum 
{
  RESET = 0U, 
  SET = !RESET
} FlagStatus, ITStatus;

typedef enum 
{
  DISABLE = 0U, 
  ENABLE = !DISABLE
} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum 
{
  SUCCESS = 0U,
  ERROR = !SUCCESS
} ErrorStatus;

/**
  * @}
  */


/** @addtogroup Exported_macro
  * @{
  */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL))) 

/* Use of CMSIS compiler intrinsics for register exclusive access */
/* Atomic 32-bit register access macro to set one or several bits */
#define ATOMIC_SET_BIT(REG, BIT)                             \
  do {                                                       \
    uint32_t val;                                            \
    do {                                                     \
      val = __LDREXW((__IO uint32_t *)&(REG)) | (BIT);       \
    } while ((__STREXW(val,(__IO uint32_t *)&(REG))) != 0U); \
  } while(0)

/* Atomic 32-bit register access macro to clear one or several bits */
#define ATOMIC_CLEAR_BIT(REG, BIT)                           \
  do {                                                       \
    uint32_t val;                                            \
    do {                                                     \
      val = __LDREXW((__IO uint32_t *)&(REG)) & ~(BIT);      \
    } while ((__STREXW(val,(__IO uint32_t *)&(REG))) != 0U); \
  } while(0)

/* Atomic 32-bit register access macro to clear and set one or several bits */
#define ATOMIC_MODIFY_REG(REG, CLEARMSK, SETMASK)                          \
  do {                                                                     \
    uint32_t val;                                                          \
    do {                                                                   \
      val = (__LDREXW((__IO uint32_t *)&(REG)) & ~(CLEARMSK)) | (SETMASK); \
    } while ((__STREXW(val,(__IO uint32_t *)&(REG))) != 0U);               \
  } while(0)

/* Atomic 16-bit register access macro to set one or several bits */
#define ATOMIC_SETH_BIT(REG, BIT)                            \
  do {                                                       \
    uint16_t val;                                            \
    do {                                                     \
      val = __LDREXH((__IO uint16_t *)&(REG)) | (BIT);       \
    } while ((__STREXH(val,(__IO uint16_t *)&(REG))) != 0U); \
  } while(0)

/* Atomic 16-bit register access macro to clear one or several bits */
#define ATOMIC_CLEARH_BIT(REG, BIT)                          \
  do {                                                       \
    uint16_t val;                                            \
    do {                                                     \
      val = __LDREXH((__IO uint16_t *)&(REG)) & ~(BIT);      \
    } while ((__STREXH(val,(__IO uint16_t *)&(REG))) != 0U); \
  } while(0)

/* Atomic 16-bit register access macro to clear and set one or several bits */
#define ATOMIC_MODIFYH_REG(REG, CLEARMSK, SETMASK)                         \
  do {                                                                     \
    uint16_t val;                                                          \
    do {                                                                   \
      val = (__LDREXH((__IO uint16_t *)&(REG)) & ~(CLEARMSK)) | (SETMASK); \
    } while ((__STREXH(val,(__IO uint16_t *)&(REG))) != 0U);               \
  } while(0)

/**
  * @}
  */
  
#if defined (USE_HAL_DRIVER)
 #include "stm32f2xx_hal.h"
#endif /* USE_HAL_DRIVER */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32F2xx_H */

/**
  * @}
  */

/**
  * @}
  */
  



