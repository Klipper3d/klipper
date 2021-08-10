# Additional linux build rules

dirs-y += src/linux src/generic

src-y += linux/main.c linux/timer.c linux/console.c linux/watchdog.c
src-y += linux/pca9685.c linux/spidev.c linux/analog.c linux/hard_pwm.c
src-y += linux/i2c.c linux/gpio.c generic/crc16_ccitt.c generic/alloc.c
src-y += linux/sensor_ds18b20.c

CFLAGS_klipper.elf += -lutil -lpthread

flash: $(OUT)klipper.elf
	@echo "  Flashing"
	$(Q)sudo ./scripts/flash-linux.sh
