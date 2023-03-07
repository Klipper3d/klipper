CROSS_PREFIX=or1k-linux-musl-
dirs-y += src/generic src/ar100 lib/ar100

CFLAGS		+= -O3
CFLAGS		+= -fno-builtin
CFLAGS		+= -fno-pie
CFLAGS		+= -ffreestanding
CFLAGS		+= -msfimm -mshftimm -msoft-div -msoft-mul
CFLAGS		+= -Ilib/ar100
CFLAGS_klipper.elf := $(CFLAGS) -T src/ar100/ar100.ld
CFLAGS_klipper.elf += -Wl,--gc-sections -static
CFLAGS_klipper.elf += -Wl,--no-dynamic-linker

SFLAGS		 = -nostdinc -MMD
SFLAGS		+= -Ilib/ar100

# Add source files
src-y += ar100/main.c ar100/gpio.c ar100/serial.c
src-y += ar100/util.c ar100/timer.c
src-y += generic/crc16_ccitt.c generic/timer_irq.c

# Remove files that are not needed to save space
src-y := $(filter-out lcd_hd44780.c,$(src-y))
src-y := $(filter-out lcd_st7920.c,$(src-y))
src-y := $(filter-out sensor_angle.c,$(src-y))
src-y := $(filter-out thermocouple.c,$(src-y))

OBJS_klipper.elf += $(OUT)lib/ar100/start.o
OBJS_klipper.elf += $(OUT)lib/ar100/runtime.o

# Build the AR100 binary
target-y += $(OUT)ar100.bin

$(OUT)lib/ar100/start.o:
	@echo "  Compiling $@"
	$(Q)$(CC) $(SFLAGS) -c $(PWD)/lib/ar100/start.S -o $@

$(OUT)lib/ar100/runtime.o:
	@echo "  Compiling $@"
	$(Q)$(CC) $(SFLAGS) -c $(PWD)/lib/ar100/runtime.S -o $@

$(OUT)ar100.bin: $(OUT)klipper.elf
	@echo "  Object copy $@"
	$(OBJCOPY) -O binary -S --reverse-bytes 4 $(OUT)klipper.elf $@
	truncate -s %8 $@
