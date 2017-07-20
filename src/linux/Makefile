# Additional linux build rules

dirs-y += src/linux src/generic

src-y += linux/main.c linux/timer.c linux/console.c linux/watchdog.c
src-y += generic/crc16_ccitt.c generic/alloc.c

CFLAGS_klipper.elf += -lutil
