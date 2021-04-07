# T5UID1 stm32 build rules

dirs-$(CONFIG_T5UID1_SERIAL) += src/stm32/t5uid1 src/generic/t5uid1

t5uid1-src-y := stm32/t5uid1/serial.c
t5uid1-src-$(CONFIG_MACH_STM32F0) := stm32/t5uid1/stm32f0_serial.c
src-$(CONFIG_T5UID1_SERIAL) += $(t5uid1-src-y) generic/t5uid1/serial_irq.c
