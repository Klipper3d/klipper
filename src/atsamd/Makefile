# Additional atsamd build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/atsamd src/generic
dirs-$(CONFIG_MACH_SAMD21) += lib/samd21/samd21a/gcc/gcc/
dirs-$(CONFIG_MACH_SAMD51) += lib/samd51/samd51a/gcc/gcc/

MCU := $(shell echo $(CONFIG_MCU) | tr a-z A-Z)

CFLAGS-$(CONFIG_MACH_SAMD21) += -mcpu=cortex-m0plus -Ilib/samd21/samd21a/include
CFLAGS-$(CONFIG_MACH_SAMD51) += -mcpu=cortex-m4 -Ilib/samd51/samd51a/include
CFLAGS-$(CONFIG_MACH_SAMD51) += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += $(CFLAGS-y) -D__$(MCU)__ -mthumb -Ilib/cmsis-core

CFLAGS_klipper.elf += -T $(OUT)samd.ld --specs=nano.specs --specs=nosys.specs

# Add source files
src-y += atsamd/main.c atsamd/gpio.c generic/crc16_ccitt.c generic/armcm_irq.c
src-$(CONFIG_USBSERIAL) += atsamd/usbserial.c generic/usb_cdc.c
src-$(CONFIG_SERIAL) += atsamd/serial.c generic/serial_irq.c
src-$(CONFIG_HAVE_GPIO_ADC) += atsamd/adc.c
src-$(CONFIG_HAVE_GPIO_I2C) += atsamd/i2c.c
src-$(CONFIG_HAVE_GPIO_SPI) += atsamd/spi.c
src-$(CONFIG_HAVE_SERCOM) += atsamd/sercom.c
src-$(CONFIG_HAVE_GPIO_HARD_PWM) += atsamd/hard_pwm.c
src-$(CONFIG_MACH_SAMD21) += atsamd/watchdog.c
src-$(CONFIG_MACH_SAMD21) += atsamd/clock.c atsamd/timer.c generic/timer_irq.c
src-$(CONFIG_MACH_SAMD21) += ../lib/samd21/samd21a/gcc/gcc/startup_samd21.c
src-$(CONFIG_MACH_SAMD51) += atsamd/samd51_watchdog.c
src-$(CONFIG_MACH_SAMD51) += atsamd/samd51_clock.c generic/armcm_timer.c
src-$(CONFIG_MACH_SAMD51) += ../lib/samd51/samd51a/gcc/gcc/startup_samd51.c

# Support bootloader offset address
target-y := $(target-y)

$(OUT)samd.ld: src/atsamd/samd.lds.S $(OUT)board-link
	@echo "  Preprocessing $@"
	$(Q)$(CPP) -I$(OUT) -P -MD -MT $@ $< -o $@

$(OUT)klipper.elf: $(OUT)samd.ld

# Build the additional hex and bin output files
target-y += $(OUT)klipper.bin $(OUT)klipper.elf.hex

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating bin file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

$(OUT)klipper.elf.hex: $(OUT)klipper.elf
	@echo "  Creating hex file $@"
	$(Q)$(OBJCOPY) -j .text -j .relocate -O ihex $< $@

# Flash rules
lib/bossac/bin/bossac:
	@echo "  Building bossac"
	$(Q)make -C lib/bossac bin/bossac

BOSSAC_OFFSET-$(CONFIG_MACH_SAMD21) := 0x2000
BOSSAC_OFFSET-$(CONFIG_MACH_SAMD51) := 0x4000

flash: $(OUT)klipper.bin lib/bossac/bin/bossac
	@echo "  Flashing $< to $(FLASH_DEVICE)"
	$(Q)$(PYTHON) ./scripts/flash_usb.py -t atsamd -d "$(FLASH_DEVICE)" -o $(BOSSAC_OFFSET-y) $(OUT)klipper.bin
