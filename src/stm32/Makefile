# Additional STM32 build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/stm32 src/generic lib/fast-hash
dirs-$(CONFIG_MACH_STM32F0) += lib/stm32f0
dirs-$(CONFIG_MACH_STM32F1) += lib/stm32f1
dirs-$(CONFIG_MACH_N32G45x) += lib/n32g45x
dirs-$(CONFIG_MACH_STM32F2) += lib/stm32f2
dirs-$(CONFIG_MACH_STM32F4) += lib/stm32f4
dirs-$(CONFIG_MACH_STM32F7) += lib/stm32f7
dirs-$(CONFIG_MACH_STM32G0) += lib/stm32g0
dirs-$(CONFIG_MACH_STM32G4) += lib/stm32g4
dirs-$(CONFIG_MACH_STM32H7) += lib/stm32h7
dirs-$(CONFIG_MACH_STM32L4) += lib/stm32l4

MCU := $(shell echo $(CONFIG_MCU))
MCU_UPPER := $(shell echo $(CONFIG_MCU) | tr a-z A-Z | tr X x)

CFLAGS-$(CONFIG_MACH_STM32F0) += -mcpu=cortex-m0 -Ilib/stm32f0/include
CFLAGS-$(CONFIG_MACH_STM32F103) += -mcpu=cortex-m3
CFLAGS-$(CONFIG_MACH_N32G45x) += -mcpu=cortex-m4 -Ilib/n32g45x/include
CFLAGS-$(CONFIG_MACH_STM32F1) += -Ilib/stm32f1/include
CFLAGS-$(CONFIG_MACH_STM32F2) += -mcpu=cortex-m3 -Ilib/stm32f2/include
CFLAGS-$(CONFIG_MACH_STM32F4) += -mcpu=cortex-m4 -Ilib/stm32f4/include
CFLAGS-$(CONFIG_MACH_STM32F7) += -mcpu=cortex-m7 -Ilib/stm32f7/include
CFLAGS-$(CONFIG_MACH_STM32G0) += -mcpu=cortex-m0plus -Ilib/stm32g0/include
CFLAGS-$(CONFIG_MACH_STM32G4) += -mcpu=cortex-m4 -Ilib/stm32g4/include
CFLAGS-$(CONFIG_MACH_STM32H7) += -mcpu=cortex-m7 -Ilib/stm32h7/include
CFLAGS-$(CONFIG_MACH_STM32L4) += -mcpu=cortex-m4 -Ilib/stm32l4/include
CFLAGS += $(CFLAGS-y) -D$(MCU_UPPER) -mthumb -Ilib/cmsis-core -Ilib/fast-hash

CFLAGS_klipper.elf += -nostdlib -lgcc -lc_nano
CFLAGS_klipper.elf += -T $(OUT)src/generic/armcm_link.ld
$(OUT)klipper.elf: $(OUT)src/generic/armcm_link.ld

