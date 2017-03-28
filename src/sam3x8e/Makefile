# Additional sam3x8e build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/sam3x8e src/generic
dirs-y += lib/cmsis-sam3x8e/source lib/cmsis-sam3x8e/source/gcc

CFLAGS-y += -mthumb -mcpu=cortex-m3
CFLAGS-y += -Ilib/cmsis-sam3x8e/include -Ilib/cmsis-sam3x8e/cmsis-include
CFLAGS-y += -D__SAM3X8E__

LDFLAGS-y += -Llib/cmsis-sam3x8e/source/gcc
LDFLAGS-y += -T lib/cmsis-sam3x8e/source/gcc/sam3x8e_flash.ld
LDFLAGS-y += --specs=nano.specs --specs=nosys.specs

# Add source files
src-y += sam3x8e/main.c sam3x8e/timer.c sam3x8e/gpio.c
src-y += generic/crc16_ccitt.c generic/armcm_irq.c generic/timer_irq.c
src-y += ../lib/cmsis-sam3x8e/source/system_sam3xa.c
src-y += ../lib/cmsis-sam3x8e/source/gcc/startup_sam3xa.c
src-$(CONFIG_SERIAL) += sam3x8e/serial.c

# Build the additional hex output file
target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating hex file $@"
	$(Q)$(OBJCOPY) -O binary $< $@
