# Additional STM32F1 build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/stm32f1 src/generic
dirs-y += lib/cmsis-stm32f1/source
dirs-y += lib/hal-stm32f1/source

CFLAGS += -mthumb -mcpu=cortex-m3
CFLAGS += -Ilib/cmsis-stm32f1/include -Ilib/cmsis-stm32f1/cmsis-include
CFLAGS += -Ilib/hal-stm32f1/include
CFLAGS += -DSTM32F103xB
CFLAGS += -O3

CFLAGS_klipper.elf += -Llib/cmsis-stm32f1/source/
CFLAGS_klipper.elf += -Tlib/cmsis-stm32f1/source/stm32f1.ld
CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs

# Extra build rules
$(OUT)%.o: %.s $(OUT)autoconf.h $(OUT)board-link
	@echo "  Assembling $@"
	$(Q)$(AS) $< -o $@

# Add source files
src-y += stm32f1/main.c stm32f1/timer.c stm32f1/gpio.c
src-y += $(addprefix ../, $(wildcard lib/hal-stm32f1/source/stm32f1xx_ll_*.c))
src-y += generic/crc16_ccitt.c generic/armcm_irq.c generic/timer_irq.c
src-y += ../lib/cmsis-stm32f1/source/system_stm32f1xx.c
src-ys = ../lib/cmsis-stm32f1/source/startup_stm32f103xb.s
src-$(CONFIG_SERIAL) += stm32f1/serial.c generic/serial_irq.c

# Build the additional hex output file
target-y += $(OUT)klipper.bin

# Add assembler objects to prerequisites list
$(OUT)klipper.elf: $(patsubst %.s, $(OUT)src/%.o,$(src-ys))

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating hex file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

flash: $(OUT)klipper.bin
	@echo "  Flashing $^ to $(FLASH_DEVICE) via stm32flash"
	$(Q)stm32flash -w $(OUT)klipper.bin -v -g 0 $(FLASH_DEVICE)
