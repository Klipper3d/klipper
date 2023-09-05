# Additional linux build rules

dirs-y += src/linux src/generic

src-y += linux/main.c linux/timer.c linux/console.c linux/watchdog.c
src-y += linux/pca9685.c linux/spidev.c linux/analog.c linux/hard_pwm.c
src-y += linux/i2c.c generic/crc16_ccitt.c generic/alloc.c
src-y += linux/sensor_ds18b20.c

ifeq ($(CONFIG_WIRINGPI),y)
	CFLAGS += -DWIRINGPI
	CFLAGS += -I/usr/local/include -I/usr/include/wiringpi2
	CFLAGS_klipper.elf += -lwiringPiDev -lwiringPi -lm -lcrypt
	src-y += linux/gpio_wiringPi.c
else
	src-y += linux/gpio.c
endif

CFLAGS_klipper.elf += -lutil -lrt -lpthread

flash: $(OUT)klipper.elf
	@echo "  Flashing"
	$(Q)sudo ./scripts/flash-linux.sh $(OUT)
