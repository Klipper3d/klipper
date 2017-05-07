# Additional PRU build rules

# Setup the toolchain
CROSS_PREFIX=pru-

dirs-y += src/pru src/generic
dirs-y += lib/pru_rpmsg

CFLAGS += -Os -mmcu=am335x.pru0
CFLAGS += -Ilib/pru_rpmsg/include -Ilib/pru_rpmsg/include/am335x

CFLAGS_klipper.o := $(filter-out -mmcu=%, $(CFLAGS_klipper.o))
CFLAGS_klipper.elf := $(CFLAGS) -minrt -T src/pru/pru.lds
CFLAGS_pru1.elf := $(filter-out -mmcu=%, $(CFLAGS)) -mmcu=am335x.pru1

# Add source files
src-y := $(filter-out debugcmds.c, $(src-y))
src-y += pru/main.c pru/console.c pru/gpio.c
src-y += generic/crc16_ccitt.c generic/timer_irq.c
src-y += ../lib/pru_rpmsg/pru_rpmsg.c ../lib/pru_rpmsg/pru_virtqueue.c

# Build the additional PRU1 binary
target-y += $(OUT)pru1.elf

$(OUT)pru1.elf: $(OUT)src/pru/pru1.o
	@echo "  Linking $@"
	$(Q)$(CC) $(CFLAGS_pru1.elf) $^ -o $@
