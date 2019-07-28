# Additional STM32F4 build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/stm32f4 src/generic
dirs-$(CONFIG_MACH_STM32F1xx) += lib/stm32f1 lib/stm32f1/gcc
dirs-$(CONFIG_MACH_STM32F4xx) += lib/stm32f4 lib/stm32f4/gcc

MCU := $(shell echo $(CONFIG_MCU))
MCU_UPPER := $(shell echo $(CONFIG_MCU) | tr a-z A-Z | tr X x)

CFLAGS-$(CONFIG_MACH_STM32F1xx) += -mcpu=cortex-m3 -Ilib/stm32f1/include
CFLAGS-$(CONFIG_MACH_STM32F4xx) += -mcpu=cortex-m4 -Ilib/stm32f4/include
CFLAGS-$(CONFIG_MACH_STM32F4xx) += -mfpu=fpv4-sp-d16 -mfloat-abi=hard

CFLAGS += $(CFLAGS-y) -D$(MCU_UPPER) -mthumb -Ilib/cmsis-core
CFLAGS_klipper.elf += -T $(OUT)stm32f4.ld --specs=nano.specs --specs=nosys.specs

# Add source files
src-y += stm32f4/main.c stm32f4/watchdog.c stm32f4/gpio.c
src-y += generic/crc16_ccitt.c generic/armcm_irq.c generic/armcm_timer.c
src-$(CONFIG_MACH_STM32F1xx) += ../lib/stm32f1/system_stm32f1xx.c
src-$(CONFIG_MACH_STM32F1xx) += stm32f4/stm32f1.c
src-$(CONFIG_MACH_STM32F4xx) += ../lib/stm32f4/system_stm32f4xx.c
src-$(CONFIG_MACH_STM32F4xx) += stm32f4/clock.c
src-$(CONFIG_HAVE_GPIO_ADC) += stm32f4/adc.c
src-$(CONFIG_HAVE_GPIO_SPI) += stm32f4/spi.c
src-$(CONFIG_USBSERIAL) += stm32f4/usbfs.c generic/usb_cdc.c
src-$(CONFIG_SERIAL) += stm32f4/serial.c generic/serial_irq.c

# Add assembler build rules
$(OUT)%.o: %.s $(OUT)autoconf.h $(OUT)board-link
	@echo "  Assembling $@"
	$(Q)$(AS) $< -o $@

asmsrc-$(CONFIG_MACH_STM32F1xx) := ../lib/stm32f1/gcc/startup_$(MCU).s
asmsrc-$(CONFIG_MACH_STM32F4xx) := ../lib/stm32f4/gcc/startup_$(MCU).s
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

FLASH_TYPE-$(CONFIG_MACH_STM32F1xx) := stm32f1
FLASH_TYPE-$(CONFIG_MACH_STM32F4xx) := stm32f4

flash: $(OUT)klipper.bin
	@echo "  Flashing $< to $(FLASH_DEVICE)"
	$(Q)$(PYTHON) ./scripts/flash_usb.py -t $(FLASH_TYPE-y) -d "$(FLASH_DEVICE)" $(if $(NOSUDO),--no-sudo) $(OUT)klipper.bin

serialflash: $(OUT)klipper.bin
	@echo "  Flashing $< to $(FLASH_DEVICE) via stm32flash"
	$(Q)stm32flash -w $< -v -g 0 $(FLASH_DEVICE)
