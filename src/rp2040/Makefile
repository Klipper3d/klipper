# Additional RP2040 build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/rp2040 src/generic lib/rp2040/elf2uf2 lib/fast-hash lib/can2040

CFLAGS += -mcpu=cortex-m0plus -mthumb -Ilib/cmsis-core
CFLAGS += -Ilib/rp2040 -Ilib/rp2040/cmsis_include -Ilib/fast-hash -Ilib/can2040

# Add source files
src-y += rp2040/main.c rp2040/watchdog.c rp2040/gpio.c
src-y += rp2040/adc.c rp2040/timer.c rp2040/bootrom.c
src-y += generic/armcm_boot.c generic/armcm_irq.c generic/armcm_reset.c
src-y += generic/timer_irq.c generic/crc16_ccitt.c
src-$(CONFIG_USBSERIAL) += rp2040/usbserial.c generic/usb_cdc.c
src-$(CONFIG_USBSERIAL) += rp2040/chipid.c
src-$(CONFIG_SERIAL) += rp2040/serial.c generic/serial_irq.c
src-$(CONFIG_CANSERIAL) += rp2040/can.c rp2040/chipid.c ../lib/can2040/can2040.c
src-$(CONFIG_CANSERIAL) += generic/canserial.c generic/canbus.c
src-$(CONFIG_CANSERIAL) += ../lib/fast-hash/fasthash.c
src-$(CONFIG_USBCANBUS) += rp2040/can.c rp2040/chipid.c ../lib/can2040/can2040.c
src-$(CONFIG_USBCANBUS) += generic/canserial.c generic/usb_canbus.c
src-$(CONFIG_USBCANBUS) += ../lib/fast-hash/fasthash.c rp2040/usbserial.c
src-$(CONFIG_HAVE_GPIO_HARD_PWM) += rp2040/hard_pwm.c
src-$(CONFIG_HAVE_GPIO_SPI) += rp2040/spi.c
src-$(CONFIG_HAVE_GPIO_I2C) += rp2040/i2c.c

# rp2040 stage2 building
STAGE2_FILE := $(shell echo $(CONFIG_RP2040_STAGE2_FILE))
$(OUT)stage2.o: lib/rp2040/boot_stage2/$(STAGE2_FILE) $(OUT)autoconf.h
	@echo "  Building rp2040 stage2 $@"
	$(Q)$(CC) $(CFLAGS) -Ilib/rp2040/boot_stage2 -Ilib/rp2040/boot_stage2/asminclude -DPICO_FLASH_SPI_CLKDIV=$(CONFIG_RP2040_STAGE2_CLKDIV) -c $< -o $(OUT)stage2raw1.o
	$(Q)$(LD) $(OUT)stage2raw1.o --script=lib/rp2040/boot_stage2/boot_stage2.ld -o $(OUT)stage2raw.o
	$(Q)$(OBJCOPY) -O binary $(OUT)stage2raw.o $(OUT)stage2raw.bin
	$(Q)lib/rp2040/boot_stage2/pad_checksum -s 0xffffffff $(OUT)stage2raw.bin $(OUT)stage2.S
	$(Q)$(CC) $(CFLAGS) -c $(OUT)stage2.S -o $(OUT)stage2.o

# Binary output file rules when using stage2
$(OUT)lib/rp2040/elf2uf2/elf2uf2: lib/rp2040/elf2uf2/main.cpp
	@echo "  Building $@"
	$(Q)g++ -g -O -Ilib/rp2040 $< -o $@

$(OUT)klipper.uf2: $(OUT)klipper.elf $(OUT)lib/rp2040/elf2uf2/elf2uf2
	@echo "  Creating uf2 file $@"
	$(Q)$(OUT)lib/rp2040/elf2uf2/elf2uf2 $< $@

rptarget-$(CONFIG_RP2040_HAVE_STAGE2) := $(OUT)klipper.uf2
rplink-$(CONFIG_RP2040_HAVE_STAGE2) := $(OUT)src/rp2040/rp2040_link.ld
stage2-$(CONFIG_RP2040_HAVE_STAGE2) := $(OUT)stage2.o

# rp2040 building when using a bootloader
$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating bin file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

rptarget-$(CONFIG_RP2040_HAVE_BOOTLOADER) := $(OUT)klipper.bin
rplink-$(CONFIG_RP2040_HAVE_BOOTLOADER) := $(OUT)src/generic/armcm_link.ld

# Set klipper.elf linker rules
target-y += $(rptarget-y)
CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs -T $(rplink-y)
OBJS_klipper.elf += $(stage2-y)
$(OUT)klipper.elf: $(stage2-y) $(rplink-y)

# Flash rules
lib/rp2040_flash/rp2040_flash:
	@echo "  Building rp2040_flash"
	$(Q)make -C lib/rp2040_flash rp2040_flash

flash: $(rptarget-y) lib/rp2040_flash/rp2040_flash
	@echo "  Flashing $< to $(FLASH_DEVICE)"
	$(Q)$(PYTHON) ./scripts/flash_usb.py -t $(CONFIG_MCU) -d "$(FLASH_DEVICE)" $(if $(NOSUDO),--no-sudo) $(rptarget-y)
