/**
  ******************************************************************************
  * @file    stm32f0xx.h
  * @author  MCD Application Team
  * @brief   CMSIS STM32F0xx Device Peripheral Access Layer Header File.           
  *            
  *          The file is the unique include file that the application programmer
  *          is using in the C source code, usually in main.c. This file contains:
  *           - Configuration section that allows to select:
  *              - The STM32F0xx device used in the target application
  *              - To use or not the peripheral's drivers in application code(i.e. 
  *                code will be based on direct access to peripheral's registers 
  *                rather than drivers API), this option is controlled by 
  *                "#define USE_HAL_DRIVER"
  *  
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
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

/** @addtogroup stm32f0xx
  * @{
  */
    
#ifndef __STM32F0xx_H
#define __STM32F0xx_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
   
/** @addtogroup Library_configuration_section
  * @{
  */
  
/**
  * @brief STM32 Family
  */
#if !defined  (STM32F0)
#define STM32F0
#endif /* STM32F0 */

/** Uncomment the line below according to the target STM32 device used in your application.
  * stm32f0xxxx.h file contains:
  * - All the peripheral register's definitions, bits definitions and memory mapping for STM32F0xxxx devices
  * - IRQ channel definition
  * - Peripheral memory mapping and physical registers address definition
  * - Peripheral pointer declaration and driver header file inclusion
  * - Product miscellaneous configuration: assert macros, �
  * Note: These CMSIS drivers (stm32f0xxxx.h) are always supporting features of the sub-family's superset.
  */

#if !defined (STM32F030x6) && !defined (STM32F030x8) &&                           \
    !defined (STM32F031x6) && !defined (STM32F038xx) &&                           \
    !defined (STM32F042x6) && !defined (STM32F048xx) && !defined (STM32F070x6) && \
    !defined (STM32F051x8) && !defined (STM32F058xx) &&                           \
    !defined (STM32F071xB) && !defined (STM32F072xB) && !defined (STM32F078xx) && !defined (STM32F070xB) && \
    !defined (STM32F091xC) && !defined (STM32F098xx) && !defined (STM32F030xC)
  /* #define STM32F030x6 */  /*!< STM32F030x4, STM32F030x6 Devices (STM32F030xx microcontrollers where the Flash memory ranges between 16 and 32 Kbytes)              */
  /* #define STM32F030x8 */  /*!< STM32F030x8 Devices (STM32F030xx microcontrollers where the Flash memory is 64 Kbytes)                                              */
  /* #define STM32F031x6 */  /*!< STM32F031x4, STM32F031x6 Devices (STM32F031xx microcontrollers where the Flash memory ranges between 16 and 32 Kbytes)              */
  /* #define STM32F038xx */  /*!< STM32F038xx Devices (STM32F038xx microcontrollers where the Flash memory is 32 Kbytes)                                              */
  /* #define STM32F042x6 */  /*!< STM32F042x4, STM32F042x6 Devices (STM32F042xx microcontrollers where the Flash memory ranges between 16 and 32 Kbytes)              */
  /* #define STM32F048xx */  /*!< STM32F048xx Devices (STM32F048xx microcontrollers where the Flash memory is 32 Kbytes)                                              */
  /* #define STM32F051x8 */  /*!< STM32F051x4, STM32F051x6, STM32F051x8 Devices (STM32F051xx microcontrollers where the Flash memory ranges between 16 and 64 Kbytes) */
  /* #define STM32F058xx */  /*!< STM32F058xx Devices (STM32F058xx microcontrollers where the Flash memory is 64 Kbytes)                                              */
  /* #define STM32F070x6 */  /*!< STM32F070x6 Devices (STM32F070x6 microcontrollers where the Flash memory ranges between 16 and 32 Kbytes)                           */
  /* #define STM32F070xB */  /*!< STM32F070xB Devices (STM32F070xB microcontrollers where the Flash memory ranges between 64 and 128 Kbytes)                          */
  /* #define STM32F071xB */  /*!< STM32F071x8, STM32F071xB Devices (STM32F071xx microcontrollers where the Flash memory ranges between 64 and 128 Kbytes)             */
  /* #define STM32F072xB */  /*!< STM32F072x8, STM32F072xB Devices (STM32F072xx microcontrollers where the Flash memory ranges between 64 and 128 Kbytes)             */
  /* #define STM32F078xx */  /*!< STM32F078xx Devices (STM32F078xx microcontrollers where the Flash memory is 128 Kbytes)                                             */
  /* #define STM32F030xC */  /*!< STM32F030xC Devices (STM32F030xC microcontrollers where the Flash memory is 256 Kbytes)                                             */  
  /* #define STM32F091xC */  /*!< STM32F091xB, STM32F091xC Devices (STM32F091xx microcontrollers where the Flash memory ranges between 128 and 256 Kbytes)            */
  /* #define STM32F098xx */  /*!< STM32F098xx Devices (STM32F098xx microcontrollers where the Flash memory is 256 Kbytes)                                             */
