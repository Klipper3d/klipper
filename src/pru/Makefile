# Additional PRU build rules

# Setup the toolchain
CROSS_PREFIX=pru-

dirs-y += src/pru src/generic
dirs-y += lib/pru_rpmsg

CFLAGS += -Os -minrt -mmcu=am335x.pru1
CFLAGS += -Ilib/pru_rpmsg/include -Ilib/pru_rpmsg/include/am335x

CFLAGS_klipper.elf := $(filter-out -mmcu=%, $(CFLAGS))
CFLAGS_klipper.elf += -Wl,-r -nostdlib -Wl,-T,src/pru/pru.lds
CFLAGS_pru0.elf := $(filter-out -mmcu=%, $(CFLAGS)) -mmcu=am335x.pru0
CFLAGS_pru1.elf := $(CFLAGS)

# Add source files
src-y += pru/main.c pru/gpio.c generic/timer_irq.c
src-$(CONFIG_HAVE_GPIO_ADC) += pru/adc.c

pru0-y := pru/pru0.c generic/crc16_ccitt.c command.c
pru0-y += ../lib/pru_rpmsg/pru_rpmsg.c ../lib/pru_rpmsg/pru_virtqueue.c

# Build the PRU binaries
target-y += $(OUT)pru0.elf $(OUT)pru1.elf

$(OUT)pru0.elf: $(patsubst %.c, $(OUT)src/%.o,$(pru0-y))
	@echo "  Linking $@"
	$(Q)$(CC) $(CFLAGS_klipper.elf) $^ -o $(OUT)pru0.o
	$(Q)$(CC) $(CFLAGS_pru0.elf) $(OUT)pru0.o -o $@

$(OUT)pru1.elf: $(OUT)klipper.elf
	@echo "  Linking $@"
	$(Q)$(CC) $(CFLAGS_pru1.elf) $< -o $@

flash: $(OUT)pru0.elf $(OUT)pru1.elf
	@echo "  Flashing"
	$(Q)sudo ./scripts/flash-pru.sh
