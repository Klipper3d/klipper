# Additional avr build rules

# Use the avr toolchain
CROSS_PREFIX=avr-

CFLAGS-y += -mmcu=$(CONFIG_MCU) -DF_CPU=$(CONFIG_CLOCK_FREQ)
LDFLAGS-y += -Wl,--relax

# Add avr source files
src-y += avr/main.c avr/timer.c avr/gpio.c avr/misc.c
src-$(CONFIG_AVR_WATCHDOG) += avr/watchdog.c
src-$(CONFIG_AVR_USBSERIAL) += avr/usbserial.c ../lib/pjrc_usb_serial/usb_serial.c
src-$(CONFIG_AVR_SERIAL) += avr/serial.c

# Build the additional hex output file
target-y += $(OUT)klipper.elf.hex

$(OUT)klipper.elf.hex: $(OUT)klipper.elf
	@echo "  Creating hex file $@"
	$(Q)$(OBJCOPY) -j .text -j .data -O ihex $< $@
