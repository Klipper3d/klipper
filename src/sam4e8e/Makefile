# Additional sam4e8e build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-
dirs-y += src/sam4e8e src/generic

dirs-y += lib/asf/sam/drivers/pmc \
		  lib/asf/sam/drivers/pio \
		  lib/asf/sam/drivers/tc \
		  lib/asf/sam/drivers/wdt \
		  lib/asf/sam/utils/cmsis/sam4e/source/interrupt \
		  lib/asf/common/services/sleepmgr/sam \
		  lib/asf/common/services/clock/sam4e \
		  lib/asf/sam/drivers/pio \

dirs-$(CONFIG_HAVE_GPIO_I2C) += lib/asf/sam/drivers/twi \


dirs-$(CONFIG_SERIAL) += lib/asf/sam/drivers/udp \
		  	 lib/asf/sam/drivers/twi \
		  	 lib/asf/common/services/usb/class/cdc/device \
		  	 lib/asf/common/services/usb/udc \

dirs-$(CONFIG_HAVE_GPIO_SPI) += lib/asf/sam/drivers/spi \
		  		lib/asf/sam/drivers/usart

dirs-$(CONFIG_HAVE_GPIO_ADC) += lib/asf/sam/drivers/afec \

CFLAGS += -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard

CFLAGS += -Isrc/sam4e8e/conf \
		  -Ilib/asf/sam/ \
		  -Ilib/asf/sam/utils \
		  -Ilib/asf/sam/utils/fpu \
		  -Ilib/asf/sam/utils/preprocessor \
		  -Ilib/asf/sam/utils/header_files \
		  -Ilib/asf/sam/utils/cmsis/sam4e/source \
		  -Ilib/asf/sam/utils/cmsis/sam4e/include \
		  -Ilib/asf/sam/utils/cmsis/sam4e/source/templates \
		  -Ilib/asf/sam/utils/cmsis/sam4e/source/interrupt \
		  -Ilib/asf/thirdparty/CMSIS/Include \
		  -Ilib/asf/common/utils \
		  -Ilib/asf/common/services/usb \
		  -Ilib/asf/common/services/ioport \
		  -Ilib/asf/common/services/usb/udc \
		  -Ilib/asf/common/services/usb/class/cdc \
		  -Ilib/asf/common/services/usb/class/cdc/device \


CFLAGS += -Ilib/asf/sam/drivers \
		  -Ilib/asf/sam/drivers/pio \
		  -Ilib/asf/sam/drivers/pmc \
		  -Ilib/asf/sam/drivers/twi \
		  -Ilib/asf/common/services/sleepmgr \
		  -Ilib/asf/common/services/clock \
		  -Ilib/asf/common/services/clock/sam4e \

CFLAGS += -D__SAM4E8E__ -D__USE_ASF__

CFLAGS_klipper.elf += -Llib/asf/sam/utils/linker_scripts/sam4e/sam4e8/gcc
CFLAGS_klipper.elf += -T lib/asf/sam/utils/linker_scripts/sam4e/sam4e8/gcc/flash.ld
CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs

# Add source files
src-y += ../lib/asf/sam/drivers/pmc/pmc.c \
		 ../lib/asf/sam/drivers/pmc/sleep.c \
		 ../lib/asf/sam/drivers/tc/tc.c \
		 ../lib/asf/sam/drivers/wdt/wdt.c \
		 ../lib/asf/sam/drivers/pio/pio.c \
		 ../lib/asf/common/services/sleepmgr/sam/sleepmgr.c \
		 ../lib/asf/common/services/clock/sam4e/sysclk.c \
		 ../lib/asf/sam/utils/cmsis/sam4e/source/interrupt/interrupt_sam_nvic.c \

src-$(CONFIG_HAVE_GPIO_ADC) += ../lib/asf/sam/drivers/afec/afec.c \

src-$(CONFIG_SERIAL) += ../lib/asf/common/services/usb/class/cdc/device/udi_cdc.c \
		 				../lib/asf/common/services/usb/udc/udc.c \
		 				../lib/asf/common/services/usb/class/cdc/device/udi_cdc_desc.c \
		 				../lib/asf/sam/drivers/udp/udp_device.c \

src-$(CONFIG_HAVE_GPIO_I2C) += ../lib/asf/sam/drivers/twi/twi.c \
							   sam4e8e/i2c.c

src-$(CONFIG_HAVE_GPIO_SPI) += ../lib/asf/sam/drivers/spi/spi.c \
							   ../lib/asf/sam/drivers/usart/usart.c \
							   sam4e8e/spi.c

src-$(CONFIG_SERIAL) += sam4e8e/usb_serial.c
src-$(CONFIG_HAVE_GPIO) += sam4e8e/gpio.c
src-y += generic/crc16_ccitt.c generic/alloc.c
src-y += generic/armcm_irq.c generic/timer_irq.c
src-y += sam4e8e/system_sam4e.c sam4e8e/exceptions.c
src-y += sam4e8e/startup_sam4e.c sam4e8e/main.c sam4e8e/timer.c





# Build the additional hex output file
target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating bin file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

flash: $(OUT)klipper.bin
	@echo "  Flashing $^ to $(FLASH_DEVICE) via bossac"
	$(Q)tools/bossa/bin/bossac --port="$(FLASH_DEVICE)" -b -U -e -w -v $(OUT)klipper.bin -R
