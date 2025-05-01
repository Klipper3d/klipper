# Additional lgt build rules

# Use the lgt toolchain
CROSS_PREFIX=avr-

# if you have a custom bootloader, do not forget to change this
#UPLOAD_BAUD=57600
UPLOAD_BAUD=115200

dirs-y += src/lgt src/generic

CFLAGS-$(CONFIG_HAVE_LIMITED_CODE_SIZE) += -Os
CFLAGS += $(CFLAGS-y) -mmcu=atmega328p


# Add lgt source files
src-y += lgt/main.c lgt/timer.c
src-$(CONFIG_HAVE_GPIO) += lgt/gpio.c
src-$(CONFIG_WANT_ADC) += lgt/adc.c
src-$(CONFIG_WANT_SPI) += lgt/spi.c
src-$(CONFIG_WANT_I2C) += lgt/i2c.c
src-$(CONFIG_WANT_HARD_PWM) += lgt/hard_pwm.c
src-$(CONFIG_LGT_WATCHDOG) += lgt/watchdog.c
src-$(CONFIG_SERIAL) += lgt/serial.c generic/serial_irq.c

# Suppress broken "misspelled signal handler" warnings on gcc 4.8.1
CFLAGS_klipper.elf := $(CFLAGS_klipper.elf) $(if $(filter 4.8.1, $(shell $(CC) -dumpversion)), -w)

# Build the additional hex output file
target-y += $(OUT)klipper.elf.hex

$(OUT)klipper.elf.hex: $(OUT)klipper.elf
	@echo "  Creating hex file $@"
	$(Q)$(OBJCOPY) -j .text -j .data -O ihex $< $@

flash: $(OUT)klipper.elf.hex
	@echo "  Flashing $< to $(FLASH_DEVICE) via avrdude"
	$(Q)if [ -z $(FLASH_DEVICE) ]; then echo "Please specify FLASH_DEVICE"; exit 1; fi
	$(Q)avrdude -pm328p -b$(UPLOAD_BAUD) -c$(CONFIG_AVRDUDE_PROTOCOL) -P"$(FLASH_DEVICE)" -D -U"flash:w:$(<):i"
