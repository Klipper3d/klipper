# lpc176x build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/lpc176x src/generic
dirs-y += lib/lpc176x/device/TOOLCHAIN_GCC_ARM

CFLAGS += -mthumb -mcpu=cortex-m3
CFLAGS += -Ilib/lpc176x/device -Ilib/cmsis-core

# Add source files
src-y += lpc176x/main.c lpc176x/gpio.c
src-$(CONFIG_HAVE_GPIO_ADC) += lpc176x/adc.c
src-$(CONFIG_HAVE_GPIO_I2C) += lpc176x/i2c.c
src-$(CONFIG_HAVE_GPIO_SPI) += lpc176x/spi.c
src-y += generic/crc16_ccitt.c generic/alloc.c
src-y += generic/armcm_irq.c generic/armcm_timer.c
src-y += ../lib/lpc176x/device/system_LPC17xx.c
src-$(CONFIG_USBSERIAL) += lpc176x/usbserial.c generic/usb_cdc.c
src-$(CONFIG_SERIAL) += lpc176x/serial.c generic/serial_irq.c

# Add assembler build rules
$(OUT)%.o: %.S
	@echo "  Assembling $@"
	$(Q)$(AS) $< -o $@

asmsrc-y := ../lib/lpc176x/device/TOOLCHAIN_GCC_ARM/startup_LPC17xx.S
OBJS_klipper.elf += $(patsubst %.S, $(OUT)src/%.o,$(asmsrc-y))

# Build the linker script
$(OUT)LPC1768.ld: lib/lpc176x/device/TOOLCHAIN_GCC_ARM/LPC1768.ld $(OUT)board-link
	@echo "  Preprocessing $@"
	$(Q)$(CPP) -P -MD -MT $@ -DMBED_APP_START=$(CONFIG_FLASH_START) -DMBED_APP_SIZE="(512K - $(CONFIG_FLASH_START))" $< -o $@

CFLAGS_klipper.elf += -T $(OUT)LPC1768.ld
CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs
$(OUT)klipper.elf : $(OUT)LPC1768.ld

# Build the additional bin output file
target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating bin file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

flash: $(OUT)klipper.bin
	@echo "  Flashing $< to $(FLASH_DEVICE)"
	$(Q)$(PYTHON) ./scripts/flash_usb.py -t lpc176x -d "$(FLASH_DEVICE)" $(if $(NOSUDO),--no-sudo) $(OUT)klipper.bin
