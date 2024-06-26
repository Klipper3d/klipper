// Hardware PWM support on stm32
//
// Copyright (C) 2021  Michael Kurz <michi.kurz@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_MACH_STM32H7
#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "gpio.h" // gpio_pwm_write
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

#define MAX_PWM 255
DECL_CONSTANT("PWM_MAX", MAX_PWM);

struct gpio_pwm_info {
    TIM_TypeDef* timer;
    uint8_t pin, channel, function;
};

static const struct gpio_pwm_info pwm_regs[] = {
#if CONFIG_MACH_STM32F0
  #if CONFIG_MACH_STM32F070
    {TIM15, GPIO('A', 2), 1, GPIO_FUNCTION(0)},
    {TIM15, GPIO('A', 3), 2, GPIO_FUNCTION(0)},
    {TIM14, GPIO('A', 4), 1, GPIO_FUNCTION(4)},
    {TIM3, GPIO('A', 6), 1, GPIO_FUNCTION(1)},
    {TIM3, GPIO('A', 7), 2, GPIO_FUNCTION(1)},
    {TIM1, GPIO('A', 8), 1, GPIO_FUNCTION(2)},
    {TIM1, GPIO('A', 9), 2, GPIO_FUNCTION(2)},
    {TIM1, GPIO('A', 10), 3, GPIO_FUNCTION(2)},
    {TIM1, GPIO('A', 11), 4, GPIO_FUNCTION(2)},
    {TIM3, GPIO('B', 0), 3, GPIO_FUNCTION(1)},
    {TIM3, GPIO('B', 1), 4, GPIO_FUNCTION(1)},
    {TIM3, GPIO('B', 4), 1, GPIO_FUNCTION(1)},
    {TIM3, GPIO('B', 5), 2, GPIO_FUNCTION(1)},
    {TIM16, GPIO('B', 8), 1, GPIO_FUNCTION(2)},
    {TIM17, GPIO('B', 9), 1, GPIO_FUNCTION(2)},
    {TIM15, GPIO('B', 14), 1, GPIO_FUNCTION(1)},
    {TIM15, GPIO('B', 15), 2, GPIO_FUNCTION(1)},
    {TIM3, GPIO('C', 6), 1, GPIO_FUNCTION(0)},
    {TIM3, GPIO('C', 7), 2, GPIO_FUNCTION(0)},
    {TIM3, GPIO('C', 8), 3, GPIO_FUNCTION(0)},
    {TIM3, GPIO('C', 9), 4, GPIO_FUNCTION(0)}
  #endif
  #if CONFIG_MACH_STM32F072
    {TIM2, GPIO('A', 1), 2, GPIO_FUNCTION(2)},
    {TIM2, GPIO('A', 2), 3, GPIO_FUNCTION(2)},
    {TIM2, GPIO('A', 3), 4, GPIO_FUNCTION(2)},
    {TIM14, GPIO('A', 4), 1, GPIO_FUNCTION(4)},
    {TIM3, GPIO('A', 6), 1, GPIO_FUNCTION(1)},
    {TIM3, GPIO('A', 7), 2, GPIO_FUNCTION(1)},
    {TIM1, GPIO('A', 8), 1, GPIO_FUNCTION(2)},
    {TIM1, GPIO('A', 9), 2, GPIO_FUNCTION(2)},
    {TIM1, GPIO('A', 10), 3, GPIO_FUNCTION(2)},
    {TIM1, GPIO('A', 11), 4, GPIO_FUNCTION(2)},
    {TIM3, GPIO('B', 0), 3, GPIO_FUNCTION(1)},
    {TIM3, GPIO('B', 1), 4, GPIO_FUNCTION(1)},
    {TIM2, GPIO('B', 3), 2, GPIO_FUNCTION(2)},
    {TIM3, GPIO('B', 4), 1, GPIO_FUNCTION(1)},
    {TIM3, GPIO('B', 5), 2, GPIO_FUNCTION(1)},
    {TIM16, GPIO('B', 8), 1, GPIO_FUNCTION(2)},
    {TIM17, GPIO('B', 9), 1, GPIO_FUNCTION(2)},
    {TIM2, GPIO('B', 10), 3, GPIO_FUNCTION(2)},
    {TIM2, GPIO('B', 11), 4, GPIO_FUNCTION(2)},
    {TIM15, GPIO('B', 14), 1, GPIO_FUNCTION(1)},
    {TIM15, GPIO('B', 15), 2, GPIO_FUNCTION(1)},
    {TIM3, GPIO('C', 6), 1, GPIO_FUNCTION(0)},
    {TIM3, GPIO('C', 7), 2, GPIO_FUNCTION(0)},
    {TIM3, GPIO('C', 8), 3, GPIO_FUNCTION(0)},
    {TIM3, GPIO('C', 9), 4, GPIO_FUNCTION(0)},
    {TIM16, GPIO('E', 0), 1, GPIO_FUNCTION(0)},
    {TIM17, GPIO('E', 1), 1, GPIO_FUNCTION(0)},
    {TIM3, GPIO('E', 3), 1, GPIO_FUNCTION(0)},
    {TIM3, GPIO('E', 4), 2, GPIO_FUNCTION(0)},
    {TIM3, GPIO('E', 5), 3, GPIO_FUNCTION(0)},
    {TIM3, GPIO('E', 6), 4, GPIO_FUNCTION(0)},
    {TIM1, GPIO('E', 9), 1, GPIO_FUNCTION(0)},
    {TIM1, GPIO('E', 11), 2, GPIO_FUNCTION(0)},
    {TIM1, GPIO('E', 13), 3, GPIO_FUNCTION(0)},
    {TIM1, GPIO('E', 14), 4, GPIO_FUNCTION(0)},
    {TIM15, GPIO('F', 9), 1, GPIO_FUNCTION(0)},
    {TIM15, GPIO('F', 10), 2, GPIO_FUNCTION(0)}
  #endif
#elif CONFIG_MACH_STM32F1
    {TIM2, GPIO('A', 0),  1, GPIO_FUNCTION(2)},
    {TIM2, GPIO('A', 1),  2, GPIO_FUNCTION(2)},
    {TIM2, GPIO('A', 2),  3, GPIO_FUNCTION(2)},
    {TIM2, GPIO('A', 3),  4, GPIO_FUNCTION(2)},
    {TIM2, GPIO('A', 15), 1, GPIO_FUNCTION(1)},
    {TIM2, GPIO('B', 3),  2, GPIO_FUNCTION(1)},
    {TIM2, GPIO('B', 10), 3, GPIO_FUNCTION(1)},
    {TIM2, GPIO('B', 11), 4, GPIO_FUNCTION(1)},
    {TIM3, GPIO('A', 6),  1, GPIO_FUNCTION(1)},
    {TIM3, GPIO('A', 7),  2, GPIO_FUNCTION(1)},
    {TIM3, GPIO('B', 0),  3, GPIO_FUNCTION(1)},
    {TIM3, GPIO('B', 1),  4, GPIO_FUNCTION(1)},
    {TIM3, GPIO('C', 6),  1, GPIO_FUNCTION(2)},
    {TIM3, GPIO('C', 7),  2, GPIO_FUNCTION(2)},
    {TIM3, GPIO('C', 8),  3, GPIO_FUNCTION(2)},
    {TIM3, GPIO('C', 9),  4, GPIO_FUNCTION(2)},
    {TIM4, GPIO('D', 12), 1, GPIO_FUNCTION(2)},
    {TIM4, GPIO('D', 13), 2, GPIO_FUNCTION(2)},
    {TIM4, GPIO('D', 14), 3, GPIO_FUNCTION(2)},
    {TIM4, GPIO('D', 15), 4, GPIO_FUNCTION(2)},
    {TIM4, GPIO('B', 6),  1, GPIO_FUNCTION(2)},
    {TIM4, GPIO('B', 7),  2, GPIO_FUNCTION(2)},
    {TIM4, GPIO('B', 8),  3, GPIO_FUNCTION(2)},
    {TIM4, GPIO('B', 9),  4, GPIO_FUNCTION(2)}
#elif CONFIG_MACH_STM32F4
  #if CONFIG_MACH_STM32F401
    {TIM3,  GPIO('A',  6),  1, GPIO_FUNCTION(2)},
    {TIM3,  GPIO('C',  6),  1, GPIO_FUNCTION(3)},
    {TIM3,  GPIO('A',  7),  2, GPIO_FUNCTION(2)},
    {TIM3,  GPIO('C',  7),  2, GPIO_FUNCTION(3)},
    {TIM3,  GPIO('C',  8),  3, GPIO_FUNCTION(3)},
    {TIM3,  GPIO('C',  9),  4, GPIO_FUNCTION(3)},
  #endif
  #if CONFIG_MACH_STM32F446
    {TIM2,  GPIO('B',  2),  4, GPIO_FUNCTION(1)},
  #endif
  #if CONFIG_MACH_STM32F4x5 || CONFIG_MACH_STM32F446
    {TIM8,  GPIO('C',  6),  1, GPIO_FUNCTION(3)},
    {TIM8,  GPIO('C',  7),  2, GPIO_FUNCTION(3)},
    {TIM8,  GPIO('C',  8),  3, GPIO_FUNCTION(3)},
    {TIM8,  GPIO('C',  9),  4, GPIO_FUNCTION(3)},
    {TIM11, GPIO('F',  7),  1, GPIO_FUNCTION(3)},
    {TIM12, GPIO('B', 14),  1, GPIO_FUNCTION(9)},
    {TIM12, GPIO('B', 15),  2, GPIO_FUNCTION(9)},
    {TIM13, GPIO('A',  6),  1, GPIO_FUNCTION(9)},
    {TIM13, GPIO('F',  8),  1, GPIO_FUNCTION(9)},
    {TIM14, GPIO('A',  7),  1, GPIO_FUNCTION(9)},
    {TIM14, GPIO('F',  9),  1, GPIO_FUNCTION(9)},
  #endif
    // Pins that map to all klipper defined STM32F4 procs
    {TIM1,  GPIO('A',  8),  1, GPIO_FUNCTION(1)},
    {TIM1,  GPIO('E',  9),  1, GPIO_FUNCTION(1)},
    {TIM1,  GPIO('A',  9),  2, GPIO_FUNCTION(1)},
    {TIM1,  GPIO('E', 11),  2, GPIO_FUNCTION(1)},
    {TIM1,  GPIO('A', 10),  3, GPIO_FUNCTION(1)},
    {TIM1,  GPIO('E', 13),  3, GPIO_FUNCTION(1)},
    {TIM1,  GPIO('A', 11),  4, GPIO_FUNCTION(1)},
    {TIM1,  GPIO('E', 14),  4, GPIO_FUNCTION(1)},
    {TIM2,  GPIO('A',  5),  1, GPIO_FUNCTION(1)},
    {TIM2,  GPIO('A', 15),  1, GPIO_FUNCTION(1)},
    {TIM2,  GPIO('B',  3),  2, GPIO_FUNCTION(1)},
    {TIM2,  GPIO('B', 10),  3, GPIO_FUNCTION(1)},
    {TIM2,  GPIO('B', 11),  4, GPIO_FUNCTION(1)},
    {TIM3,  GPIO('B',  4),  1, GPIO_FUNCTION(2)},
    {TIM3,  GPIO('B',  5),  2, GPIO_FUNCTION(2)},
    {TIM3,  GPIO('B',  0),  3, GPIO_FUNCTION(2)},
    {TIM3,  GPIO('B',  1),  4, GPIO_FUNCTION(2)},
    {TIM4,  GPIO('B',  6),  1, GPIO_FUNCTION(2)},
    {TIM4,  GPIO('D', 12),  1, GPIO_FUNCTION(2)},
    {TIM4,  GPIO('B',  7),  2, GPIO_FUNCTION(2)},
    {TIM4,  GPIO('D', 13),  2, GPIO_FUNCTION(2)},
    {TIM4,  GPIO('D', 14),  3, GPIO_FUNCTION(2)},
    {TIM4,  GPIO('D', 15),  4, GPIO_FUNCTION(2)},
    {TIM5,  GPIO('A',  0),  1, GPIO_FUNCTION(2)},
    {TIM5,  GPIO('A',  1),  2, GPIO_FUNCTION(2)},
    {TIM5,  GPIO('A',  2),  3, GPIO_FUNCTION(2)},
    {TIM5,  GPIO('A',  3),  4, GPIO_FUNCTION(2)},
    {TIM9,  GPIO('E',  5),  1, GPIO_FUNCTION(3)},
    {TIM9,  GPIO('E',  6),  2, GPIO_FUNCTION(3)},
    {TIM10, GPIO('B',  8),  1, GPIO_FUNCTION(3)},
    {TIM11, GPIO('B',  9),  1, GPIO_FUNCTION(3)}
#elif CONFIG_MACH_STM32F7
    {TIM2, GPIO('A', 15), 1, GPIO_FUNCTION(1)},
    {TIM2, GPIO('B',  3), 2, GPIO_FUNCTION(1)},
    {TIM2, GPIO('B', 10), 3, GPIO_FUNCTION(1)},
    {TIM2, GPIO('B', 11), 4, GPIO_FUNCTION(1)},
    {TIM3, GPIO('B',  4), 1, GPIO_FUNCTION(1)},
    {TIM3, GPIO('B',  5), 1, GPIO_FUNCTION(1)},
    {TIM3, GPIO('C',  8), 2, GPIO_FUNCTION(1)},
    {TIM3, GPIO('C',  9), 2, GPIO_FUNCTION(1)},
    {TIM5, GPIO('A', 10), 3, GPIO_FUNCTION(1)},
    {TIM5, GPIO('E', 13), 3, GPIO_FUNCTION(1)},
    {TIM5, GPIO('A', 11), 4, GPIO_FUNCTION(1)},
    {TIM5, GPIO('E', 14), 4, GPIO_FUNCTION(1)},
#elif CONFIG_MACH_STM32G0
    {TIM15, GPIO('A',  2), 1, GPIO_FUNCTION(5)},
    {TIM15, GPIO('A',  3), 2, GPIO_FUNCTION(5)},
    {TIM14, GPIO('A',  4), 1, GPIO_FUNCTION(4)},
    {TIM3,  GPIO('A',  6), 1, GPIO_FUNCTION(1)},
    {TIM3,  GPIO('A',  7), 2, GPIO_FUNCTION(1)},
    {TIM1,  GPIO('A',  8), 1, GPIO_FUNCTION(2)},
    {TIM1,  GPIO('A',  9), 2, GPIO_FUNCTION(2)},
    {TIM1,  GPIO('A', 10), 3, GPIO_FUNCTION(2)},
    {TIM1,  GPIO('A', 11), 4, GPIO_FUNCTION(2)},
    {TIM3,  GPIO('B',  0), 3, GPIO_FUNCTION(1)},
    {TIM3,  GPIO('B',  1), 4, GPIO_FUNCTION(1)},
    {TIM1,  GPIO('B',  3), 2, GPIO_FUNCTION(1)},
    {TIM3,  GPIO('B',  4), 1, GPIO_FUNCTION(1)},
    {TIM3,  GPIO('B',  5), 2, GPIO_FUNCTION(1)},
  #if CONFIG_MACH_STM32G0Bx
    {TIM4,  GPIO('B',  6), 1, GPIO_FUNCTION(9)},
    {TIM4,  GPIO('B',  7), 2, GPIO_FUNCTION(9)},
    {TIM4,  GPIO('B',  8), 3, GPIO_FUNCTION(9)},
    {TIM4,  GPIO('B',  9), 4, GPIO_FUNCTION(9)},
  #endif
    {TIM15, GPIO('B', 14), 1, GPIO_FUNCTION(5)},
    {TIM15, GPIO('B', 15), 2, GPIO_FUNCTION(5)},
    {TIM15, GPIO('C',  1), 1, GPIO_FUNCTION(2)},
    {TIM15, GPIO('C',  2), 2, GPIO_FUNCTION(2)},
    {TIM3,  GPIO('C',  6), 1, GPIO_FUNCTION(1)},
    {TIM3,  GPIO('C',  7), 2, GPIO_FUNCTION(1)},
    {TIM3,  GPIO('C',  8), 3, GPIO_FUNCTION(1)},
    {TIM3,  GPIO('C',  9), 4, GPIO_FUNCTION(1)},
    {TIM1,  GPIO('C', 10), 3, GPIO_FUNCTION(2)},
    {TIM1,  GPIO('C', 11), 4, GPIO_FUNCTION(2)},
    {TIM14, GPIO('C', 12), 1, GPIO_FUNCTION(2)},
    {TIM16, GPIO('D',  0), 1, GPIO_FUNCTION(2)},
    {TIM17, GPIO('D',  1), 1, GPIO_FUNCTION(2)},
  #if CONFIG_MACH_STM32G0Bx
    {TIM4,  GPIO('D', 12), 1, GPIO_FUNCTION(2)},
    {TIM4,  GPIO('D', 13), 2, GPIO_FUNCTION(2)},
    {TIM4,  GPIO('D', 14), 3, GPIO_FUNCTION(2)},
    {TIM4,  GPIO('D', 15), 4, GPIO_FUNCTION(2)},
  #endif
    {TIM3,  GPIO('E',  3), 1, GPIO_FUNCTION(1)},
    {TIM3,  GPIO('E',  4), 2, GPIO_FUNCTION(1)},
    {TIM3,  GPIO('E',  5), 3, GPIO_FUNCTION(1)},
    {TIM3,  GPIO('E',  6), 4, GPIO_FUNCTION(1)},
    {TIM1,  GPIO('E',  9), 1, GPIO_FUNCTION(1)},
    {TIM1,  GPIO('E', 11), 2, GPIO_FUNCTION(1)},
    {TIM1,  GPIO('E', 13), 3, GPIO_FUNCTION(1)},
    {TIM1,  GPIO('E', 14), 4, GPIO_FUNCTION(1)},
    {TIM14, GPIO('F',  0), 1, GPIO_FUNCTION(2)},
    {TIM15, GPIO('F', 12), 1, GPIO_FUNCTION(0)},
    {TIM15, GPIO('F', 13), 2, GPIO_FUNCTION(0)},
#elif CONFIG_MACH_STM32H7
    {TIM2, GPIO('A', 0),  1, GPIO_FUNCTION(1)},
    {TIM2, GPIO('A', 5),  1, GPIO_FUNCTION(1)},
    {TIM2, GPIO('A', 15), 1, GPIO_FUNCTION(1)},
    {TIM2, GPIO('B', 3),  2, GPIO_FUNCTION(1)},
    {TIM2, GPIO('A', 1),  2, GPIO_FUNCTION(1)},
    {TIM2, GPIO('B', 10), 3, GPIO_FUNCTION(1)},
    {TIM2, GPIO('A', 2),  3, GPIO_FUNCTION(1)},
    {TIM3, GPIO('C', 6),  1, GPIO_FUNCTION(2)},
    {TIM3, GPIO('B', 4),  1, GPIO_FUNCTION(2)},
    {TIM3, GPIO('A', 6),  1, GPIO_FUNCTION(2)},
    {TIM3, GPIO('C', 7),  2, GPIO_FUNCTION(2)},
    {TIM3, GPIO('B', 5),  2, GPIO_FUNCTION(2)},
    {TIM3, GPIO('A', 7),  2, GPIO_FUNCTION(2)},
    {TIM3, GPIO('C', 8),  3, GPIO_FUNCTION(2)},
    {TIM3, GPIO('B', 0),  3, GPIO_FUNCTION(2)},
    {TIM4, GPIO('D', 12), 1, GPIO_FUNCTION(2)},
    {TIM4, GPIO('B', 6),  1, GPIO_FUNCTION(2)},
    {TIM4, GPIO('D', 13), 2, GPIO_FUNCTION(2)},
    {TIM4, GPIO('B', 7),  2, GPIO_FUNCTION(2)},
    {TIM4, GPIO('D', 14), 3, GPIO_FUNCTION(2)},
    {TIM4, GPIO('B', 8),  3, GPIO_FUNCTION(2)},
    {TIM5, GPIO('H', 10), 1, GPIO_FUNCTION(2)},
    {TIM5, GPIO('A', 0),  1, GPIO_FUNCTION(2)},
    {TIM5, GPIO('H', 11), 2, GPIO_FUNCTION(2)},
    {TIM5, GPIO('A', 1),  2, GPIO_FUNCTION(2)},
    {TIM5, GPIO('H', 12), 3, GPIO_FUNCTION(2)},
    {TIM5, GPIO('A', 2),  3, GPIO_FUNCTION(2)},
    {TIM12, GPIO('H', 6),  1, GPIO_FUNCTION(2)},
    {TIM12, GPIO('B', 14), 1, GPIO_FUNCTION(2)},
    {TIM12, GPIO('H', 9),  2, GPIO_FUNCTION(2)},
    {TIM12, GPIO('B', 15), 2, GPIO_FUNCTION(2)},
    {TIM13, GPIO('F', 8),  1, GPIO_FUNCTION(9)},
    {TIM13, GPIO('A', 6),  1, GPIO_FUNCTION(9)},
    {TIM14, GPIO('F', 9),  1, GPIO_FUNCTION(9)},
    {TIM14, GPIO('A', 7),  1, GPIO_FUNCTION(9)},
    {TIM15, GPIO('E', 5),  1, GPIO_FUNCTION(4)},
    {TIM15, GPIO('A', 2),  1, GPIO_FUNCTION(4)},
    {TIM16, GPIO('F', 6),  1, GPIO_FUNCTION(1)},
    {TIM16, GPIO('B', 8),  1, GPIO_FUNCTION(1)},
    {TIM17, GPIO('F', 7),  1, GPIO_FUNCTION(1)},
    {TIM17, GPIO('B', 9),  1, GPIO_FUNCTION(1)}
#endif
};

