# Additional STM32F1 build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/stm32f4 src/generic
dirs-y += lib/cmsis-stm32f4/source
dirs-y += lib/hal-stm32f4/source

CFLAGS += -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -Ilib/cmsis-core
CFLAGS += -Ilib/cmsis-stm32f4/include -Ilib/hal-stm32f4/include
CFLAGS += -DSTM32F407xx  -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 
CFLAGS += -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 
CFLAGS += -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 


CFLAGS_klipper.elf += -T $(OUT)stm32f4.ld
CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs

# Add source files
src-y += stm32f4/main.c stm32f4/timer.c stm32f4/gpio.c
src-$(CONFIG_HAVE_GPIO_ADC) += stm32f4/adc.c
src-$(CONFIG_HAVE_GPIO_SPI) += stm32f4/spi.c
src-y += $(addprefix ../, $(wildcard lib/hal-stm32f4/source/stm32f4xx_ll_*.c))
src-y += generic/crc16_ccitt.c generic/armcm_irq.c generic/timer_irq.c
src-y += ../lib/cmsis-stm32f4/source/system_stm32f4xx.c
src-$(CONFIG_USBSERIAL) += stm32f4/usbserial.c generic/usb_cdc.c
src-$(CONFIG_SERIAL) += stm32f4/serial.c generic/serial_irq.c
asmsrc-y = ../lib/cmsis-stm32f4/source/startup_stm32f407xx.s

# Add assembler build rules
$(OUT)%.o: %.s $(OUT)autoconf.h $(OUT)board-link
	@echo "  Assembling $@"
	$(Q)$(AS) $< -o $@

$(OUT)klipper.elf: $(patsubst %.s, $(OUT)src/%.o,$(asmsrc-y))

# Build the linker script
target-y := $(OUT)stm32f4.ld $(target-y)

$(OUT)stm32f4.ld: src/stm32f4/stm32f4.ld $(OUT)board-link
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
