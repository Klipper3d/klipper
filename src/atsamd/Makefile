# Additional atsamd build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/atsamd src/generic
dirs-$(CONFIG_MACH_SAMD21) += lib/samd21/samd21a/gcc/gcc/
dirs-$(CONFIG_MACH_SAMD51) += lib/samd51/samd51a/gcc/gcc/

CFLAGS-$(CONFIG_MACH_SAMD21) += -mcpu=cortex-m0plus -Ilib/samd21/samd21a/include
CFLAGS-$(CONFIG_MACH_SAMD51) += -mcpu=cortex-m4 -Ilib/samd51/samd51a/include
CFLAGS-$(CONFIG_MACH_SAMD51) += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS-$(CONFIG_MACH_SAMD21G18) += -D__SAMD21G18A__
CFLAGS-$(CONFIG_MACH_SAMD51G19) += -D__SAMD51G19A__
CFLAGS-$(CONFIG_MACH_SAMD51J19) += -D__SAMD51J19A__
CFLAGS-$(CONFIG_MACH_SAMD51N19) += -D__SAMD51N19A__
CFLAGS-$(CONFIG_MACH_SAMD51P20) += -D__SAMD51P20A__
CFLAGS += $(CFLAGS-y) -mthumb -Ilib/cmsis-core

CFLAGS_klipper.elf += -T $(OUT)samd.ld --specs=nano.specs --specs=nosys.specs

# Add source files
src-y += atsamd/main.c atsamd/gpio.c
src-y += generic/crc16_ccitt.c generic/alloc.c
src-y += generic/armcm_irq.c generic/timer_irq.c
src-$(CONFIG_USBSERIAL) += atsamd/usbserial.c generic/usb_cdc.c
src-$(CONFIG_SERIAL) += atsamd/serial.c generic/serial_irq.c
src-$(CONFIG_HAVE_GPIO_ADC) += atsamd/adc.c
src-$(CONFIG_HAVE_GPIO_I2C) += atsamd/i2c.c
src-$(CONFIG_HAVE_GPIO_SPI) += atsamd/spi.c
src-$(CONFIG_HAVE_GPIO_HARD_PWM) += atsamd/hard_pwm.c
src-$(CONFIG_MACH_SAMD21) += atsamd/clock.c atsamd/timer.c
src-$(CONFIG_MACH_SAMD21) += ../lib/samd21/samd21a/gcc/gcc/startup_samd21.c
src-$(CONFIG_MACH_SAMD51) += atsamd/samd51_clock.c atsamd/samd51_timer.c
src-$(CONFIG_MACH_SAMD51) += ../lib/samd51/samd51a/gcc/gcc/startup_samd51.c

# Support bootloader offset address
target-y := $(OUT)samd.ld $(target-y)

ldfile-$(CONFIG_MACH_SAMD21G18) := lib/samd21/samd21a/gcc/gcc/samd21g18a_flash.ld
ldfile-$(CONFIG_MACH_SAMD51G19) := lib/samd51/samd51a/gcc/gcc/samd51g19a_flash.ld
ldfile-$(CONFIG_MACH_SAMD51J19) := lib/samd51/samd51a/gcc/gcc/samd51j19a_flash.ld
ldfile-$(CONFIG_MACH_SAMD51N19) := lib/samd51/samd51a/gcc/gcc/samd51n19a_flash.ld
ldfile-$(CONFIG_MACH_SAMD51P20) := lib/samd51/samd51a/gcc/gcc/samd51p20a_flash.ld

$(OUT)samd.ld: $(ldfile-y) $(OUT)board-link
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

BOSSAC_OFFSET-$(CONFIG_MACH_SAMD21) := 0x2000
BOSSAC_OFFSET-$(CONFIG_MACH_SAMD51) := 0x4000

flash: $(OUT)klipper.bin lib/bossac/bin/bossac
	@echo "  Flashing $^ to $(FLASH_DEVICE) via bossac"
	$(Q)if [ -z $(FLASH_DEVICE) ]; then echo "Please specify FLASH_DEVICE"; exit 1; fi
	$(Q)lib/bossac/bin/bossac -U -p "$(FLASH_DEVICE)" -a --offset=$(BOSSAC_OFFSET-y) -w $(OUT)klipper.bin -v -b -R
