#if defined (STM32L431xx) || defined (STM32L432xx) || defined (STM32L433xx) || \
    defined (STM32L442xx) || defined (STM32L443xx) || defined (STM32L451xx) || \
    defined (STM32L452xx) || defined (STM32L462xx) || defined (STM32L471xx) || \
    defined (STM32L475xx) || defined (STM32L476xx) || defined (STM32L485xx) || \
    defined (STM32L486xx) || defined (STM32L496xx) || defined (STM32L4A6xx)
#include "stm32l4xx.h"
#include "core_cm4.h"
#include "stm32l4xx_hal_def.h"
#include "stm32l4xx_ll_adc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_comp.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_crc.h"
#include "stm32l4xx_ll_crs.h"
#include "stm32l4xx_ll_dac.h"
#include "stm32l4xx_ll_dma2d.h"
#include "stm32l4xx_ll_dma.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_fmc.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_iwdg.h"
#include "stm32l4xx_ll_lptim.h"
#include "stm32l4xx_ll_lpuart.h"
#include "stm32l4xx_ll_opamp.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_rng.h"
#include "stm32l4xx_ll_rtc.h"
#include "stm32l4xx_ll_sdmmc.h"
#include "stm32l4xx_ll_spi.h"
#include "stm32l4xx_ll_swpmi.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_usb.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_wwdg.h"
#endif

#if defined (STM32F405xx) || defined (STM32F415xx) || defined (STM32F407xx) || \
    defined (STM32F417xx) || defined (STM32F427xx) || defined (STM32F437xx) || \
    defined (STM32F429xx) || defined (STM32F439xx) || defined (STM32F401xC) || \
    defined (STM32F401xE) || defined (STM32F410Tx) || defined (STM32F410Cx) || \
    defined (STM32F410Rx) || defined (STM32F411xE) || defined (STM32F446xx) || \
    defined (STM32F469xx) || defined (STM32F479xx) || defined (STM32F412Cx) || \
    defined (STM32F412Rx) || defined (STM32F412Vx) || defined (STM32F412Zx) || \
    defined (STM32F413xx) || defined (STM32F423xx)
#include "stm32f4xx.h"
#include "core_cm4.h"
#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_crc.h"
#include "stm32f4xx_ll_dac.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_dma2d.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_i2c.h"
#include "stm32f4xx_ll_lptim.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_rng.h"
#include "stm32f4xx_ll_rtc.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_utils.h"
#endif

#if defined (STM32F756xx) || defined (STM32F746xx) || defined (STM32F745xx) || \
    defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx) || \
    defined (STM32F779xx) || defined (STM32F722xx) || defined (STM32F723xx) || \
    defined (STM32F732xx) || defined (STM32F733xx)
#include "stm32f7xx.h"
#include "core_cm7.h"
#include "stm32f7xx_ll_adc.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_crc.h"
#include "stm32f7xx_ll_dac.h"
#include "stm32f7xx_ll_dma.h"
#include "stm32f7xx_ll_dma2d.h"
#include "stm32f7xx_ll_exti.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_i2c.h"
#include "stm32f7xx_ll_lptim.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_rng.h"
#include "stm32f7xx_ll_rtc.h"
#include "stm32f7xx_ll_spi.h"
#include "stm32f7xx_ll_tim.h"
#include "stm32f7xx_ll_usart.h"
#include "stm32f7xx_ll_utils.h"
#endif
