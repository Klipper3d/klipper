
OBJS_klipper.elf = $(OUT)autoconf.h
CMD_klipper.elf = IDF_TARGET=$(CONFIG_MCU) idf.py -B$(OUT)build -C src/$(CONFIG_BOARD_DIRECTORY) build

#$(OUT)klipper.bin: $(OUT)klipper.elf
#	cp -a out/build/klipper.bin out/klipper.bin

# @Todo flash rules
#flash: $(OUT)klipper.bin lib/bossac/bin/bossac
#	@echo "  Flashing $< to $(FLASH_DEVICE)"
#	$(Q)$(PYTHON) ./scripts/flash_usb.py -t $(CONFIG_MCU) -d "$(FLASH_DEVICE)" $(OUT)klipper.bin
