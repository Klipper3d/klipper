# Main code build rules

src-y += sched.c command.c basecmd.c debugcmds.c initial_pins.c
src-$(CONFIG_HAVE_GPIO) += gpiocmds.c stepper.c endstop.c
src-$(CONFIG_HAVE_GPIO_ADC) += adccmds.c
src-$(CONFIG_HAVE_GPIO_SPI) += spicmds.c thermocouple.c
src-$(CONFIG_HAVE_GPIO_I2C) += i2ccmds.c
src-$(CONFIG_HAVE_GPIO_HARD_PWM) += pwmcmds.c
src-$(CONFIG_HAVE_GPIO_BITBANGING) += lcd_st7920.c lcd_hd44780.c buttons.c \
    tmcuart.c spi_software.c neopixel.c
