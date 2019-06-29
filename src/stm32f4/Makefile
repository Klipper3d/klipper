# Additional STM32F1 build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/stm32f4 src/generic
dirs-y += lib/cmsis-stm32f4/source
dirs-y += lib/hal-stm32f4/source
dirs-y += lib/libusb_stm32/src

CFLAGS += -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -Wno-attributes
CFLAGS += -Ilib/cmsis-core
CFLAGS += -Ilib/cmsis-stm32f4/include -Ilib/hal-stm32f4/include
CFLAGS += -Ilib/libusb_stm32/inc
CFLAGS += -DSTM32F405xx -DSTM32F4

# Add source files
src-y += stm32f4/main.c stm32f4/gpio.c stm32f4/rcc.c
src-$(CONFIG_HAVE_GPIO_ADC) += stm32f4/adc.c
src-$(CONFIG_HAVE_GPIO_SPI) += stm32f4/spi.c
src-y += $(addprefix ../, $(wildcard lib/hal-stm32f4/source/stm32f4xx_ll_*.c))
src-y += generic/crc16_ccitt.c generic/armcm_irq.c generic/armcm_timer.c
src-y += ../lib/cmsis-stm32f4/source/system_stm32f4xx.c
src-y += ../lib/libusb_stm32/src/usbd_core.c
src-y += ../lib/libusb_stm32/src/usbd_stm32f429_otgfs.c

# Add assembler build rules
$(OUT)%.o: %.s $(OUT)autoconf.h $(OUT)board-link
	@echo "  Assembling $@"
	$(Q)$(AS) $< -o $@

asmsrc-y = ../lib/cmsis-stm32f4/source/startup_stm32f405xx.s
OBJS_klipper.elf += $(patsubst %.s, $(OUT)src/%.o,$(asmsrc-y))

# Build the linker script
$(OUT)stm32f405rgtx.ld: src/stm32f4/stm32f405rgtx.ld $(OUT)board-link
	@echo "  Preprocessing $@"
	$(Q)$(CPP) -P -MD -MT $@ -DFLASH_START=$(CONFIG_FLASH_START) $< -o $@

CFLAGS_klipper.elf += -T $(OUT)stm32f405rgtx.ld
CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs
$(OUT)klipper.elf : $(OUT)stm32f405rgtx.ld

# Binary output file rules
target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating hex file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

flash: $(OUT)klipper.bin
	@echo "  Flashing $< to $(FLASH_DEVICE)"
	$(Q)$(PYTHON) ./scripts/flash_usb.py -t stm32f4 -d "$(FLASH_DEVICE)" $(if $(NOSUDO),--no-sudo) $(OUT)klipper.bin

serialflash: $(OUT)klipper.bin
	@echo "  Flashing $< to $(FLASH_DEVICE) via stm32flash"
	$(Q)stm32flash -w $< -v -g 0 $(FLASH_DEVICE)
