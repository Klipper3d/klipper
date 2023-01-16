# Additional atsamd build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/atsamd src/generic lib/fast-hash

MCU := $(shell echo $(CONFIG_MCU) | tr a-z A-Z)

CFLAGS-$(CONFIG_MACH_SAMC21) += -mcpu=cortex-m0plus -Ilib/samc21/samc21/include
CFLAGS-$(CONFIG_MACH_SAMD21) += -mcpu=cortex-m0plus -Ilib/samd21/samd21a/include
CFLAGS-$(CONFIG_MACH_SAMD51) += -Ilib/samd51/samd51a/include
CFLAGS-$(CONFIG_MACH_SAME51) += -Ilib/same51/include
CFLAGS-$(CONFIG_MACH_SAMX5) += -mcpu=cortex-m4 -Ilib/same54/include
CFLAGS += $(CFLAGS-y) -D__$(MCU)__ -mthumb -Ilib/cmsis-core -Ilib/fast-hash

CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs
CFLAGS_klipper.elf += -T $(OUT)src/generic/armcm_link.ld
$(OUT)klipper.elf: $(OUT)src/generic/armcm_link.ld

# Add source files
src-y += atsamd/main.c atsamd/gpio.c generic/crc16_ccitt.c
src-y += generic/armcm_boot.c generic/armcm_irq.c generic/armcm_reset.c
src-$(CONFIG_USBSERIAL) += atsamd/usbserial.c atsamd/chipid.c generic/usb_cdc.c
src-$(CONFIG_SERIAL) += atsamd/serial.c generic/serial_irq.c
canbus-src-y := generic/canserial.c ../lib/fast-hash/fasthash.c
canbus-src-y += atsamd/fdcan.c atsamd/chipid.c
src-$(CONFIG_USBCANBUS) += $(canbus-src-y) atsamd/usbserial.c generic/usb_canbus.c
src-$(CONFIG_CANSERIAL) += $(canbus-src-y) generic/canbus.c
src-$(CONFIG_HAVE_GPIO_ADC) += atsamd/adc.c
src-$(CONFIG_HAVE_GPIO_I2C) += atsamd/i2c.c
src-$(CONFIG_HAVE_GPIO_SPI) += atsamd/spi.c
src-$(CONFIG_HAVE_SERCOM) += atsamd/sercom.c
src-$(CONFIG_HAVE_GPIO_HARD_PWM) += atsamd/hard_pwm.c
src-$(CONFIG_MACH_SAMC21) += atsamd/samd51_watchdog.c
src-$(CONFIG_MACH_SAMC21) += atsamd/samc21_clock.c atsamd/timer.c generic/timer_irq.c
src-$(CONFIG_MACH_SAMD21) += atsamd/watchdog.c
src-$(CONFIG_MACH_SAMD21) += atsamd/clock.c atsamd/timer.c generic/timer_irq.c
src-$(CONFIG_MACH_SAMX5) += atsamd/samd51_watchdog.c
src-$(CONFIG_MACH_SAMX5) += atsamd/samd51_clock.c generic/armcm_timer.c

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
BOSSAC_OFFSET-$(CONFIG_MACH_SAMX5) := 0x4000

flash: $(OUT)klipper.bin lib/bossac/bin/bossac
	@echo "  Flashing $< to $(FLASH_DEVICE)"
	$(Q)$(PYTHON) ./scripts/flash_usb.py -t $(CONFIG_MCU) -d "$(FLASH_DEVICE)" -s "$(BOSSAC_OFFSET-y)" $(OUT)klipper.bin
