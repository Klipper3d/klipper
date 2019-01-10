# Additional atsamd build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/atsamd src/generic
dirs-$(CONFIG_MACH_SAMD21A) += lib/samd21/samd21a/gcc/gcc/

CFLAGS-$(CONFIG_MACH_SAMD21A) += -mcpu=cortex-m0plus
CFLAGS-$(CONFIG_MACH_SAMD21A) += -Ilib/samd21/samd21a/include -D__SAMD21G18A__
CFLAGS += $(CFLAGS-y) -mthumb -Ilib/cmsis-core

eflags-$(CONFIG_MACH_SAMD21A) += -T $(OUT)samd21a.ld
CFLAGS_klipper.elf += $(eflags-y) --specs=nano.specs --specs=nosys.specs

# Add source files
src-y += atsamd/main.c atsamd/timer.c atsamd/clock.c atsamd/gpio.c
src-y += generic/crc16_ccitt.c generic/alloc.c
src-y += generic/armcm_irq.c generic/timer_irq.c
src-$(CONFIG_USBSERIAL) += atsamd/usbserial.c generic/usb_cdc.c
src-$(CONFIG_SERIAL) += atsamd/serial.c generic/serial_irq.c
src-$(CONFIG_HAVE_GPIO_ADC) += atsamd/adc.c
src-$(CONFIG_HAVE_GPIO_I2C) += atsamd/i2c.c
src-$(CONFIG_HAVE_GPIO_SPI) += atsamd/spi.c
src-$(CONFIG_HAVE_GPIO_HARD_PWM) += atsamd/hard_pwm.c
src-$(CONFIG_MACH_SAMD21A) += ../lib/samd21/samd21a/gcc/gcc/startup_samd21.c

# Support bootloader offset address
target-$(CONFIG_MACH_SAMD21A) := $(OUT)samd21a.ld $(target-y)

$(OUT)samd21a.ld: lib/samd21/samd21a/gcc/gcc/samd21g18a_flash.ld $(OUT)board-link
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

# Flash rules
lib/bossac/bin/bossac:
	@echo "  Building bossac"
	$(Q)make -C lib/bossac bin/bossac

flash: $(OUT)klipper.bin lib/bossac/bin/bossac
	@echo "  Flashing $^ to $(FLASH_DEVICE) via bossac"
	$(Q)if [ -z $(FLASH_DEVICE) ]; then echo "Please specify FLASH_DEVICE"; exit 1; fi
	$(Q)lib/bossac/bin/bossac -U -p "$(FLASH_DEVICE)" -a --offset=0x2000 -w $(OUT)klipper.bin -v -b -R
