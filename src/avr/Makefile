# Additional avr build rules

# Use the avr toolchain
CROSS_PREFIX=avr-

dirs-y += src/avr src/generic

CFLAGS-$(CONFIG_HAVE_LIMITED_CODE_SIZE) += -Os
CFLAGS += $(CFLAGS-y) -mmcu=$(CONFIG_MCU)

# Add avr source files
src-y += avr/main.c avr/timer.c
src-$(CONFIG_HAVE_GPIO) += avr/gpio.c
src-$(CONFIG_WANT_ADC) += avr/adc.c
src-$(CONFIG_WANT_SPI) += avr/spi.c
src-$(CONFIG_WANT_I2C) += avr/i2c.c
src-$(CONFIG_WANT_HARD_PWM) += avr/hard_pwm.c
src-$(CONFIG_AVR_WATCHDOG) += avr/watchdog.c
src-$(CONFIG_USBSERIAL) += avr/usbserial.c generic/usb_cdc.c
src-$(CONFIG_SERIAL) += avr/serial.c generic/serial_irq.c

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
	$(Q)avrdude -p$(CONFIG_MCU) -c$(CONFIG_AVRDUDE_PROTOCOL) -P"$(FLASH_DEVICE)" -D -U"flash:w:$(<):i"
