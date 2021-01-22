# Additional STM32 build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/stm32 src/generic
dirs-$(CONFIG_MACH_STM32F0) += lib/stm32f0
dirs-$(CONFIG_MACH_STM32F1) += lib/stm32f1
dirs-$(CONFIG_MACH_STM32F2) += lib/stm32f2
dirs-$(CONFIG_MACH_STM32F4) += lib/stm32f4

MCU := $(shell echo $(CONFIG_MCU))
MCU_UPPER := $(shell echo $(CONFIG_MCU) | tr a-z A-Z | tr X x)

CFLAGS-$(CONFIG_MACH_STM32F0) += -mcpu=cortex-m0 -Ilib/stm32f0/include
CFLAGS-$(CONFIG_MACH_STM32F1) += -mcpu=cortex-m3 -Ilib/stm32f1/include
CFLAGS-$(CONFIG_MACH_STM32F2) += -mcpu=cortex-m3 -Ilib/stm32f2/include
CFLAGS-$(CONFIG_MACH_STM32F4) += -mcpu=cortex-m4 -Ilib/stm32f4/include
CFLAGS-$(CONFIG_MACH_STM32F4) += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += $(CFLAGS-y) -D$(MCU_UPPER) -mthumb -Ilib/cmsis-core -Ilib/fast-hash

CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs
CFLAGS_klipper.elf += -T $(OUT)src/generic/armcm_link.ld
$(OUT)klipper.elf: $(OUT)src/generic/armcm_link.ld

# Add source files
src-y += stm32/watchdog.c stm32/gpio.c generic/crc16_ccitt.c
src-y += generic/armcm_boot.c generic/armcm_irq.c generic/armcm_reset.c
src-$(CONFIG_MACH_STM32F0) += ../lib/stm32f0/system_stm32f0xx.c
src-$(CONFIG_MACH_STM32F0) += generic/timer_irq.c stm32/stm32f0_timer.c
src-$(CONFIG_MACH_STM32F0) += stm32/stm32f0.c stm32/stm32f0_adc.c
src-$(CONFIG_MACH_STM32F0) += stm32/stm32f0_i2c.c
src-$(CONFIG_MACH_STM32F1) += ../lib/stm32f1/system_stm32f1xx.c
src-$(CONFIG_MACH_STM32F1) += stm32/stm32f1.c generic/armcm_timer.c
src-$(CONFIG_MACH_STM32F1) += stm32/adc.c stm32/i2c.c
src-$(CONFIG_MACH_STM32F2) += ../lib/stm32f2/system_stm32f2xx.c
src-$(CONFIG_MACH_STM32F2) += stm32/stm32f4.c generic/armcm_timer.c
src-$(CONFIG_MACH_STM32F2) += stm32/adc.c stm32/i2c.c
src-$(CONFIG_MACH_STM32F4) += ../lib/stm32f4/system_stm32f4xx.c
src-$(CONFIG_MACH_STM32F4) += stm32/stm32f4.c generic/armcm_timer.c
src-$(CONFIG_MACH_STM32F4) += stm32/adc.c stm32/i2c.c
src-$(CONFIG_HAVE_GPIO_SPI) += stm32/spi.c
usb-src-$(CONFIG_HAVE_STM32_USBFS) := stm32/usbfs.c
usb-src-$(CONFIG_HAVE_STM32_USBOTG) := stm32/usbotg.c
src-$(CONFIG_USBSERIAL) += $(usb-src-y) stm32/chipid.c generic/usb_cdc.c
serial-src-y := stm32/serial.c
serial-src-$(CONFIG_MACH_STM32F0) := stm32/stm32f0_serial.c
src-$(CONFIG_SERIAL) += $(serial-src-y) generic/serial_irq.c
can-src-$(CONFIG_CANSERIAL) += stm32/can.c ../lib/fast-hash/fasthash.c
src-$(CONFIG_CANSERIAL) += $(can-src-y) generic/serial_irq.c

dirs-$(CONFIG_CANSERIAL) += lib/fast-hash

# Binary output file rules
target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating hex file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

# Flash rules
lib/hidflash/hid-flash:
	@echo "  Building hid-flash"
	$(Q)make -C lib/hidflash

flash: $(OUT)klipper.bin lib/hidflash/hid-flash
	@echo "  Flashing $< to $(FLASH_DEVICE)"
	$(Q)$(PYTHON) ./scripts/flash_usb.py -t $(CONFIG_MCU) -d "$(FLASH_DEVICE)" -s "$(CONFIG_FLASH_START)" $(if $(NOSUDO),--no-sudo) $(OUT)klipper.bin

serialflash: $(OUT)klipper.bin
	@echo "  Flashing $< to $(FLASH_DEVICE) via stm32flash"
	$(Q)stm32flash -w $< -v -g 0 $(FLASH_DEVICE)
