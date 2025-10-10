# hc32f460 build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

dirs-y += src/hc32f460 src/generic lib/hc32f460/driver/src lib/hc32f460/mcu/common

CFLAGS += -mthumb -mcpu=cortex-m4 -Isrc/hc32f460 -Ilib/hc32f460/driver/inc -Ilib/hc32f460/mcu/common -Ilib/cmsis-core -DHC32F460

CFLAGS_klipper.elf += -nostdlib -lgcc -lc_nano
CFLAGS_klipper.elf += -T $(OUT)src/generic/armcm_link.ld
$(OUT)klipper.elf: $(OUT)src/generic/armcm_link.ld

# Add source files
src-y += hc32f460/main.c
src-y += hc32f460/interrupts.c
src-y += hc32f460/gpio.c
src-y += ../lib/hc32f460/mcu/common/system_hc32f460.c
src-y += ../lib/hc32f460/driver/src/hc32f460_clk.c
src-y += ../lib/hc32f460/driver/src/hc32f460_efm.c
src-y += ../lib/hc32f460/driver/src/hc32f460_sram.c
src-y += ../lib/hc32f460/driver/src/hc32f460_utility.c
src-y += ../lib/hc32f460/driver/src/hc32f460_gpio.c
src-y += ../lib/hc32f460/driver/src/hc32f460_pwc.c
src-$(CONFIG_WANT_ADC) += hc32f460/adc.c ../lib/hc32f460/driver/src/hc32f460_adc.c
src-$(CONFIG_SERIAL) += hc32f460/serial.c generic/serial_irq.c ../lib/hc32f460/driver/src/hc32f460_usart.c
src-$(CONFIG_WANT_HARD_PWM) += hc32f460/hard_pwm.c ../lib/hc32f460/driver/src/hc32f460_timera.c
src-y += generic/armcm_boot.c generic/armcm_irq.c generic/armcm_timer.c
src-y += generic/armcm_reset.c generic/crc16_ccitt.c


# Build the additional bin output file
target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating bin file $@"
	$(Q)$(OBJCOPY) -O binary $< $@
