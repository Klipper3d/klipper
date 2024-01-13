# at32f40x build rules

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-


dirs-$(CONFIG_MACH_GD32F30X) += src/gd32 src/generic lib/gd32f30x
dirs-$(CONFIG_MACH_GD32E23X) += src/gd32 src/generic lib/gd32e23x

MCU := $(shell echo $(CONFIG_MCU))
MCU_UPPER := $(shell echo $(CONFIG_MCU) | tr a-z A-Z | tr X x)

CFLAGS-$(CONFIG_MACH_GD32F30X) += -DUSE_STDPERIPH_DRIVER
CFLAGS-$(CONFIG_MACH_GD32F303XB) += -DGD32F30X_HD
CFLAGS-$(CONFIG_MACH_GD32F303XC) += -DGD32F30X_HD
CFLAGS-$(CONFIG_MACH_GD32F303XE) += -DGD32F30X_HD
CFLAGS-$(CONFIG_MACH_GD32F303XG) += -DGD32F30X_XD
CFLAGS-$(CONFIG_MACH_GD32F303XI) += -DGD32F30X_XD
CFLAGS-$(CONFIG_MACH_GD32F303XK) += -DGD32F30X_XD
CFLAGS-$(CONFIG_MACH_GD32F305XX) += -DGD32F30X_CL
CFLAGS-$(CONFIG_MACH_GD32F307XX) += -DGD32F30X_CL
CFLAGS-$(CONFIG_MACH_GD32E23X) += -DGD32E230

#CFLAGS += -DI2C_MODULE_ENABLED
#CFLAGS += -DSPI_MODULE_ENABLED
#CFLAGS += -DUSB_MODULE_ENABLED

CFLAGS-$(CONFIG_MACH_GD32E23X) += -mcpu=cortex-m23 -Ilib/gd32e23x
CFLAGS-$(CONFIG_MACH_GD32F30X) += -mcpu=cortex-m4 -Ilib/gd32f30x
CFLAGS += $(CFLAGS-y) -D$(MCU_UPPER) -mthumb -Ilib/cmsis-core

# add debug info for debugging.
# ifeq ($(DEBUG_EN),1)
# CFLAGS += -gdwarf-4 
# endif
#CFLAGS += -Wl,-Map,out/target.map 
#LDFLAGS += -Wl,-Map=out/target.map 


CFLAGS_klipper.elf += --specs=nano.specs --specs=nosys.specs
CFLAGS_klipper.elf += -T $(OUT)src/generic/armcm_link.ld
$(OUT)klipper.elf: $(OUT)src/generic/armcm_link.ld

# Add source files
src-y += generic/armcm_boot.c generic/armcm_irq.c 
src-y += generic/armcm_reset.c generic/crc16_ccitt.c

src-$(CONFIG_MACH_GD32F30X) += gd32/gd32f30x.c gd32/gd32f30x_gpio.c
src-$(CONFIG_MACH_GD32F30X) += generic/armcm_timer.c
# pwm-src-$(CONFIG_MACH_GD32F30X) := gd32/hard_pwm.c

src-$(CONFIG_MACH_GD32E23X) += gd32/gd32e23x.c gd32/gd32e23x_gpio.c  
src-$(CONFIG_MACH_GD32E23X) += gd32/gd32e23x_timer.c generic/timer_irq.c

src-$(CONFIG_HAVE_GPIO_ADC) += gd32/adc.c
src-$(CONFIG_HAVE_GPIO_I2C) += gd32/i2c.c
src-$(CONFIG_HAVE_GPIO_SPI) += gd32/spi.c
src-$(CONFIG_USBSERIAL) += gd32/usbserial.c
src-$(CONFIG_USBSERIAL) += generic/usb_cdc.c
src-$(CONFIG_SERIAL) += gd32/serial.c generic/serial_irq.c
src-$(CONFIG_HAVE_GPIO_HARD_PWM) += $(pwm-src-y)

# Rename binary output file rules
ifeq ($(CONFIG_BOARD_INFO_CONFIGURE),y)

ifneq ($(board_hw_version)$(board_fw_version),)
rename_bin := $(board_hw_version)-$(board_fw_version).bin

target-y += $(OUT)$(rename_bin)

$(OUT)$(rename_bin):$(OUT)klipper.bin
	@echo "  Rename hex file"
	$(Q)cp -v $< $@
endif

endif

# Build the additional bin output file
target-y += $(OUT)klipper.bin

#$(OUT)klipper.hex: $(OUT)klipper.bin
#	@echo " Creating hex file $@"
#	$(Q)$(OBJCOPY) -l binary -O ihex $< $@

ifeq ($(CONFIG_MACH_GD32F30X),y)
$(OUT)src/prtouch_v2.o:
	$(Q)cp src/prtouch_v2_cm4.o $@
endif

ifeq ($(CONFIG_MACH_GD32E23X),y)
$(OUT)src/prtouch_v2.o: 
	$(Q)cp src/prtouch_v2_cm23.o $@
endif


$(OUT)klipper.bin: $(OUT)klipper.elf $(OUT)hostCrc16.elf
	@echo "  Creating bin file $@"
	$(Q)$(OBJCOPY) -O binary $< $@
	$(Q)$(OUT)hostCrc16.elf $@
	$(Q)srec_cat $@ -binary -offset $(CONFIG_FLASH_APPLICATION_ADDRESS) -o $(OUT)klipper.hex -intel 
#	$(Q)$(OBJCOPY) $< -O ihex $(OUT)klipper.hex

# Flash rules
flash: $(OUT)klipper.bin
	@echo "  Flashing $< to $(FLASH_DEVICE)"
	$(Q)$(PYTHON) ./scripts/flash_usb.py -t $(CONFIG_MCU) -d "$(FLASH_DEVICE)" $(if $(NOSUDO),--no-sudo) $(OUT)klipper.bin
