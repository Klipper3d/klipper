# XXX build system
#
# Copyright (C) 2014  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU LGPLv3 license.

# Output directory
OUT=out/

# Kconfig includes
export HOSTCC             := $(CC)
export CONFIG_SHELL       := sh
export KCONFIG_AUTOHEADER := autoconf.h
export KCONFIG_CONFIG     := $(CURDIR)/.config
-include $(KCONFIG_CONFIG)

# Common command definitions
ifeq ($(CONFIG_MACH_AVR),y)
CROSS_PREFIX=avr-
endif
CC=$(CROSS_PREFIX)gcc
AS=$(CROSS_PREFIX)as
LD=$(CROSS_PREFIX)ld
OBJCOPY=$(CROSS_PREFIX)objcopy
OBJDUMP=$(CROSS_PREFIX)objdump
STRIP=$(CROSS_PREFIX)strip
CPP=cpp
PYTHON=python

# Source files
src-y=sched.c command.c
src-$(CONFIG_MACH_AVR) += avr/main.c avr/timer.c
src-$(CONFIG_MACH_SIMU) += simulator/main.c
src-$(CONFIG_AVR_WATCHDOG) += avr/watchdog.c
src-$(CONFIG_AVR_SERIAL) += avr/serial.c
DIRS=src src/avr src/simulator

# Default compiler flags
cc-option=$(shell if test -z "`$(1) $(2) -S -o /dev/null -xc /dev/null 2>&1`" \
    ; then echo "$(2)"; else echo "$(3)"; fi ;)

CFLAGS-y := -I$(OUT) -Isrc -Os -MD -g \
    -Wall -Wold-style-definition $(call cc-option,$(CC),-Wtype-limits,) \
    -ffunction-sections -fdata-sections
CFLAGS-y += -flto -fwhole-program
CFLAGS-$(CONFIG_MACH_AVR) += -mmcu=$(CONFIG_AVR_MCU) -DF_CPU=$(CONFIG_AVR_FREQ)
CFLAGS := $(CFLAGS-y)

LDFLAGS-$(CONFIG_MACH_AVR) := -Wl,--gc-sections -Wl,--relax
LDFLAGS-$(CONFIG_MACH_AVR) += -Wl,-u,vfprintf -lprintf_min -lm
LDFLAGS := $(LDFLAGS-y)

CPPFLAGS = -P -MD -MT $@

# Run with "make V=1" to see the actual compile commands
ifdef V
Q=
else
Q=@
MAKEFLAGS += --no-print-directory
endif

# Default targets
target-y := $(OUT)klipper.elf

all: $(target-y)

################ Common build rules

$(OUT)%.o: %.c $(OUT)autoconf.h $(OUT)board
	@echo "  Compiling $@"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

################ Main build rules

$(OUT)board: $(KCONFIG_CONFIG)
	@echo "  Creating symbolic link $@"
	$(Q)rm -f $@
	$(Q)ln -sf $(PWD)/src/$(CONFIG_BOARD_DIRECTORY) $@

$(OUT)declfunc.lds: src/declfunc.lds.S
	@echo "  Precompiling $@"
	$(Q)$(CPP) $(CPPFLAGS) -D__ASSEMBLY__ $< -o $@

$(OUT)klipper.o: $(patsubst %.c, $(OUT)src/%.o,$(src-y)) $(OUT)declfunc.lds
	@echo "  Linking $@"
	$(Q)$(CC) $(CFLAGS) -Wl,-r -Wl,-T,$(OUT)declfunc.lds -nostdlib $(patsubst %.c, $(OUT)src/%.o,$(src-y)) -o $@

$(OUT)klipper.elf: $(OUT)klipper.o
	@echo "  Linking $@"
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

################ Kconfig rules

define do-kconfig
$(Q)mkdir -p $(OUT)/scripts/kconfig/lxdialog
$(Q)mkdir -p $(OUT)/include/config
$(Q)mkdir -p $(addprefix $(OUT), $(DIRS))
$(Q)$(MAKE) -C $(OUT) -f $(CURDIR)/scripts/kconfig/Makefile srctree=$(CURDIR) src=scripts/kconfig obj=scripts/kconfig Q=$(Q) Kconfig=$(CURDIR)/src/Kconfig $1
endef

$(OUT)autoconf.h : $(KCONFIG_CONFIG) ; $(call do-kconfig, silentoldconfig)
$(KCONFIG_CONFIG): src/Kconfig ; $(call do-kconfig, olddefconfig)
%onfig: ; $(call do-kconfig, $@)
help: ; $(call do-kconfig, $@)


################ Generic rules

# Make definitions
.PHONY : all clean distclean FORCE
.DELETE_ON_ERROR:

clean:
	$(Q)rm -rf $(OUT)

distclean: clean
	$(Q)rm -f .config .config.old

-include $(patsubst %,$(OUT)%/*.d,$(DIRS))
