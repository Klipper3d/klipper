CROSS_PREFIX=or1k-linux-musl-
dirs-y += src/generic src/ar100

CFLAGS		+= -fno-builtin
CFLAGS		+= -fno-pie
CFLAGS		+= -ggdb
CFLAGS		+= -ffreestanding #
CFLAGS		+= -msfimm -mshftimm -msoft-div -msoft-mul
CFLAGS_klipper.elf := $(CFLAGS) -Wl,-T,src/ar100/ar100.ld -Wl,-Map,out/klipper.map
CFLAGS_klipper.elf += -Wl,--no-undefined  -Wl,--gc-sections -static
CFLAGS_klipper.elf += -Wl,--no-dynamic-linker

# Add source files
src-y += ar100/main.c ar100/gpio.c ar100/serial.c ar100/prcm.c generic/timer_irq.c
src-y += generic/crc16_ccitt.c generic/alloc.c

OBJS_klipper.elf += $(OUT)src/ar100/start.o
OBJS_klipper.elf += $(OUT)src/ar100/runtime.o

# Build the AR100 binary
target-y += $(OUT)klipper.bin

SFLAGS = -nostdinc -Wa,--fatal-warnings -MMD -Wmissing-prototypes -Wall -Wextra

$(OUT)src/ar100/start.o:
	@echo "  Compiling $@"
	$(Q)$(CC) $(SFLAGS) -c $(PWD)/src/ar100/start.S -o $@

$(OUT)src/ar100/runtime.o:
	@echo "  Compiling $@"
	$(Q)$(CC) $(SFLAGS) -c $(PWD)/src/ar100/runtime.S -o $@


$(OUT)klipper.bin: $(OUT)klipper.elf
	@echo "  Object copy $@"
	$(OBJCOPY) -O binary -S --reverse-bytes 4 $(OUT)klipper.elf $@
	truncate -s %8 $@

flash: $(OUT)ar100.bin
	@echo "  Flashing"
	$(Q)sudo ./scripts/load out/klipper.bin
