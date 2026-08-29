/**
  ******************************************************************************
  * @file    stm32g4xx.h
  * @author  MCD Application Team
  * @brief   CMSIS STM32G4xx Device Peripheral Access Layer Header File.
  *
  *          The file is the unique include file that the application programmer
  *          is using in the C source code, usually in main.c. This file contains:
  *           - Configuration section that allows to select:
  *              - The STM32G4xx device used in the target application
  *              - To use or not the peripheral’s drivers in application code(i.e.
  *                code will be based on direct access to peripheral’s registers
  *                rather than drivers API), this option is controlled by
  *                "#define USE_HAL_DRIVER"
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
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

/** @addtogroup stm32g4xx
  * @{
  */

#ifndef __STM32G4xx_H
#define __STM32G4xx_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/** @addtogroup Library_configuration_section
  * @{
  */

/**
  * @brief STM32 Family
  */
#if !defined (STM32G4)
#define STM32G4
#endif /* STM32G4 */

/* Uncomment the line below according to the target STM32G4 device used in your
   application
  */

#if !defined (STM32G431xx) && !defined (STM32G441xx) && !defined (STM32G471xx) && \
    !defined (STM32G473xx) && !defined (STM32G474xx) && !defined (STM32G484xx) && \
    !defined (STM32GBK1CB) && !defined (STM32G491xx) && !defined (STM32G4A1xx) && \
    !defined (STM32G414xx)
  /* #define STM32G414xx */   /*!< STM32G414xx Devices */
  /* #define STM32G431xx */   /*!< STM32G431xx Devices */
  /* #define STM32G441xx */   /*!< STM32G441xx Devices */
  /* #define STM32G471xx */   /*!< STM32G471xx Devices */
  /* #define STM32G473xx */   /*!< STM32G473xx Devices */
  /* #define STM32G483xx */   /*!< STM32G483xx Devices */
  /* #define STM32G474xx */   /*!< STM32G474xx Devices */
  /* #define STM32G484xx */   /*!< STM32G484xx Devices */
  /* #define STM32G491xx */   /*!< STM32G491xx Devices */
  /* #define STM32G4A1xx */   /*!< STM32G4A1xx Devices */
  /* #define STM32GBK1CB */   /*!< STM32GBK1CB Devices */
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
  * @brief CMSIS Device version number V1.2.4
  */
#define __STM32G4_CMSIS_VERSION_MAIN   (0x01U) /*!< [31:24] main version */
#define __STM32G4_CMSIS_VERSION_SUB1   (0x02U) /*!< [23:16] sub1 version */
#define __STM32G4_CMSIS_VERSION_SUB2   (0x04U) /*!< [15:8]  sub2 version */
#define __STM32G4_CMSIS_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */
#define __STM32G4_CMSIS_VERSION        ((__STM32G4_CMSIS_VERSION_MAIN << 24)\
                                       |(__STM32G4_CMSIS_VERSION_SUB1 << 16)\
                                       |(__STM32G4_CMSIS_VERSION_SUB2 << 8 )\
                                       |(__STM32G4_CMSIS_VERSION_RC))

/**
  * @}
  */

/** @addtogroup Device_Included
  * @{
  */

#if defined(STM32G431xx)
  #include "stm32g431xx.h"
#elif defined(STM32G441xx)
  #include "stm32g441xx.h"
#elif defined(STM32G471xx)
  #include "stm32g471xx.h"
#elif defined(STM32G473xx)
  #include "stm32g473xx.h"
#elif defined(STM32G483xx)
  #include "stm32g483xx.h"
#elif defined(STM32G474xx)
  #include "stm32g474xx.h"
#elif defined(STM32G484xx)
  #include "stm32g484xx.h"
#elif defined(STM32G491xx)
  #include "stm32g491xx.h"
#elif defined(STM32G4A1xx)
  #include "stm32g4a1xx.h"
#elif defined(STM32GBK1CB)
  #include "stm32gbk1cb.h"
#elif defined(STM32G414xx)
  #include "stm32g414xx.h"
#else
  #error "Please select first the target STM32G4xx device used in your application (in stm32g4xx.h file)"
#endif

/**
  * @}
  */

/** @addtogroup Exported_types
  * @{
  */
typedef enum
{
  RESET = 0,
  SET = !RESET
} FlagStatus, ITStatus;

typedef enum
{
  DISABLE = 0,
  ENABLE = !DISABLE
} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum
{
  SUCCESS = 0,
  ERROR = !SUCCESS
} ErrorStatus;

/**
  * @}
  */


/** @addtogroup Exported_macros
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
 #include "stm32g4xx_hal.h"
#endif /* USE_HAL_DRIVER */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32G4xx_H */
/**
  * @}
  */

/**
  * @}
  */




