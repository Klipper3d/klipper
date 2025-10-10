# lpc176x build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/lpc176x src/generic lib/lpc176x/device

CFLAGS += -mthumb -mcpu=cortex-m3 -Ilib/lpc176x/device -Ilib/cmsis-core

CFLAGS_klipper.elf += -nostdlib -lgcc -lc_nano
CFLAGS_klipper.elf += -T $(OUT)src/generic/armcm_link.ld
$(OUT)klipper.elf: $(OUT)src/generic/armcm_link.ld

# Add source files
src-y += lpc176x/main.c lpc176x/gpio.c
src-y += generic/armcm_boot.c generic/armcm_irq.c generic/armcm_timer.c
src-y += generic/armcm_reset.c generic/crc16_ccitt.c
src-y += ../lib/lpc176x/device/system_LPC17xx.c
src-$(CONFIG_WANT_ADC) += lpc176x/adc.c
src-$(CONFIG_WANT_SPI) += lpc176x/spi.c
src-$(CONFIG_WANT_I2C) += lpc176x/i2c.c
src-$(CONFIG_USBSERIAL) += lpc176x/usbserial.c lpc176x/chipid.c
src-$(CONFIG_USBSERIAL) += generic/usb_cdc.c
src-$(CONFIG_SERIAL) += lpc176x/serial.c generic/serial_irq.c
src-$(CONFIG_WANT_HARD_PWM) += lpc176x/hard_pwm.c

# Build the additional bin output file
target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating bin file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

# Flash rules
flash: $(OUT)klipper.bin
	@echo "  Flashing $< to $(FLASH_DEVICE)"
	$(Q)$(PYTHON) ./scripts/flash_usb.py -t $(CONFIG_MCU) -d "$(FLASH_DEVICE)" $(if $(NOSUDO),--no-sudo) $(OUT)klipper.bin
