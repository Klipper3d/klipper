# Main code build rules

src-y += sched.c command.c basecmd.c debugcmds.c
src-$(CONFIG_HAVE_GPIO) += initial_pins.c gpiocmds.c stepper.c endstop.c \
    trsync.c
src-$(CONFIG_WANT_ADC) += adccmds.c
src-$(CONFIG_WANT_SPI) += spicmds.c
src-$(CONFIG_WANT_I2C) += i2ccmds.c
src-$(CONFIG_WANT_HARD_PWM) += pwmcmds.c
src-$(CONFIG_HAVE_GPIO_SDIO) += sdiocmds.c

src-$(CONFIG_WANT_BUTTONS) += buttons.c
src-$(CONFIG_WANT_TMCUART) += tmcuart.c
src-$(CONFIG_WANT_NEOPIXEL) += neopixel.c
src-$(CONFIG_WANT_PULSE_COUNTER) += pulse_counter.c
src-$(CONFIG_WANT_ST7920) += lcd_st7920.c
src-$(CONFIG_WANT_HD44780) += lcd_hd44780.c
src-$(CONFIG_WANT_SOFTWARE_SPI) += spi_software.c
src-$(CONFIG_WANT_SOFTWARE_I2C) += i2c_software.c
src-$(CONFIG_WANT_THERMOCOUPLE) += thermocouple.c
src-$(CONFIG_WANT_ADXL345) += sensor_adxl345.c
src-$(CONFIG_WANT_LIS2DW) += sensor_lis2dw.c
src-$(CONFIG_WANT_MPU9250) += sensor_mpu9250.c
src-$(CONFIG_WANT_ICM20948) += sensor_icm20948.c
src-$(CONFIG_WANT_HX71X) += sensor_hx71x.c
src-$(CONFIG_WANT_ADS1220) += sensor_ads1220.c
src-$(CONFIG_WANT_LDC1612) += sensor_ldc1612.c sensor_ldc1612_ng.c
src-$(CONFIG_WANT_SENSOR_ANGLE) += sensor_angle.c
src-$(CONFIG_NEED_SENSOR_BULK) += sensor_bulk.c
src-$(CONFIG_NEED_SOS_FILTER) += sos_filter.c
src-$(CONFIG_WANT_LOAD_CELL_PROBE) += load_cell_probe.c
