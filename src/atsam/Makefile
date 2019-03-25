# Additional ATSAM build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/atsam src/generic
dirs-$(CONFIG_MACH_SAM3X) += lib/sam3x/gcc/gcc
dirs-$(CONFIG_MACH_SAM4S) += lib/sam4s/gcc/gcc
dirs-$(CONFIG_MACH_SAM4E) += lib/sam4e/gcc/gcc

CFLAGS-$(CONFIG_MACH_SAM3X) += -mcpu=cortex-m3 -falign-loops=16
CFLAGS-$(CONFIG_MACH_SAM4) += -mcpu=cortex-m4
CFLAGS-$(CONFIG_MACH_SAM4E) += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS-$(CONFIG_MACH_SAM3X) += -Ilib/sam3x/include
CFLAGS-$(CONFIG_MACH_SAM4S) += -Ilib/sam4s/include
CFLAGS-$(CONFIG_MACH_SAM4E) += -Ilib/sam4e/include
CFLAGS-$(CONFIG_MACH_SAM3X8E) += -D__SAM3X8E__
CFLAGS-$(CONFIG_MACH_SAM3X8C) += -D__SAM3X8C__
CFLAGS-$(CONFIG_MACH_SAM4S8C) += -D__SAM4S8C__
CFLAGS-$(CONFIG_MACH_SAM4E8E) += -D__SAM4E8E__
CFLAGS += -mthumb $(CFLAGS-y) -Ilib/cmsis-core

eflags-$(CONFIG_MACH_SAM3X) += -Llib/sam3x/gcc/gcc
eflags-$(CONFIG_MACH_SAM3X) += -T lib/sam3x/gcc/gcc/sam3x8e_flash.ld
eflags-$(CONFIG_MACH_SAM4S) += -Llib/sam4s/gcc/gcc
eflags-$(CONFIG_MACH_SAM4S) += -T lib/sam4s/gcc/gcc/sam4s8c_flash.ld
eflags-$(CONFIG_MACH_SAM4E) += -Llib/sam4e/gcc/gcc
eflags-$(CONFIG_MACH_SAM4E) += -T lib/sam4e/gcc/gcc/sam4e8e_flash.ld
CFLAGS_klipper.elf += $(eflags-y) --specs=nano.specs --specs=nosys.specs

# Add source files
src-y += atsam/main.c atsam/gpio.c atsam/i2c.c atsam/spi.c atsam/hard_pwm.c
src-y += generic/crc16_ccitt.c generic/alloc.c
src-y += generic/armcm_irq.c generic/armcm_timer.c
usbserial-$(CONFIG_MACH_SAM3X) := atsam/sam3_usb.c
usbserial-$(CONFIG_MACH_SAM4) := atsam/sam4_usb.c
src-$(CONFIG_USBSERIAL) += $(usbserial-y) generic/usb_cdc.c
src-$(CONFIG_SERIAL) += atsam/serial.c generic/serial_irq.c
src-$(CONFIG_MACH_SAM3X) += atsam/adc.c
src-$(CONFIG_MACH_SAM4S) += atsam/adc.c
src-$(CONFIG_MACH_SAM4E) += atsam/sam4e_afec.c atsam/sam4_cache.c
src-$(CONFIG_MACH_SAM3X) += ../lib/sam3x/gcc/system_sam3xa.c
src-$(CONFIG_MACH_SAM3X) += ../lib/sam3x/gcc/gcc/startup_sam3xa.c
src-$(CONFIG_MACH_SAM4S) += atsam/sam4s_sysinit.c
src-$(CONFIG_MACH_SAM4S) += ../lib/sam4s/gcc/gcc/startup_sam4s.c
src-$(CONFIG_MACH_SAM4E) += ../lib/sam4e/gcc/system_sam4e.c
src-$(CONFIG_MACH_SAM4E) += ../lib/sam4e/gcc/gcc/startup_sam4e.c

# Build the additional hex output file
target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating bin file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

# Flash rules
lib/bossac/bin/bossac:
	@echo "  Building bossac"
	$(Q)make -C lib/bossac bin/bossac

FLASH_TYPE-$(CONFIG_MACH_SAM3X) := atsam3
FLASH_TYPE-$(CONFIG_MACH_SAM4) := atsam4

flash: $(OUT)klipper.bin lib/bossac/bin/bossac
	@echo "  Flashing $< to $(FLASH_DEVICE)"
	$(Q)$(PYTHON) ./scripts/flash_usb.py -t $(FLASH_TYPE-y) -d "$(FLASH_DEVICE)" $(OUT)klipper.bin