#endif
/* Legacy aliases */
#if defined (STM32F048x6)
 #define STM32F048xx 
#endif /* STM32F048x6 */

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
  * @brief CMSIS Device version number V2.3.7
  */
#define __STM32F0_DEVICE_VERSION_MAIN   (0x02) /*!< [31:24] main version */
#define __STM32F0_DEVICE_VERSION_SUB1   (0x03) /*!< [23:16] sub1 version */
#define __STM32F0_DEVICE_VERSION_SUB2   (0x07) /*!< [15:8]  sub2 version */
#define __STM32F0_DEVICE_VERSION_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __STM32F0_DEVICE_VERSION        ((__STM32F0_DEVICE_VERSION_MAIN << 24)\
                                        |(__STM32F0_DEVICE_VERSION_SUB1 << 16)\
                                        |(__STM32F0_DEVICE_VERSION_SUB2 << 8 )\
                                        |(__STM32F0_DEVICE_VERSION_RC))
                                             
/**
  * @}
  */

/** @addtogroup Device_Included
  * @{
  */

#if defined(STM32F030x6)
  #include "stm32f030x6.h"
#elif defined(STM32F030x8)
  #include "stm32f030x8.h"
#elif defined(STM32F031x6)
  #include "stm32f031x6.h"
#elif defined(STM32F038xx)
  #include "stm32f038xx.h"
#elif defined(STM32F042x6)
  #include "stm32f042x6.h"
#elif defined(STM32F048xx)
  #include "stm32f048xx.h"
#elif defined(STM32F051x8)
  #include "stm32f051x8.h"
#elif defined(STM32F058xx)
  #include "stm32f058xx.h"
#elif defined(STM32F070x6)
  #include "stm32f070x6.h"
#elif defined(STM32F070xB)
  #include "stm32f070xb.h"
#elif defined(STM32F071xB)
  #include "stm32f071xb.h"
#elif defined(STM32F072xB)
  #include "stm32f072xb.h"
#elif defined(STM32F078xx)
  #include "stm32f078xx.h"
#elif defined(STM32F091xC)
  #include "stm32f091xc.h"
#elif defined(STM32F098xx)
  #include "stm32f098xx.h"
#elif defined(STM32F030xC)
  #include "stm32f030xc.h"    
#else
 #error "Please select first the target STM32F0xx device used in your application (in stm32f0xx.h file)"
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

/* Use of interrupt control for register exclusive access */
/* Atomic 32-bit register access macro to set one or several bits */
#define ATOMIC_SET_BIT(REG, BIT)                             \
  do {                                                       \
    uint32_t primask;                                        \
    primask = __get_PRIMASK();                               \
    __set_PRIMASK(1);                                        \
    SET_BIT((REG), (BIT));                                   \
    __set_PRIMASK(primask);                                  \
  } while(0)

/* Atomic 32-bit register access macro to clear one or several bits */
#define ATOMIC_CLEAR_BIT(REG, BIT)                           \
  do {                                                       \
    uint32_t primask;                                        \
    primask = __get_PRIMASK();                               \
    __set_PRIMASK(1);                                        \
    CLEAR_BIT((REG), (BIT));                                 \
    __set_PRIMASK(primask);                                  \
  } while(0)

/* Atomic 32-bit register access macro to clear and set one or several bits */
#define ATOMIC_MODIFY_REG(REG, CLEARMSK, SETMASK)            \
  do {                                                       \
    uint32_t primask;                                        \
    primask = __get_PRIMASK();                               \
    __set_PRIMASK(1);                                        \
    MODIFY_REG((REG), (CLEARMSK), (SETMASK));                \
    __set_PRIMASK(primask);                                  \
  } while(0)

/* Atomic 16-bit register access macro to set one or several bits */
#define ATOMIC_SETH_BIT(REG, BIT) ATOMIC_SET_BIT(REG, BIT)                                   \

/* Atomic 16-bit register access macro to clear one or several bits */
#define ATOMIC_CLEARH_BIT(REG, BIT) ATOMIC_CLEAR_BIT(REG, BIT)                               \

/* Atomic 16-bit register access macro to clear and set one or several bits */
#define ATOMIC_MODIFYH_REG(REG, CLEARMSK, SETMASK) ATOMIC_MODIFY_REG(REG, CLEARMSK, SETMASK) \

/**
  * @}
  */

#if defined (USE_HAL_DRIVER)
 #include "stm32f0xx_hal.h"
#endif /* USE_HAL_DRIVER */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32F0xx_H */
/**
  * @}
  */

/**
  * @}
  */

