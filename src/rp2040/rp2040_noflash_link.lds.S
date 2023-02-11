// rp2040 linker script (based on armcm_link.lds.S and customized for no flash device)
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_FLASH_SIZE

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY
{
  ram (rwx) : ORIGIN = CONFIG_RAM_START , LENGTH = CONFIG_RAM_SIZE
}

SECTIONS
{
    .main : {
        . = ALIGN(4);
        KEEP(*(.rp2040_main_noflash))
        __logical_binary_start = .;
    } > ram

    .text : {
        . = ALIGN(256);
        KEEP(*(.vector_table))
        _text_vectortable_end = .;
        *(.text .text.*)
        *(.rodata .rodata*)
    } > ram

    . = ALIGN(4);
    _data_flash = .;

    .data : AT (_data_flash)
    {
        . = ALIGN(4);
        _data_start = .;
        *(.ramfunc .ramfunc.*);
        *(.data .data.*);
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

    /DISCARD/ : {
        // The .init/.fini sections are used by __libc_init_array(), but
        // that isn't needed so no need to include them in the binary.
        *(.init)
        *(.fini)
    }
}
