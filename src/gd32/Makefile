# GD32 platform build rules
#
# Runtime code lives in src/gd32.  Vendor device headers are kept behind the
# lib/gd32*/include boundary so platform code and vendor material do not mix.

CROSS_PREFIX=arm-none-eabi-

dirs-y += src/gd32 src/generic
dirs-$(CONFIG_CANBUS) += lib/fast-hash
dirs-$(CONFIG_MACH_GD32F30X) += lib/gd32f30x/include
dirs-$(CONFIG_MACH_GD32E23X) += lib/gd32e23x/include

CFLAGS-$(CONFIG_MACH_GD32F30X) += -DUSE_STDPERIPH_DRIVER
CFLAGS-$(CONFIG_MACH_GD32F303XB) += -DGD32F30X_HD
CFLAGS-$(CONFIG_MACH_GD32F303XC) += -DGD32F30X_HD
CFLAGS-$(CONFIG_MACH_GD32F303XE) += -DGD32F30X_XD
CFLAGS-$(CONFIG_MACH_GD32F30X) += -mcpu=cortex-m4 -Ilib/gd32f30x/include
CFLAGS-$(CONFIG_MACH_GD32E23X) += -DGD32E230 -mcpu=cortex-m23
CFLAGS-$(CONFIG_MACH_GD32E23X) += -Ilib/gd32e23x/include
CFLAGS-$(CONFIG_HAVE_LIMITED_CODE_SIZE) += -Os
CFLAGS-$(CONFIG_CANBUS) += -Ilib/fast-hash
CFLAGS += $(CFLAGS-y) -mthumb -Ilib/cmsis-core
CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs
CFLAGS_klipper.elf += -T $(OUT)src/generic/armcm_link.ld
$(OUT)klipper.elf: $(OUT)src/generic/armcm_link.ld

src-y += generic/armcm_boot.c generic/armcm_irq.c
src-y += generic/armcm_reset.c generic/crc16_ccitt.c
src-$(CONFIG_MACH_GD32F30X) += gd32/gd32f30x.c gd32/gd32f30x_gpio.c
src-$(CONFIG_MACH_GD32F30X) += generic/armcm_timer.c
src-$(CONFIG_MACH_GD32E23X) += gd32/gd32e23x.c gd32/gd32e23x_gpio.c
src-$(CONFIG_MACH_GD32E23X) += gd32/gd32e23x_timer.c generic/timer_irq.c

src-$(CONFIG_WANT_ADC) += gd32/adc.c
src-$(CONFIG_WANT_SPI) += gd32/spi.c
src-$(CONFIG_WANT_I2C) += gd32/i2c.c
src-$(CONFIG_SERIAL) += gd32/serial.c generic/serial_irq.c
src-$(CONFIG_USBSERIAL) += gd32/usbfs.c gd32/chipid.c generic/usb_cdc.c
canbus-src-y := gd32/can.c gd32/chipid.c generic/canserial.c
canbus-src-y += generic/canbus.c ../lib/fast-hash/fasthash.c
src-$(CONFIG_CANSERIAL) += $(canbus-src-y)

target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating binary $@"
	$(Q)$(OBJCOPY) -O binary $< $@

flash: $(OUT)klipper.bin
	@echo "GD32 automatic flashing is not enabled; use the board bootloader."
