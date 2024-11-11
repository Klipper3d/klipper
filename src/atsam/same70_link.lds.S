// Generic ARM Cortex-M linker script
//
// Copyright (C) 2019-2024  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2024  Luke Vuksta <wulfstawulfsta@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_FLASH_APPLICATION_ADDRESS

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY
{
  rom (rx) : ORIGIN = CONFIG_FLASH_APPLICATION_ADDRESS , LENGTH = CONFIG_FLASH_SIZE
  ram (rwx) : ORIGIN = CONFIG_RAM_START , LENGTH = CONFIG_RAM_SIZE
  dtcm (rw) : ORIGIN = CONFIG_ARMCM_DTCM_START , LENGTH = CONFIG_ARMCM_DTCM_SIZE
}

SECTIONS
{
    .text : AT (CONFIG_ARMCM_ITCM_FLASH_MIRROR_START) {
        . = ALIGN(4);
        _text_vectortable_start = .;
        KEEP(*(.vector_table))
        _text_vectortable_end = .;
        *(.text .text.*)
        *(.ramfunc .ramfunc.*)
        *(.rodata .rodata*)
    } > rom

    _text_size = SIZEOF (.text);
    . = CONFIG_ARMCM_ITCM_FLASH_MIRROR_START + _text_size;
    . = ALIGN(4);
    _data_flash = .;

    .data : AT (_data_flash)
    {
        . = ALIGN(4);
        _data_start = .;
        *(.data .data.*)
        . = ALIGN(4);
        _data_end = .;
    } > ram

    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
    } > ram

    _stack_start = CONFIG_RAM_START + CONFIG_RAM_SIZE - CONFIG_STACK_SIZE ;
    .stack _stack_start (NOLOAD) :
    {
        . = . + CONFIG_STACK_SIZE;
        _stack_end = .;
    } > ram

    .dtcm_bss (NOLOAD) :
    {
        . = ALIGN(4);
        _dtcm_bss_start = .;
        *(.dtcm.bss)
        . = ALIGN(4);
        _dtcm_bss_end = .;
    } > dtcm

    /DISCARD/ : {
        // The .init/.fini sections are used by __libc_init_array(), but
        // that isn't needed so no need to include them in the binary.
        *(.init)
        *(.fini)
        // Don't include exception tables
        *(.ARM.extab)
        *(.ARM.exidx)
    }
}
