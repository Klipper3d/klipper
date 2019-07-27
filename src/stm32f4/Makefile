# Additional STM32F4 build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/stm32f4 src/generic
dirs-y += lib/stm32f4 lib/stm32f4/gcc

MCU := $(shell echo $(CONFIG_MCU) | tr a-z A-Z)
CFLAGS += -D$(MCU)xx

CFLAGS += -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -Ilib/cmsis-core -Ilib/stm32f4/include

CFLAGS_klipper.elf += -T $(OUT)stm32f4.ld --specs=nano.specs --specs=nosys.specs

# Add source files
src-y += stm32f4/main.c stm32f4/clock.c stm32f4/watchdog.c stm32f4/gpio.c
src-y += generic/crc16_ccitt.c generic/armcm_irq.c generic/armcm_timer.c
src-y += ../lib/stm32f4/system_stm32f4xx.c
src-$(CONFIG_HAVE_GPIO_ADC) += stm32f4/adc.c
src-$(CONFIG_HAVE_GPIO_SPI) += stm32f4/spi.c
src-$(CONFIG_SERIAL) += stm32f4/serial.c generic/serial_irq.c

# Add assembler build rules
$(OUT)%.o: %.s $(OUT)autoconf.h $(OUT)board-link
	@echo "  Assembling $@"
	$(Q)$(AS) $< -o $@

asmsrc-y := ../lib/stm32f4/gcc/startup_$(shell echo $(CONFIG_MCU))xx.s
OBJS_klipper.elf += $(patsubst %.s, $(OUT)src/%.o,$(asmsrc-y))

# Build the linker script
$(OUT)stm32f4.ld: src/stm32f4/stm32f4.lds.S $(OUT)board-link
	@echo "  Preprocessing $@"
	$(Q)$(CPP) -I$(OUT) -P -MD -MT $@ $< -o $@
$(OUT)klipper.elf: $(OUT)stm32f4.ld

# Binary output file rules
target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating hex file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

flash: $(OUT)klipper.bin
	@echo "  Flashing $< to $(FLASH_DEVICE) via stm32flash"
	$(Q)stm32flash -w $< -v -g 0 $(FLASH_DEVICE)
