/* Linker script for atsamd chips
 *
 * Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
 *
 * This file may be distributed under the terms of the GNU GPLv3 license.
 */

#include "autoconf.h"

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY
{
  rom (rx) : ORIGIN = CONFIG_FLASH_START , LENGTH = CONFIG_FLASH_SIZE
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = CONFIG_RAM_SIZE
}

SECTIONS
{
    .text : {
        . = ALIGN(4);
        _sfixed = .;
        KEEP(*(.vectors .vectors.*))
        *(.text .text.*)
        *(.rodata .rodata*)

        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        KEEP(*(.fini))
    } > rom

    . = ALIGN(4);
    _etext = .;

    .relocate : AT (_etext)
    {
        . = ALIGN(4);
        _srelocate = .;
        *(.ramfunc .ramfunc.*);
        *(.data .data.*);
        . = ALIGN(4);
        _erelocate = .;
    } > ram

    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _szero = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ezero = .;
    } > ram

    _sstack = 0x20000000 + CONFIG_RAM_SIZE - CONFIG_STACK_SIZE ;
    .stack _sstack (NOLOAD) :
    {
        . = . + CONFIG_STACK_SIZE;
        _estack = .;
    } > ram
}
