# at32f40x build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-


dirs-$(CONFIG_MACH_AT32F415) += src/at32 src/generic lib/at32f415
dirs-$(CONFIG_MACH_AT32F425) += src/at32 src/generic lib/at32f425
dirs-$(CONFIG_MACH_AT32F421) += src/at32 src/generic lib/at32f421
dirs-$(CONFIG_MACH_AT32F403A) += src/at32 src/generic lib/at32 lib/at32/include
dirs-$(CONFIG_MACH_AT32F407) += src/at32 src/generic lib/at32 lib/at32/include

MCU := $(shell echo $(CONFIG_MCU))
MCU_UPPER := $(shell echo $(CONFIG_MCU) | tr a-z A-Z | tr X x)

CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS-$(CONFIG_MACH_AT32F403A) += -DAT_START_F403A_V1
CFLAGS-$(CONFIG_MACH_AT32F407) += -DAT_START_F403A_V1
CFLAGS-$(CONFIG_MACH_AT32F415) += -DAT_START_F415_V1
CFLAGS-$(CONFIG_MACH_AT32F425) += -DAT_START_F425_V1
CFLAGS-$(CONFIG_MACH_AT32F421) += -DAT_START_F421_V1
CFLAGS-$(CONFIG_MACH_AT32F403A) += -DAT32F403AVGT7
CFLAGS-$(CONFIG_MACH_AT32F407) += -DAT32F403AVGT7
CFLAGS-$(CONFIG_MACH_AT32F415) += -DAT32F415CCU7
CFLAGS-$(CONFIG_MACH_AT32F425) += -DAT32F425F8P7
CFLAGS-$(CONFIG_MACH_AT32F421) += -DAT32F421F6P7
CFLAGS += -DMISC_MODULE_ENABLED
CFLAGS += -DCRM_MODULE_ENABLED
CFLAGS += -DGPIO_MODULE_ENABLED
CFLAGS += -DADC_MODULE_ENABLED
CFLAGS += -DTMR_MODULE_ENABLED
CFLAGS += -DUSART_MODULE_ENABLED
CFLAGS += -DWDT_MODULE_ENABLED
CFLAGS += -DFLASH_MODULE_ENABLED
#CFLAGS += -DI2C_MODULE_ENABLED
#CFLAGS += -DSPI_MODULE_ENABLED
#CFLAGS += -DUSB_MODULE_ENABLED

CFLAGS-$(CONFIG_MACH_AT32F403A) += -mfpu=vfpv4-d16
CFLAGS-$(CONFIG_MACH_AT32F407) += -mfpu=vfpv4-d16

CFLAGS-$(CONFIG_MACH_AT32F403A) += -Ilib/at32/include -Ilib/cmsis-core -Isrc/at32/include
CFLAGS-$(CONFIG_MACH_AT32F407) += -Ilib/at32/include -Ilib/cmsis-core -Isrc/at32/include
CFLAGS-$(CONFIG_MACH_AT32F415) += -Ilib/at32f415 -Ilib/cmsis-core -Isrc/at32/include
CFLAGS-$(CONFIG_MACH_AT32F425) += -Ilib/at32f425 -Ilib/cmsis-core -Isrc/at32/include
CFLAGS-$(CONFIG_MACH_AT32F421) += -Ilib/at32f421 -Ilib/cmsis-core
CFLAGS += $(CFLAGS-y) -D$(MCU_UPPER) -mthumb -mcpu=cortex-m4

#add debug info for debugging.
ifeq ($(DEBUG_EN),1)
CFLAGS += -gdwarf-4 
endif


CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs
CFLAGS_klipper.elf += -T $(OUT)src/generic/armcm_link.ld
$(OUT)klipper.elf: $(OUT)src/generic/armcm_link.ld

