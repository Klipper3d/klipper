# Additional SAM3/SAM4 build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/sam3 src/generic
dirs-$(CONFIG_MACH_SAM3X8E) += lib/sam3x/gcc/gcc
dirs-$(CONFIG_MACH_SAM4S8C) += lib/sam4s/gcc/gcc
dirs-$(CONFIG_MACH_SAM4E8E) += lib/sam4e/gcc/gcc

CFLAGS-$(CONFIG_MACH_SAM3X8E) += -mcpu=cortex-m3 -falign-loops=16
CFLAGS-$(CONFIG_MACH_SAM3X8E) += -Ilib/sam3x/include -D__SAM3X8E__
CFLAGS-$(CONFIG_MACH_SAM4S8C) += -mcpu=cortex-m4
CFLAGS-$(CONFIG_MACH_SAM4S8C) += -Ilib/sam4s/include -D__SAM4S8C__
CFLAGS-$(CONFIG_MACH_SAM4E8E) += -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS-$(CONFIG_MACH_SAM4E8E) += -Ilib/sam4e/include -D__SAM4E8E__
CFLAGS += -mthumb $(CFLAGS-y) -Ilib/cmsis-core

eflags-$(CONFIG_MACH_SAM3X8E) += -Llib/sam3x/gcc/gcc
eflags-$(CONFIG_MACH_SAM3X8E) += -T lib/sam3x/gcc/gcc/sam3x8e_flash.ld
eflags-$(CONFIG_MACH_SAM4S8C) += -Llib/sam4s/gcc/gcc
eflags-$(CONFIG_MACH_SAM4S8C) += -T lib/sam4s/gcc/gcc/sam4s8c_flash.ld
eflags-$(CONFIG_MACH_SAM4E8E) += -Llib/sam4e/gcc/gcc
eflags-$(CONFIG_MACH_SAM4E8E) += -T lib/sam4e/gcc/gcc/sam4e8e_flash.ld
CFLAGS_klipper.elf += $(eflags-y) --specs=nano.specs --specs=nosys.specs

# Add source files
src-y += sam3/main.c sam3/gpio.c sam3/i2c.c sam3/spi.c
src-y += generic/crc16_ccitt.c generic/alloc.c
src-y += generic/armcm_irq.c generic/timer_irq.c
src-$(CONFIG_USBSERIAL) += sam3/sam4_usb.c generic/usb_cdc.c
src-$(CONFIG_SERIAL) += sam3/serial.c generic/serial_irq.c
src-$(CONFIG_MACH_SAM3X8E) += sam3/adc.c sam3/timer.c
src-$(CONFIG_MACH_SAM3X8E) += ../lib/sam3x/gcc/system_sam3xa.c
src-$(CONFIG_MACH_SAM3X8E) += ../lib/sam3x/gcc/gcc/startup_sam3xa.c
src-$(CONFIG_MACH_SAM4S8C) += sam3/adc.c sam3/sam4s_timer.c sam3/sam4s_sysinit.c
src-$(CONFIG_MACH_SAM4S8C) += ../lib/sam4s/gcc/gcc/startup_sam4s.c
src-$(CONFIG_MACH_SAM4E8E) += sam3/sam4e_afec.c sam3/timer.c sam3/sam4_cache.c
src-$(CONFIG_MACH_SAM4E8E) += ../lib/sam4e/gcc/system_sam4e.c
src-$(CONFIG_MACH_SAM4E8E) += ../lib/sam4e/gcc/gcc/startup_sam4e.c

# Build the additional hex output file
target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating bin file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

# Flash rules
lib/bossac/bin/bossac:
	@echo "  Building bossac"
	$(Q)make -C lib/bossac bin/bossac

flash: $(OUT)klipper.bin lib/bossac/bin/bossac
	@echo "  Flashing $^ to $(FLASH_DEVICE) via bossac"
	$(Q)if [ -z $(FLASH_DEVICE) ]; then echo "Please specify FLASH_DEVICE"; exit 1; fi
	$(Q)lib/bossac/bin/bossac -U -p "$(FLASH_DEVICE)" -a -e -w $(OUT)klipper.bin -v -b
	$(Q)lib/bossac/bin/bossac -p "$(FLASH_DEVICE)" -b -R > /dev/null 2>&1 || true