# Add source files
src-y += stm32/watchdog.c stm32/gpio.c stm32/clockline.c stm32/dfu_reboot.c
src-y += generic/crc16_ccitt.c
src-y += generic/armcm_boot.c generic/armcm_irq.c generic/armcm_reset.c
src-$(CONFIG_MACH_STM32F0) += ../lib/stm32f0/system_stm32f0xx.c
src-$(CONFIG_MACH_STM32F0) += generic/timer_irq.c stm32/stm32f0_timer.c
src-$(CONFIG_MACH_STM32F0) += stm32/stm32f0.c stm32/gpioperiph.c
src-$(CONFIG_MACH_STM32F0) += stm32/stm32f0_adc.c stm32/stm32f0_i2c.c
src-$(CONFIG_MACH_STM32F103) += ../lib/stm32f1/system_stm32f1xx.c
src-$(CONFIG_MACH_STM32F103) += stm32/adc.c
src-$(CONFIG_MACH_N32G45x) += ../lib/stm32f1/system_stm32f1xx.c
src-$(CONFIG_MACH_N32G45x) += ../lib/n32g45x/n32g45x_adc.c stm32/n32g45x_adc.c
src-$(CONFIG_MACH_STM32F1) += stm32/stm32f1.c generic/armcm_timer.c stm32/i2c.c
src-$(CONFIG_MACH_STM32F2) += ../lib/stm32f2/system_stm32f2xx.c
src-$(CONFIG_MACH_STM32F2) += stm32/stm32f4.c generic/armcm_timer.c
src-$(CONFIG_MACH_STM32F2) += stm32/gpioperiph.c stm32/adc.c stm32/i2c.c
src-$(CONFIG_MACH_STM32F4) += ../lib/stm32f4/system_stm32f4xx.c
src-$(CONFIG_MACH_STM32F4) += stm32/stm32f4.c generic/armcm_timer.c
src-$(CONFIG_MACH_STM32F4) += stm32/gpioperiph.c stm32/adc.c stm32/i2c.c
src-$(CONFIG_MACH_STM32F7) += ../lib/stm32f7/system_stm32f7xx.c
src-$(CONFIG_MACH_STM32F7) += stm32/stm32f7.c generic/armcm_timer.c
src-$(CONFIG_MACH_STM32F7) += stm32/gpioperiph.c stm32/adc.c stm32/stm32f0_i2c.c
src-$(CONFIG_MACH_STM32G0) += generic/timer_irq.c stm32/stm32f0_timer.c
src-$(CONFIG_MACH_STM32G0) += stm32/stm32g0.c stm32/gpioperiph.c
src-$(CONFIG_MACH_STM32G0) += stm32/stm32f0_adc.c stm32/stm32f0_i2c.c
src-$(CONFIG_MACH_STM32G4) += ../lib/stm32g4/system_stm32g4xx.c
src-$(CONFIG_MACH_STM32G4) += stm32/stm32g4.c generic/armcm_timer.c
src-$(CONFIG_MACH_STM32G4) += stm32/gpioperiph.c stm32/stm32h7_adc.c
src-$(CONFIG_MACH_STM32G4) += stm32/stm32f0_i2c.c
src-$(CONFIG_MACH_STM32H7) += ../lib/stm32h7/system_stm32h7xx.c
src-$(CONFIG_MACH_STM32H7) += stm32/stm32h7.c generic/armcm_timer.c
src-$(CONFIG_MACH_STM32H7) += stm32/gpioperiph.c stm32/stm32h7_adc.c
src-$(CONFIG_MACH_STM32H7) += stm32/stm32f0_i2c.c
src-$(CONFIG_MACH_STM32L4) += ../lib/stm32l4/system_stm32l4xx.c
src-$(CONFIG_MACH_STM32L4) += stm32/stm32l4.c generic/armcm_timer.c
src-$(CONFIG_MACH_STM32L4) += stm32/gpioperiph.c
src-$(CONFIG_MACH_STM32L4) += stm32/stm32h7_adc.c stm32/stm32f0_i2c.c
spi-src-y := stm32/spi.c
spi-src-$(CONFIG_MACH_STM32H7) := stm32/stm32h7_spi.c
src-$(CONFIG_HAVE_GPIO_SPI) += $(spi-src-y)
sdio-src-y := stm32/sdio.c
src-$(CONFIG_HAVE_GPIO_SDIO) += $(sdio-src-y)
usb-src-$(CONFIG_HAVE_STM32_USBFS) := stm32/usbfs.c
usb-src-$(CONFIG_HAVE_STM32_USBOTG) := stm32/usbotg.c
src-$(CONFIG_USBSERIAL) += $(usb-src-y) stm32/chipid.c generic/usb_cdc.c
serial-src-y := stm32/serial.c
serial-src-$(CONFIG_MACH_STM32F0) := stm32/stm32f0_serial.c
serial-src-$(CONFIG_MACH_STM32G0) := stm32/stm32f0_serial.c
serial-src-$(CONFIG_MACH_STM32G4) := stm32/stm32f0_serial.c
serial-src-$(CONFIG_MACH_STM32H7) := stm32/stm32f0_serial.c
src-$(CONFIG_SERIAL) += $(serial-src-y) generic/serial_irq.c
canbus-src-y := generic/canserial.c ../lib/fast-hash/fasthash.c
canbus-src-$(CONFIG_HAVE_STM32_CANBUS) += stm32/can.c
canbus-src-$(CONFIG_HAVE_STM32_FDCANBUS) += stm32/fdcan.c
src-$(CONFIG_CANSERIAL) += $(canbus-src-y) generic/canbus.c stm32/chipid.c
src-$(CONFIG_USBCANBUS) += $(usb-src-y) $(canbus-src-y)
src-$(CONFIG_USBCANBUS) += stm32/chipid.c generic/usb_canbus.c
src-$(CONFIG_HAVE_GPIO_HARD_PWM) += stm32/hard_pwm.c

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
	$(Q)$(PYTHON) ./scripts/flash_usb.py -t $(CONFIG_MCU) -d "$(FLASH_DEVICE)" -s "$(CONFIG_FLASH_APPLICATION_ADDRESS)" $(if $(NOSUDO),--no-sudo) $(OUT)klipper.bin

serialflash: $(OUT)klipper.bin
	@echo "  Flashing $< to $(FLASH_DEVICE) via stm32flash"
	$(Q)stm32flash -w $< -v -g 0 $(FLASH_DEVICE)
