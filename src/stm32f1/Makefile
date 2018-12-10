# Additional STM32F1 build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/stm32f1 src/generic
dirs-y += lib/cmsis-stm32f1/source
dirs-y += lib/hal-stm32f1/source

CFLAGS += -mthumb -mcpu=cortex-m3
CFLAGS += -Ilib/cmsis-core
CFLAGS += -Ilib/cmsis-stm32f1/include -Ilib/hal-stm32f1/include
CFLAGS += -DSTM32F103xB

CFLAGS_klipper.elf += -T $(OUT)stm32f1.ld
CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs

# Add source files
src-y += stm32f1/main.c stm32f1/timer.c stm32f1/gpio.c
src-$(CONFIG_HAVE_GPIO_ADC) += stm32f1/adc.c
src-$(CONFIG_HAVE_GPIO_SPI) += stm32f1/spi.c
src-y += $(addprefix ../, $(wildcard lib/hal-stm32f1/source/stm32f1xx_ll_*.c))
src-y += generic/crc16_ccitt.c generic/armcm_irq.c generic/timer_irq.c
src-y += ../lib/cmsis-stm32f1/source/system_stm32f1xx.c
src-$(CONFIG_USBSERIAL) += stm32f1/usbserial.c generic/usb_cdc.c
src-$(CONFIG_SERIAL) += stm32f1/serial.c generic/serial_irq.c
asmsrc-y = ../lib/cmsis-stm32f1/source/startup_stm32f103xb.s

# Add assembler build rules
$(OUT)%.o: %.s $(OUT)autoconf.h $(OUT)board-link
	@echo "  Assembling $@"
	$(Q)$(AS) $< -o $@

$(OUT)klipper.elf: $(patsubst %.s, $(OUT)src/%.o,$(asmsrc-y))

# Build the linker script
target-y := $(OUT)stm32f1.ld $(target-y)

$(OUT)stm32f1.ld: src/stm32f1/stm32f1.ld $(OUT)board-link
	@echo "  Preprocessing $@"
	$(Q)$(CPP) -P -MD -MT $@ -DFLASH_START=$(CONFIG_FLASH_START) $< -o $@

# Binary output file rules
target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating hex file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

flash: $(OUT)klipper.bin
	@echo "  Flashing $^ to $(FLASH_DEVICE) via stm32flash"
	$(Q)stm32flash -w $(OUT)klipper.bin -v -g 0 $(FLASH_DEVICE)
