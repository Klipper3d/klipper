# Main code build rules

src-y += sched.c command.c basecmd.c debugcmds.c
src-$(CONFIG_HAVE_GPIO) += gpiocmds.c stepper.c endstop.c
src-$(CONFIG_HAVE_GPIO_ADC) += adccmds.c
src-$(CONFIG_HAVE_GPIO_SPI) += spicmds.c
src-$(CONFIG_HAVE_GPIO_HARD_PWM) += pwmcmds.c
src-$(CONFIG_HAVE_USER_INTERFACE) += buttons.c lcd_st7920.c
