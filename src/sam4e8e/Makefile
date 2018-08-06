# Additional sam4e8e build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-
dirs-y += src/sam4e8e src/generic

CFLAGS += -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -D__SAM4E8E__

CFLAGS_klipper.elf += -L lib/cmsis-sam4e/gcc/gcc
CFLAGS_klipper.elf += -T lib/cmsis-sam4e/gcc/gcc/sam4e8e_flash.ld
CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs

dirs-y += lib/cmsis-sam4e/gcc \
		  lib/cmsis-sam4e/gcc/gcc
CFLAGS += -Ilib/cmsis-sam4e/include \
		  -Ilib/cmsis-core
src-y +=  ../lib/cmsis-sam4e/gcc/system_sam4e.c \
		  ../lib/cmsis-sam4e/gcc/gcc/startup_sam4e.c

src-$(CONFIG_HAVE_GPIO_SPI) += sam4e8e/spi.c
src-$(CONFIG_SERIAL) += sam4e8e/serial.c generic/serial_irq.c
src-$(CONFIG_HAVE_GPIO) += sam4e8e/gpio.c
src-y += generic/crc16_ccitt.c generic/alloc.c
src-y += generic/armcm_irq.c generic/timer_irq.c
src-y += sam4e8e/main.c sam4e8e/timer.c

# Build the additional hex output file
target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating bin file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

flash: $(OUT)klipper.bin
	@echo "  Flashing $^ to $(FLASH_DEVICE) via bossac"
	$(Q)tools/bossa/bin/bossac --port="$(FLASH_DEVICE)" -b -U -e -w -v $(OUT)klipper.bin -R
