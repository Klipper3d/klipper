// Hardware PWM support on rp2040
//
// Copyright (C) 2021  Lasse Dalegaard <dalegaard@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "command.h" // DECL_CONSTANT
#include "gpio.h" // gpio_pwm_write
#include "sched.h" // sched_shutdown
#include "internal.h" // get_pclock_frequency
#include "hardware/structs/pwm.h" // pwm_hw
#include "hardware/structs/iobank0.h" // iobank0_hw
#include "hardware/regs/resets.h" // RESETS_RESET_PWM_BITS

#define MAX_PWM 255
DECL_CONSTANT("PWM_MAX", MAX_PWM);

struct gpio_pwm
gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val) {
    if(pin >= 30)
        shutdown("invalid gpio pin");

    // All pins on the rp2040 can be used for PWM, there are 8 PWM
    // slices and each has two channels.
    pwm_slice_hw_t * slice = &pwm_hw->slice[(pin >> 1) & 0x7];
    uint8_t channel = pin & 1;

    // Map cycle_time to clock divider
    // The rp2040 has an 8.4 fractional divider, so we'll map the requested
    // cycle time into that. The cycle_time we receive from Klippy is in
    // relation to the crystal frequency and so we need to scale it up to match
    // the PWM clock.
    // For better precision, we introduce a scale factor such that pclk * scale
    // doesn't overflow. We then multiply by this scale factor at the beginning
    // and divide by it at the end.
    uint32_t pclk = get_pclock_frequency(RESETS_RESET_PWM_BITS);
    uint32_t scale = 1 << __builtin_clz(pclk);
    uint32_t clock_mult = (scale * get_pclock_frequency(RESETS_RESET_PWM_BITS))
                          / CONFIG_CLOCK_FREQ;
    uint32_t cycle_clocks = clock_mult * cycle_time;
    uint32_t div_int = cycle_clocks / MAX_PWM / scale;
    uint32_t div_frac = (cycle_clocks - div_int * MAX_PWM * scale) * 16
                        / MAX_PWM / scale;

    // Clamp range of the divider
    if(div_int > 255) {
        div_int = 255;
        div_frac = 15;
    } else if(div_int < 1) {
        div_int = 1;
        div_frac = 0;
    }

    uint32_t pwm_div = div_int << 4 | div_frac;

    // Enable clock
    if (!is_enabled_pclock(RESETS_RESET_PWM_BITS))
        enable_pclock(RESETS_RESET_PWM_BITS);

    // If this PWM slice hasn't been set up yet, we do the full set
    // up cycle. If it's already been set up however, we check that
    // the cycle time requested now matches the cycle time already
    // set on the slice. This allows both channels to be utilized,
    // as long as their cycle times are the same.
    if(!(slice->csr & PWM_CH0_CSR_EN_BITS)) {
        slice->div = pwm_div;
        slice->top = MAX_PWM - 1;
        slice->ctr = PWM_CH0_CTR_RESET;
        slice->cc = PWM_CH0_CC_RESET;
        slice->csr = PWM_CH0_CSR_EN_BITS;
    } else {
        if (slice->div != pwm_div)
            shutdown("PWM pin has different cycle time from another in "
                     "the same slice");

        // PWM is already enabled on this slice, we'll check if the
        // aliasing GPIO pin is already set up for PWM function. If it
        // is, then we need to bail out.
        uint32_t alias_pin = (~pin & 0x10) | (pin & 0xF);
        uint32_t alias_ctrl = iobank0_hw->io[alias_pin].ctrl;
        uint32_t alias_func = alias_ctrl & IO_BANK0_GPIO0_CTRL_FUNCSEL_BITS;
        if (alias_func == IO_BANK0_GPIO0_CTRL_FUNCSEL_VALUE_PWM_A_0)
            shutdown("Aliasing PWM pin already has PWM enabled");
    }

    struct gpio_pwm out;
    out.reg = (void*)&slice->cc;
    out.shift = channel ? PWM_CH0_CC_B_LSB : PWM_CH0_CC_A_LSB;
    out.mask = channel ? PWM_CH0_CC_B_BITS : PWM_CH0_CC_A_BITS;

    gpio_peripheral(pin, IO_BANK0_GPIO0_CTRL_FUNCSEL_VALUE_PWM_A_0, 0);
    gpio_pwm_write(out, val);

    return out;
}

void
gpio_pwm_write(struct gpio_pwm g, uint32_t val) {
    hw_write_masked((uint32_t*)g.reg, val << g.shift, g.mask);
}