# Add source files
src-y += generic/armcm_boot.c generic/armcm_irq.c generic/armcm_timer.c
src-y += generic/armcm_reset.c generic/crc16_ccitt.c
src-$(CONFIG_MACH_AT32F415) += at32/at32f415.c at32/at32f415_clock.c
src-$(CONFIG_MACH_AT32F415) += ../lib/at32f415/system_at32f415.c
src-$(CONFIG_MACH_AT32F415) += ../lib/at32f415/at32f415_crm.c
src-$(CONFIG_MACH_AT32F415) += at32/gpio.c
pwm-src-$(CONFIG_MACH_AT32F415) := at32/hard_pwm.c
src-$(CONFIG_MACH_AT32F425) += at32/at32f425.c 
src-$(CONFIG_MACH_AT32F425) += ../lib/at32f425/system_at32f425.c
src-$(CONFIG_MACH_AT32F425) += ../lib/at32f425/at32f425_crm.c
src-$(CONFIG_MACH_AT32F425) += at32/at32f425_gpio.c
pwm-src-$(CONFIG_MACH_AT32F425) := at32/at32f425_pwm.c
src-$(CONFIG_MACH_AT32F421) += at32/at32f421.c 
src-$(CONFIG_MACH_AT32F421) += ../lib/at32f421/system_at32f421.c
src-$(CONFIG_MACH_AT32F421) += ../lib/at32f421/at32f421_crm.c
src-$(CONFIG_MACH_AT32F421) += at32/at32f421_gpio.c
pwm-src-$(CONFIG_MACH_AT32F421) := at32/at32f421_pwm.c
src-$(CONFIG_MACH_AT32F407) += ../lib/at32/system_at32f403a_407.c
src-$(CONFIG_MACH_AT32F407) += ../lib/at32/at32f403a_407_crm.c
src-$(CONFIG_MACH_AT32F407) += ../lib/at32/at32f403a_407_usart.c
src-$(CONFIG_MACH_AT32F407) += ../lib/at32/at32f403a_407_board.c
src-$(CONFIG_MACH_AT32F407) += ../lib/at32/at32f403a_407_misc.c
src-$(CONFIG_MACH_AT32F407) += ../lib/at32/at32f403a_407_gpio.c
src-$(CONFIG_MACH_AT32F407) += at32/gpio.c
pwm-src-$(CONFIG_MACH_AT32F407) := at32/hard_pwm.c
src-$(CONFIG_MACH_AT32F403A) += at32/at32f403a_407.c at32/at32f403a_407_clock.c
src-$(CONFIG_MACH_AT32F403A) += ../lib/at32/system_at32f403a_407.c
src-$(CONFIG_MACH_AT32F403A) += ../lib/at32/at32f403a_407_crm.c
src-$(CONFIG_MACH_AT32F403A) += ../lib/at32/at32f403a_407_usart.c
src-$(CONFIG_MACH_AT32F403A) += ../lib/at32/at32f403a_407_board.c
src-$(CONFIG_MACH_AT32F403A) += ../lib/at32/at32f403a_407_misc.c
src-$(CONFIG_MACH_AT32F403A) += ../lib/at32/at32f403a_407_gpio.c
src-$(CONFIG_MACH_AT32F403A) += at32/gpio.c
pwm-src-$(CONFIG_MACH_AT32F403A) := at32/hard_pwm.c

src-$(CONFIG_HAVE_GPIO_ADC) += at32/adc.c
src-$(CONFIG_HAVE_GPIO_I2C) += at32/i2c.c
src-$(CONFIG_HAVE_GPIO_SPI) += at32/spi.c
src-$(CONFIG_USBSERIAL) += at32/usbserial.c
src-$(CONFIG_USBSERIAL) += generic/usb_cdc.c
src-$(CONFIG_SERIAL) += at32/serial.c generic/serial_irq.c
src-$(CONFIG_HAVE_GPIO_HARD_PWM) += $(pwm-src-y)

# Build the additional bin output file
target-y += $(OUT)klipper.bin

#$(OUT)klipper.hex: $(OUT)klipper.bin
#	@echo " Creating hex file $@"
#	$(Q)$(OBJCOPY) -l binary -O ihex $< $@

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating bin file $@"
	$(Q)$(OBJCOPY) -O binary $< $@
	$(Q)$(OBJCOPY) $< -O ihex $(OUT)klipper.hex

# Flash rules
flash: $(OUT)klipper.bin
	@echo "  Flashing $< to $(FLASH_DEVICE)"
	$(Q)$(PYTHON) ./scripts/flash_usb.py -t $(CONFIG_MCU) -d "$(FLASH_DEVICE)" $(if $(NOSUDO),--no-sudo) $(OUT)klipper.bin