struct gpio_pwm
gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val){
    // Find pin in pwm_regs table
    const struct gpio_pwm_info* p = pwm_regs;
    for (;; p++) {
        if (p >= &pwm_regs[ARRAY_SIZE(pwm_regs)])
            shutdown("Not a valid PWM pin");
        if (p->pin == pin)
            break;
    }

    // Map cycle_time to pwm clock divisor
    uint32_t pclk = get_pclock_frequency((uint32_t)p->timer);
    uint32_t pclock_div = CONFIG_CLOCK_FREQ / pclk;
    if (pclock_div > 1)
        pclock_div /= 2; // Timers run at twice the normal pclock frequency
    uint32_t prescaler = cycle_time / (pclock_div * (MAX_PWM - 1));
    if (prescaler > 0) {
        prescaler -= 1;
    } else if (prescaler > UINT16_MAX) {
        prescaler = UINT16_MAX;
    }

    gpio_peripheral(p->pin, p->function, 0);

    // Enable clock
    if (!is_enabled_pclock((uint32_t) p->timer)) {
        enable_pclock((uint32_t) p->timer);
    }

    if (p->timer->CR1 & TIM_CR1_CEN) {
        if (p->timer->PSC != (uint16_t) prescaler) {
            shutdown("PWM already programmed at different speed");
        }
    } else {
        p->timer->PSC = (uint16_t) prescaler;
        p->timer->ARR = MAX_PWM - 1;
        p->timer->EGR |= TIM_EGR_UG;
    }

    struct gpio_pwm channel;
    switch (p->channel) {
        case 1: {
            channel.reg = (void*) &p->timer->CCR1;
            p->timer->CCER &= ~TIM_CCER_CC1E;
            p->timer->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_CC1S);
            p->timer->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 |
                                TIM_CCMR1_OC1PE | TIM_CCMR1_OC1FE);
            gpio_pwm_write(channel, val);
            p->timer->CCER |= TIM_CCER_CC1E;
            break;
        }
        case 2: {
            channel.reg = (void*) &p->timer->CCR2;
            p->timer->CCER &= ~TIM_CCER_CC2E;
            p->timer->CCMR1 &= ~(TIM_CCMR1_OC2M | TIM_CCMR1_CC2S);
            p->timer->CCMR1 |= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 |
                                TIM_CCMR1_OC2PE | TIM_CCMR1_OC2FE);
            gpio_pwm_write(channel, val);
            p->timer->CCER |= TIM_CCER_CC2E;
            break;
        }
        case 3: {
            channel.reg = (void*) &p->timer->CCR3;
            p->timer->CCER &= ~TIM_CCER_CC3E;
            p->timer->CCMR2 &= ~(TIM_CCMR2_OC3M | TIM_CCMR2_CC3S);
            p->timer->CCMR2 |= (TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 |
                                TIM_CCMR2_OC3PE | TIM_CCMR2_OC3FE);
            gpio_pwm_write(channel, val);
            p->timer->CCER |= TIM_CCER_CC3E;
            break;
        }
        case 4: {
            channel.reg = (void*) &p->timer->CCR4;
            p->timer->CCER &= ~TIM_CCER_CC4E;
            p->timer->CCMR2 &= ~(TIM_CCMR2_OC4M | TIM_CCMR2_CC4S);
            p->timer->CCMR2 |= (TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 |
                                TIM_CCMR2_OC4PE | TIM_CCMR2_OC4FE);
            gpio_pwm_write(channel, val);
            p->timer->CCER |= TIM_CCER_CC4E;
            break;
        }
        default:
            shutdown("Invalid PWM channel");
    }
    // Enable PWM output
    p->timer->CR1 |= TIM_CR1_CEN;
#if CONFIG_MACH_STM32H7 || CONFIG_MACH_STM32G0
    p->timer->BDTR |= TIM_BDTR_MOE;
#endif
    return channel;
}

void
gpio_pwm_write(struct gpio_pwm g, uint32_t val) {
    *(volatile uint32_t*) g.reg = val;
}
