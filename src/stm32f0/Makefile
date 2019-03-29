# Additional STM32F0 build rules

STLINK=st-flash

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/stm32f0 src/generic
dirs-y += lib/stm32f0 lib/stm32f0/gcc lib/stm32f0/hal/source

CFLAGS += -mthumb -mcpu=cortex-m0
CFLAGS += -Ilib/cmsis-core -Iout/board
CFLAGS += -Ilib/stm32f0/include -Ilib/stm32f0/hal/include
CFLAGS += -DSTM32F042x6 -DUSE_HAL_DRIVER

# Add source files
src-y += stm32f0/main.c stm32f0/timer.c stm32f0/gpio.c
src-$(CONFIG_CANSERIAL) += stm32f0/can.c
src-$(CONFIG_DEBUG_OUT) += stm32f0/log.c
src-$(CONFIG_SERIAL) += stm32f0/serial.c
src-y += generic/serial_irq.c
src-y += generic/timer_irq.c
src-$(CONFIG_HAVE_GPIO_ADC) += stm32f0/adc.c
src-$(CONFIG_HAVE_GPIO_I2C) += stm32f0/i2c.c
src-$(CONFIG_HAVE_GPIO_SPI) += stm32f0/spi.c
src-y += $(addprefix ../, $(wildcard lib/stm32f0/hal/source/stm32f0xx_*.c))
src-y += generic/crc16_ccitt.c generic/armcm_irq.c
src-y += ../lib/stm32f0/system_stm32f0xx.c

# Add assembler build rules
$(OUT)%.o: %.s $(OUT)autoconf.h $(OUT)board-link
	@echo "  Assembling $@"
	$(Q)$(AS) $< -o $@

asmsrc-y = ../lib/stm32f0/gcc/startup_stm32f042x6.s
OBJS_klipper.elf += $(patsubst %.s, $(OUT)src/%.o,$(asmsrc-y))

# Build the linker script
$(OUT)stm32f0.ld: src/stm32f0/stm32f0.ld $(OUT)board-link
	@echo "  Preprocessing $@"
	$(Q)$(CPP) -P -MD -MT $@ -DFLASH_START=$(CONFIG_FLASH_START) $< -o $@

CFLAGS_klipper.elf += -T $(OUT)stm32f0.ld
CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs
$(OUT)klipper.elf : $(OUT)stm32f0.ld

# Binary output file rules
target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating binary file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

$(OUT)klipper.hex: $(OUT)klipper.elf
	@echo "  Creating hex file $@"
	$(Q)$(OBJCOPY) -O ihex $< $@

flash: $(OUT)klipper.hex
	@echo "  Flashing hex file $<"
	$(Q)$(STLINK) --format ihex --reset write $<

debug: $(OUT)klipper.hex
	@echo "  Remote debug:"
	openocd -f interface/stlink.cfg -f target/stm32f0x.cfg
