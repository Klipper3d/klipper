#ifndef __STM32_INTERNAL_H
#define __STM32_INTERNAL_H
// Local definitions for STM32 code

#include "autoconf.h" // CONFIG_MACH_STM32F1

#if CONFIG_MACH_GD32E23X
#include "gd32e23x.h"

// GD32E23x GPIO registers use the STM32F0-compatible layout.  Provide the
// common STM32 type so gpio.c and gpioperiph.c can be shared.
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
    volatile uint32_t BRR;
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
} SPI_TypeDef;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    volatile uint32_t GTPR;
    volatile uint32_t RTOR;
    volatile uint32_t RQR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t RDR;
    volatile uint32_t TDR;
} USART_TypeDef;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
} I2C_TypeDef;

#define SPI_CR1_CPHA_Pos 0
#define SPI_CR1_BR_Pos 3
#define SPI_CR1_SPE SPI_CTL0_SPIEN
#define SPI_CR1_MSTR SPI_CTL0_MSTMOD
#define SPI_CR1_SSM SPI_CTL0_SWNSSEN
#define SPI_CR1_SSI SPI_CTL0_SWNSS
#define SPI_SR_RXNE SPI_STAT_RBNE
#define SPI_SR_TXE SPI_STAT_TBE
#define SPI_SR_BSY SPI_STAT_TRANS

#define USART_CR1_UE USART_CTL0_UEN
#define USART_CR1_RE USART_CTL0_REN
#define USART_CR1_TE USART_CTL0_TEN
#define USART_CR1_RXNEIE USART_CTL0_RBNEIE
#define USART_CR1_TXEIE USART_CTL0_TBEIE
#define USART_CR3_OVRDIS USART_CTL2_OVRD
#define USART_ISR_RXNE USART_STAT_RBNE
#define USART_ISR_TXE USART_STAT_TBE
#define USART_BRR_DIV_MANTISSA_Pos 4
#define USART_BRR_DIV_FRACTION_Pos 0

#define I2C_CR1_PE I2C_CTL0_I2CEN
#define I2C_CR1_START I2C_CTL0_START
#define I2C_CR1_STOP I2C_CTL0_STOP
#define I2C_CR1_ACK I2C_CTL0_ACKEN
#define I2C_CR1_SWRST I2C_CTL0_SRESET
#define I2C_SR1_SB I2C_STAT0_SBSEND
#define I2C_SR1_ADDR I2C_STAT0_ADDSEND
#define I2C_SR1_TXE I2C_STAT0_TBE
#define I2C_SR1_RXNE I2C_STAT0_RBNE
#define I2C_SR1_AF I2C_STAT0_AERR
#define I2C_SR2_MSL I2C_STAT1_MASTER

#undef GPIOA
#undef GPIOB
#undef GPIOC
#undef GPIOF
#define GPIOA ((GPIO_TypeDef *)(GPIO_BASE + 0x0000U))
#define GPIOB ((GPIO_TypeDef *)(GPIO_BASE + 0x0400U))
#define GPIOC ((GPIO_TypeDef *)(GPIO_BASE + 0x0800U))
#define GPIOF ((GPIO_TypeDef *)(GPIO_BASE + 0x1400U))
#elif CONFIG_MACH_STM32F0
#include "stm32f0xx.h"
#elif CONFIG_MACH_STM32F1
#include "stm32f1xx.h"
#elif CONFIG_MACH_STM32F2
#include "stm32f2xx.h"
#elif CONFIG_MACH_STM32F4
#include "stm32f4xx.h"
#elif CONFIG_MACH_STM32F7
#include "stm32f7xx.h"
#elif CONFIG_MACH_STM32G0
#include "stm32g0xx.h"
#elif CONFIG_MACH_STM32G4
#include "stm32g4xx.h"
#elif CONFIG_MACH_STM32H7
#include "stm32h7xx.h"
#elif CONFIG_MACH_STM32L4
#include "stm32l4xx.h"
#endif

// gpio.c
GPIO_TypeDef *gpio_pin_to_regs(uint32_t pin);
#define GPIO(PORT, NUM) (((PORT)-'A') * 16 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 16)
#define GPIO2BIT(PIN) (1<<((PIN) % 16))

// gpioperiph.c
#define GPIO_INPUT 0
#define GPIO_OUTPUT 1
#define GPIO_OPEN_DRAIN 0x100
#define GPIO_HIGH_SPEED 0x200
#define GPIO_FUNCTION(fn) (2 | ((fn) << 4))
#define GPIO_ANALOG 3
void gpio_peripheral(uint32_t gpio, uint32_t mode, int pullup);

// clockline.c
void enable_pclock(uint32_t periph_base);
int is_enabled_pclock(uint32_t periph_base);

// dfu_reboot.c
void dfu_reboot(void);
void dfu_reboot_check(void);

// stm32??.c
struct cline { volatile uint32_t *en, *rst; uint32_t bit; };
struct cline lookup_clock_line(uint32_t periph_base);
uint32_t get_pclock_frequency(uint32_t periph_base);
void gpio_clock_enable(GPIO_TypeDef *regs);

#if CONFIG_MACH_GD32E23X
uint32_t is_enable_pclock(uint32_t pclk);
#endif

#endif // internal.h
