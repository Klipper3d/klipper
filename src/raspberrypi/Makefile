# Additional Raspberry Pi build rules

dirs-y += src/raspberrypi src/generic src/raspberrypi/ringbuf

src-y += raspberrypi/main.c raspberrypi/timer.c raspberrypi/console.c
src-y += raspberrypi/gpio.c generic/crc16_ccitt.c generic/alloc.c
src-y += raspberrypi/ringbuf/ringbuf.c

CFLAGS_klipper.elf += -lutil -lrt -lpthread

flash: $(OUT)klipper.elf
	@echo "  Flashing"
	$(Q)sudo ./scripts/flash-raspberrypi-linux.sh $(OUT)
