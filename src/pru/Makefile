# Additional PRU build rules

# Setup the toolchain
CROSS_PREFIX=pru-

dirs-y += src/pru src/generic
dirs-y += lib/pru_rpmsg

CFLAGS += -Os -mmcu=am335x.pru1
CFLAGS += -Ilib/pru_rpmsg/include -Ilib/pru_rpmsg/include/am335x

CFLAGS_klipper.elf := $(filter-out -mmcu=%, $(CFLAGS))
CFLAGS_klipper.elf += -Wl,-r -nostdlib -Wl,-T,src/pru/pru.lds
CFLAGS_pru0.elf := $(filter-out -mmcu=%, $(CFLAGS)) -minrt -mmcu=am335x.pru0
CFLAGS_pru1.elf := $(CFLAGS) -minrt

# Add source files
src-y := $(filter-out debugcmds.c, $(src-y))
src-y += pru/main.c pru/gpio.c
src-y += generic/crc16_ccitt.c generic/timer_irq.c

pru0-y := pru/pru0.c
pru0-y += ../lib/pru_rpmsg/pru_rpmsg.c ../lib/pru_rpmsg/pru_virtqueue.c

# Build the additional PRU0 binary
target-y += $(OUT)pru0.elf $(OUT)pru1.elf

$(OUT)pru0.elf: $(patsubst %.c, $(OUT)src/%.o,$(pru0-y))
	@echo "  Linking $@"
	$(Q)$(CC) $(CFLAGS_pru0.elf) $^ -o $@

$(OUT)pru1.elf: $(OUT)klipper.elf
	@echo "  Linking $@"
	$(Q)$(CC) $(CFLAGS_pru1.elf) $^ -o $@
