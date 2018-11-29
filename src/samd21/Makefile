# Additional samd21 build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/samd21 src/generic
dirs-y += lib/samd21/samd21a/gcc/gcc/

CFLAGS += -mthumb -mcpu=cortex-m0plus
CFLAGS += -Ilib/cmsis-core -Ilib/samd21/samd21a/include
CFLAGS += -D__SAMD21G18A__

CFLAGS_klipper.elf += -T $(OUT)samd21.ld
CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs

# Add source files
src-y += samd21/main.c samd21/timer.c samd21/clock.c samd21/gpio.c
src-y += generic/crc16_ccitt.c generic/alloc.c
src-y += generic/armcm_irq.c generic/timer_irq.c
src-y += ../lib/samd21/samd21a/gcc/gcc/startup_samd21.c
src-$(CONFIG_USBSERIAL) += samd21/usbserial.c generic/usb_cdc.c
src-$(CONFIG_SERIAL) += samd21/serial.c generic/serial_irq.c
src-$(CONFIG_HAVE_GPIO_I2C) += samd21/i2c.c
src-$(CONFIG_HAVE_GPIO_SPI) += samd21/spi.c

# Support bootloader offset address
target-y := $(OUT)samd21.ld $(target-y)

$(OUT)samd21.ld: lib/samd21/samd21a/gcc/gcc/samd21g18a_flash.ld $(OUT)board-link
	@echo "  Preprocessing $@"
	$(Q)$(CPP) -P -MD -MT $@ -DFLASH_START=$(CONFIG_FLASH_START) $< -o $@

# Build the additional hex and bin output files
target-y += $(OUT)klipper.bin $(OUT)klipper.elf.hex

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating hex file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

$(OUT)klipper.elf.hex: $(OUT)klipper.elf
	@echo "  Creating hex file $@"
	$(Q)$(OBJCOPY) -j .text -j .relocate -O ihex $< $@

flash: $(OUT)klipper.bin
	@echo ""
	@echo "  The SAMD21 build does not currently support 'make flash'"
