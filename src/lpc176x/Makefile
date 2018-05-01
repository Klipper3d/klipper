# Additional sam3x8e build rules

USBLIBDIR = lib/lpc176x/VCOM_lib/

# Setup the toolchain
CROSS_PREFIX=arm-none-eabi-

OPTIM_LEV := -O2
PROC_SPEC := -mcpu=cortex-m3 -mthumb

CFLAGS += $(OPTIM_LEV) $(PROC_SPEC)
CFLAGS += -ffunction-sections -fdata-sections  -fno-exceptions -fno-delete-null-pointer-checks
CFLAGS += -D__LPC176x__

# include dirs
CFLAGS += -Ilib/cmsis-lpc176x
CFLAGS += -Ilib/lpc176x/include
CFLAGS += -Ilib/lpc176x/libs
CFLAGS += -Ilib/lpc176x/startup
CFLAGS += -Ilib/lpc176x/VCOM_lib/inc/usbSerial

ASFLAGS += $(OPTIM_LEV) $(PROC_SPEC)

CFLAGS_klipper.elf += $(PROC_SPEC)
CFLAGS_klipper.elf += -specs=lib/lpc176x/startup/startfile.spec
CFLAGS_klipper.elf += -Wl,-Map=$(OUT)klipper.map,--cref,--gc-sections
CFLAGS_klipper.elf += -Tlib/lpc176x/startup/LPC1768.ld
CFLAGS_END_klipper.elf += --specs=nano.specs
CFLAGS_END_klipper.elf += -lm -lgcc -lc -lnosys

# Add driver sources
src-y += ../lib/lpc176x/driver/lpc17xx_clkpwr.c
src-y += ../lib/lpc176x/driver/lpc17xx_pinsel.c
src-y += ../lib/lpc176x/driver/lpc17xx_gpio.c
src-y += ../lib/lpc176x/driver/lpc17xx_spi.c
src-y += ../lib/lpc176x/driver/lpc17xx_adc.c
src-y += ../lib/lpc176x/driver/lpc17xx_wdt.c
src-y += ../lib/lpc176x/driver/lpc17xx_timer.c
src-y += ../lib/lpc176x/driver/lpc17xx_uart.c

src-y += ../lib/lpc176x/startup/cmsis_nvic.c
src-y += ../lib/lpc176x/startup/system_LPC17xx.c
src-y += ../lib/lpc176x/startup/startup_LPC17xx.s

src-y += ../lib/lpc176x/libs/delay.c

# usb serial:
src-$(CONFIG_SERIAL) += ../lib/lpc176x/VCOM_lib/src/usbSerial/usbcontrol.c
src-$(CONFIG_SERIAL) += ../lib/lpc176x/VCOM_lib/src/usbSerial/usbhw_lpc.c
src-$(CONFIG_SERIAL) += ../lib/lpc176x/VCOM_lib/src/usbSerial/usbinit.c
src-$(CONFIG_SERIAL) += ../lib/lpc176x/VCOM_lib/src/usbSerial/usbstdreq.c

# Add source files
src-y += lpc176x/main.c lpc176x/custom_start.c
src-y += lpc176x/mempool.c lpc176x/watchdog.c
src-y += lpc176x/timer.c lpc176x/gpio.c lpc176x/spi.c lpc176x/adc.c
src-$(CONFIG_SERIAL) += lpc176x/serial_usb.c lpc176x/serial_uart.c
src-y += generic/crc16_ccitt.c generic/alloc.c generic/timer_irq.c generic/armcm_irq.c

# Build the additional hex output file
target-y += $(OUT)klipper.bin
target-y += $(OUT)klipper.disasm
target-y += $(OUT)klipper.hex

dirs-y += $(dir $(src-y))


$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Creating bin file $@"
	$(Q)$(OBJCOPY) -O binary $< $@

$(OUT)klipper.hex: $(OUT)klipper.elf
	@echo "  Creating hex file $@"
	$(Q)$(OBJCOPY) -R .stack -O ihex $< $@

$(OUT)klipper.disasm: $(OUT)klipper.elf
	@echo "  Creating disasm file $@"
	$(Q)$(OBJDUMP)  -d -f -M reg-names-std --demangle $< > $@

flash: $(OUT)klipper.hex
	@echo "  Flashing $^ to $(FLASH_DEVICE) via lpc21isp"
	$(Q)if [ -z $(FLASH_DEVICE) ]; then echo "Please specify FLASH_DEVICE"; exit 1; fi
	$(Q)lpc21isp $(OUT)klipper.hex $(FLASH_DEVICE) 57600 12000
