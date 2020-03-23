# Main code build rules

src-y += sched.c command.c basecmd.c
src-$(CONFIG_HAVE_DEBUG) += debugcmds.c
src-$(CONFIG_HAVE_ENDSTOPS) += endstop.c
src-$(CONFIG_HAVE_STEPPERS) += stepper.c
src-$(CONFIG_HAVE_GPIO) += gpiocmds.c #initial_pins.c
src-$(CONFIG_HAVE_GPIO_ADC) += adccmds.c
src-$(CONFIG_HAVE_GPIO_SPI) += spicmds.c thermocouple.c
src-$(CONFIG_HAVE_GPIO_I2C) += i2ccmds.c
src-$(CONFIG_HAVE_GPIO_HARD_PWM) += pwmcmds.c
src-$(CONFIG_HAVE_GPIO_BITBANGING) += lcd_st7920.c lcd_hd44780.c buttons.c \
    spi_software.c neopixel.c
src-$(CONFIG_HAVE_GPIO_UART) += tmcuart.c