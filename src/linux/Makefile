# Additional linux build rules

dirs-y += src/linux src/generic

src-y += linux/main.c linux/timer.c linux/console.c linux/watchdog.c
src-y += linux/pca9685.c linux/spidev.c linux/analog.c linux/hard_pwm.c
src-y += linux/i2c.c linux/gpio.c generic/crc16_ccitt.c generic/alloc.c

CFLAGS_klipper.elf += -lutil
define n


endef

ifeq ($(CONFIG_GPIO_CHIP_COUNT),0)
    ifeq (,$(wildcard /dev/gpiochip*))
      $(error "Could not find character devices /dev/gpiochip* $n\
               To proceed, you must manually load the necessary \
			   kernel module before launching "make" or choose \
			   the exact number of gpiochip to be used in the configuration.\
			   $n$n\
			   If you have installed the gpioutils (recommended) it \
			   is useful to run the command "sudo gpiodetect".\
			   ")
	endif
	CFLAGS += -DGPIO_CHIP_COUNT=$(shell ls /dev/gpiochip* | wc -l)
else
	CFLAGS += -DGPIO_CHIP_COUNT=0$(CONFIG_GPIO_CHIP_COUNT)
endif

flash: $(OUT)klipper.elf
	@echo "  Flashing"
	$(Q)sudo ./scripts/flash-linux.sh
