
CROSS_PREFIX=aarch64-linux-gnu-
OC = or1k-linux-musl-objcopy
dirs-y += src/generic src/ar100
OBJ=ar100.obj
#CFLAGS_klipper.elf := $(filter-out -mmcu=%, $(CFLAGS))
#CFLAGS_klipper.elf += -Wl,-r -nostdlib -Wl,-T,src/ar100/ar100.ld
CFLAGS_ar100.elf := $(filter-out -mmcu=%, $(CFLAGS)) -Wl,-r -nostdlib -Wl,-T,src/ar100/ar100.ld



# Add source files
src-y += ar100/main.c ar100/gpio.c generic/timer_irq.c
src-y += generic/crc16_ccitt.c #generic/alloc.c

ar100-y := ar100/ar100.c generic/crc16_ccitt.c command.c

# Build the AR100 binary
target-y += $(OUT)ar100.bin

$(OUT)ar100.bin: $(patsubst %.c, $(OUT)src/%.o,$(ar100-y))
	@echo "  Linking $@"
	$(Q)or1k-elf-gcc $(CFLAGS_ar100.elf) $^ -o $(OUT)ar100.o
	$(Q)or1k-elf-gcc $(CFLAGS_ar100.elf) $(OUT)ar100.o -o $(OBJ)
	$(OC) -O binary -S --reverse-bytes 4 $(OBJ) $@

flash: $(OUT)ar100.elf
	@echo "  Flashing"
	$(Q)sudo ./scripts/flash-pru.sh
