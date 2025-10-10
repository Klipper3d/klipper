# Additional RPxxxx Raspberry Pi MCU build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/rp2040 src/generic lib/elf2uf2 lib/fast-hash lib/can2040

MCU := $(shell echo $(CONFIG_MCU))
MCU_UPPER := $(shell echo $(CONFIG_MCU) | tr a-z A-Z | tr X x)

CFLAGS-$(CONFIG_MACH_RP2040) += -mcpu=cortex-m0plus
CFLAGS-$(CONFIG_MACH_RP2350) += -mcpu=cortex-m33
CFLAGS += $(CFLAGS-y) -DPICO_$(MCU_UPPER) -mthumb -Ilib/cmsis-core
CFLAGS += -Ilib/pico-sdk/$(MCU) -Ilib/pico-sdk
CFLAGS += -Ilib/pico-sdk/$(MCU)/cmsis_include -Ilib/fast-hash -Ilib/can2040

# Add source files
src-y += rp2040/main.c rp2040/watchdog.c rp2040/gpio.c rp2040/adc.c
src-y += generic/armcm_boot.c generic/armcm_irq.c
src-y += generic/armcm_reset.c generic/crc16_ccitt.c
src-$(CONFIG_MACH_RP2040) += rp2040/timer.c generic/timer_irq.c rp2040/bootrom.c
src-$(CONFIG_MACH_RP2350) += generic/armcm_timer.c rp2040/rp2350_bootrom.c
src-$(CONFIG_USBSERIAL) += rp2040/usbserial.c generic/usb_cdc.c rp2040/chipid.c
src-$(CONFIG_SERIAL) += rp2040/serial.c generic/serial_irq.c
src-$(CONFIG_CANSERIAL) += rp2040/can.c rp2040/chipid.c ../lib/can2040/can2040.c
src-$(CONFIG_CANSERIAL) += generic/canserial.c generic/canbus.c
src-$(CONFIG_CANSERIAL) += ../lib/fast-hash/fasthash.c
src-$(CONFIG_USBCANBUS) += rp2040/can.c rp2040/chipid.c ../lib/can2040/can2040.c
src-$(CONFIG_USBCANBUS) += generic/canserial.c generic/usb_canbus.c
src-$(CONFIG_USBCANBUS) += ../lib/fast-hash/fasthash.c rp2040/usbserial.c
src-$(CONFIG_WANT_HARD_PWM) += rp2040/hard_pwm.c
src-$(CONFIG_WANT_SPI) += rp2040/spi.c
src-$(CONFIG_WANT_I2C) += rp2040/i2c.c

# rp2040 stage2 building
STAGE2_FILE := $(shell echo $(CONFIG_RP2040_STAGE2_FILE))
$(OUT)stage2.o: lib/pico-sdk/$(MCU)/boot_stage2/$(STAGE2_FILE) $(OUT)autoconf.h
	@echo "  Building rp2040 stage2 $@"
	$(Q)$(CC) $(CFLAGS) -Ilib/pico-sdk/$(MCU)/boot_stage2 -Ilib/pico-sdk/$(MCU)/boot_stage2/asminclude -DPICO_FLASH_SPI_CLKDIV=$(CONFIG_RP2040_STAGE2_CLKDIV) -c $< -o $(OUT)stage2raw1.o
	$(Q)$(LD) $(OUT)stage2raw1.o --script=lib/pico-sdk/$(MCU)/boot_stage2/boot_stage2.ld -o $(OUT)stage2raw.o
	$(Q)$(OBJCOPY) -O binary $(OUT)stage2raw.o $(OUT)stage2raw.bin
	$(Q)lib/pico-sdk/$(MCU)/boot_stage2/pad_checksum -s 0xffffffff $(OUT)stage2raw.bin $(OUT)stage2.S
	$(Q)$(CC) $(CFLAGS) -c $(OUT)stage2.S -o $(OUT)stage2.o

# Binary output file rules when using stage2
$(OUT)lib/elf2uf2/elf2uf2: lib/elf2uf2/main.cpp
	@echo "  Building $@"
	$(Q)g++ -g -O -Ilib/pico-sdk $< -o $@

$(OUT)klipper.uf2: $(OUT)klipper.elf $(OUT)lib/elf2uf2/elf2uf2
	@echo "  Creating uf2 file $@"
	$(Q)$(OUT)lib/elf2uf2/elf2uf2 $< $@

rptarget-y := $(OUT)klipper.uf2
stage2-$(CONFIG_RP2040_HAVE_STAGE2) := $(OUT)stage2.o

# rp2040 building when using a bootloader
$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating bin file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

rptarget-$(CONFIG_RPXXXX_HAVE_BOOTLOADER) := $(OUT)klipper.bin

# Set klipper.elf linker rules
target-y += $(rptarget-y)
CFLAGS_klipper.elf += -nostdlib -lgcc -lc_nano
CFLAGS_klipper.elf += -T $(OUT)src/rp2040/rpxxxx_link.ld
OBJS_klipper.elf += $(stage2-y)
$(OUT)klipper.elf: $(stage2-y) $(OUT)src/rp2040/rpxxxx_link.ld

# Flash rules
lib/rp2040_flash/rp2040_flash:
	@echo "  Building rp2040_flash"
	$(Q)make -C lib/rp2040_flash rp2040_flash

flash: $(rptarget-y) lib/rp2040_flash/rp2040_flash
	@echo "  Flashing $< to $(FLASH_DEVICE)"
	$(Q)$(PYTHON) ./scripts/flash_usb.py -t $(CONFIG_MCU) -d "$(FLASH_DEVICE)" $(if $(NOSUDO),--no-sudo) $(rptarget-y)
