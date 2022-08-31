/*
******************************************************************************
** @file       : stm32mp157x_ram_cm4.ld
**
** @author     : STM32CubeIDE
**
** Abstract    : Linker script for STM32MP157x Device from STM32MP1 series
**                      64  Kbytes RETRAM
**                      128 Kbytes SRAM1
**                      128 Kbytes SRAM2
**                      64  Kbytes SRAM3
**                      64  Kbytes SRAM4
**
**               Set heap size, stack size and stack location according
**               to application requirements.
**
**               Set memory bank area and size if external memory is used
**
** Target      : STMicroelectronics STM32
**
** Distribution: The file is distributed as is, without any warranty
**               of any kind.
**
******************************************************************************
** @attention
**
** Copyright (c) 2021 STMicroelectronics.
** All rights reserved.
**
** This software is licensed under terms that can be found in the LICENSE file
** in the root directory of this software component.
** If no LICENSE file comes with this software, it is provided AS-IS.
**
******************************************************************************
*/

/* Highest address of the user mode stack */
_stack_end = ORIGIN(SRAM2_data) + LENGTH(SRAM2_data); /* end of "SRAM2_data" Ram type memory */

_Min_Heap_Size = 0x200 ; /* required amount of heap */
_Min_Stack_Size = 0x400 ; /* required amount of stack */

_stack_start = _stack_end - _Min_Stack_Size;

/* Memories definition */
MEMORY
{
  RETRAM_interrupts (xrw)  : ORIGIN = 0x00000000,  LENGTH = 0x00000600
  SRAM1_text        (xrw)  : ORIGIN = 0x10000000,  LENGTH = 128K
  SRAM2_data        (xrw)  : ORIGIN = 0x10020000,  LENGTH = 128K
  SRAM3_ipc_shm     (xrw)  : ORIGIN = 0x10040000,  LENGTH = 64K
  SRAM4             (xrw)  : ORIGIN = 0x10050000,  LENGTH = 64K
}

/* Symbols needed for OpenAMP to enable rpmsg */
__OPENAMP_region_start__ = ORIGIN(SRAM3_ipc_shm);
__OPENAMP_region_end__   = ORIGIN(SRAM3_ipc_shm) + LENGTH(SRAM3_ipc_shm);

/* Sections */
SECTIONS
{
  /* The startup code into "RETRAM_interrupts" Ram type memory */
  .vector_table :
  {
    . = ALIGN(4);
    KEEP(*(.vector_table)) /* Startup code */
    . = ALIGN(4);
  } >RETRAM_interrupts

  /* The program code and other data into "SRAM1_text" Ram type memory */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >SRAM1_text

  /* Constant data into "SRAM1_text" Ram type memory */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >SRAM1_text

  .ARM.extab   : {
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } >SRAM1_text

  .ARM : {
    . = ALIGN(4);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  } >SRAM1_text

  .preinit_array     :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  } >SRAM1_text

  .init_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  } >SRAM1_text

  .fini_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  } >SRAM1_text

  /* Used by the startup to initialize data */
  _data_flash = .;
  _sidata = LOADADDR(.data);

  /* Initialized data sections into "SRAM2_data" Ram type memory */
  .data :  AT(_data_flash)
  {
    . = ALIGN(4);
    _data_start = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _data_end = .;        /* define a global symbol at data end */
  } >SRAM2_data

  __DATA_END = _data_flash + (_data_end - _data_start);
  text_end = ORIGIN(SRAM1_text) + LENGTH(SRAM1_text);
  ASSERT(__DATA_END <= text_end, "region SRAM1_text overflowed with text and data")


  .resource_table :
  {
    . = ALIGN(4);
    KEEP (*(.resource_table*))
    . = ALIGN(4);
  } >SRAM2_data

  /* Uninitialized data section into "SRAM2_data" Ram type memory */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _bss_start = .;         /* define a global symbol at bss start */
    __bss_start__ = _bss_start;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _bss_end = .;         /* define a global symbol at bss end */
    __bss_end__ = _bss_end;
  } >SRAM2_data

  /* User_heap_stack section, used to check that there is enough "SRAM2_data" Ram  type memory left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >SRAM2_data

  /* Remove information from the compiler libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
