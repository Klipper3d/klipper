

CROSS_PREFIX=or1k-linux-musl-
dirs-y += src/generic src/ar100

CFLAGS		+= -fno-builtin -fno-common -fno-pie -nostdlib
CFLAGS		+= -msfimm -mshftimm -msoft-div -msoft-mul
CFLAGS_klipper.elf := $(CFLAGS) -Wl,-r -Wl,-T,src/ar100/ar100.ld
CFLAGS_klipper.elf += -Wl,--no-undefined -Wl,-Map,out/klipper.map

# Add source files
src-y += ar100/main.c ar100/gpio.c generic/timer_irq.c
src-y += generic/crc16_ccitt.c #generic/alloc.c

# Build the AR100 binary
target-y += $(OUT)klipper.bin

$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Object copy $@"
	$(OBJCOPY) -O binary -S --reverse-bytes 4 $(OUT)klipper.elf $@
	truncate -s %8 $@

flash: $(OUT)ar100.bin
	@echo "  Flashing"
	$(Q)sudo ./scripts/flash-pru.sh
