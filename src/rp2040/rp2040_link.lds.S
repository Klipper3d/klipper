// rp2040 linker script (based on armcm_link.lds.S and customized for stage2)
//
// Copyright (C) 2019-2024  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_FLASH_SIZE

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

#if CONFIG_RP2040_HAVE_STAGE2
  #define ROM_ORIGIN 0x10000000
#else
  #define ROM_ORIGIN CONFIG_FLASH_APPLICATION_ADDRESS
#endif

MEMORY
{
  rom (rx) : ORIGIN = ROM_ORIGIN , LENGTH = CONFIG_FLASH_SIZE
  ram (rwx) : ORIGIN = CONFIG_RAM_START , LENGTH = CONFIG_RAM_SIZE
}

// Force flags for each output section to avoid RWX linker warning
PHDRS
{
    text_segment PT_LOAD FLAGS(5); // RX flags
    ram_vectortable_segment PT_LOAD FLAGS(6); // RW flags
    data_segment PT_LOAD FLAGS(6); // RW flags
    bss_segment PT_LOAD FLAGS(6); // RW flags
    stack_segment PT_LOAD FLAGS(6); // RW flags
}

SECTIONS
{
    .text : {
        . = ALIGN(4);
#if CONFIG_RP2040_HAVE_STAGE2
        KEEP(*(.boot2))
#endif
        _text_vectortable_start = .;
        KEEP(*(.vector_table))
        _text_vectortable_end = .;
        *(.text.armcm_boot*)
    } > rom :text_segment

    . = ALIGN(4);
    _data_flash = .;

    .ram_vectortable (NOLOAD) : {
        _ram_vectortable_start = .;
        . = . + ( _text_vectortable_end - _text_vectortable_start ) ;
        _ram_vectortable_end = .;
    } > ram :ram_vectortable_segment

    .data : AT (_data_flash)
    {
        . = ALIGN(4);
        _data_start = .;
        *(.text .text.*)
        *(.ramfunc .ramfunc.*);
        *(.rodata .rodata*)
        *(.data .data.*);
        . = ALIGN(4);
        _data_end = .;
    } > ram :data_segment

    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
    } > ram :bss_segment

    _stack_start = CONFIG_RAM_START + CONFIG_RAM_SIZE - CONFIG_STACK_SIZE ;
    .stack _stack_start (NOLOAD) :
    {
        . = . + CONFIG_STACK_SIZE;
        _stack_end = .;
    } > ram :stack_segment

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
