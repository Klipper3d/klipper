/*
******************************************************************************
**
**  File        : LinkerScript.ld
**
**  Abstract    : Linker script for STM32MP1 series
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Distribution: The file is distributed as is, without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** Copyright (c) 2021 STMicroelectronics.
** All rights reserved.
**
;* This software is licensed under terms that can be found in the LICENSE file
** in the root directory of this software component.
** If no LICENSE file comes with this software, it is provided AS-IS.
**
*****************************************************************************
*/

/* Highest address of the user mode stack */
_stack_end = 0x10040000;    /* end of RAM */

_Min_Heap_Size = 0x200 ;  /* required amount of heap  */
_Min_Stack_Size = 0x400 ; /* required amount of stack */

/* Memories definition */
MEMORY
{
  m_interrupts (RX)  : ORIGIN = 0x00000000, LENGTH = 0x00000298
  m_text       (RX)  : ORIGIN = 0x10000000, LENGTH = 0x00020000
  m_data       (RW)  : ORIGIN = 0x10020000, LENGTH = 0x00020000
  m_ipc_shm    (RW)  : ORIGIN = 0x10040000, LENGTH = 0x00008000
}

/* Sections */
SECTIONS
{

  /* The startup code into ROM memory */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.vector_table))
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } > m_interrupts


  /* The program code and other data into ROM memory */
  .text :
  {
    . = ALIGN(4);

    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)


    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } > m_text

  /* Constant data into ROM memory*/
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } > m_text

  .ARM.extab   : {
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } > m_text

  .ARM : {
    . = ALIGN(4);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  } > m_text

  /* Used by the startup to initialize data */
  __DATA_ROM = .;
  _data_flash = LOADADDR(.data);

  /* Initialized data sections */
  .data :  AT(__DATA_ROM)
  {
    . = ALIGN(4);
    _data_start = .;        /* create a global symbol at data start */
    *(.ramfunc .ramfunc.*);
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _data_end = .;        /* define a global symbol at data end */
  } > m_data

  __DATA_END = __DATA_ROM + (_data_end - _data_start);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")

  .resource_table :
  {
    . = ALIGN(4);
    KEEP (*(.resource_table*))
    . = ALIGN(4);
  } > m_data
  _eirsc = LOADADDR(.resource_table) + SIZEOF(.resource_table);
  ASSERT(SIZEOF(.resource_table) == 0 || _eirsc <= text_end, "no room for resource_table load section in text")

  /* Uninitialized data section into RAM memory */
  . = ALIGN(4);
  .bss : AT (ADDR (.bss))
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
  } > m_data

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    _stack_start = .;
    . = . + _Min_Stack_Size;
    _stack_end = .;
    . = ALIGN(8);
  } > m_data

  /* Remove information from the compiler libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
    // The .init/.fini sections are used by __libc_init_array(), but
    // that isn't needed so no need to include them in the binary.
    *(.init)
    *(.fini)
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }

}
