OUTPUT_ARCH(or1k)
OUTPUT_FORMAT(elf32-or1k)
ENTRY (start)

MEMORY {
  /*SRAM_A2 (rwx): ORIGIN = 0x00000000, LENGTH = 0x00014000*/
  SRAM_A2 (rwx): ORIGIN = 0x00000000, LENGTH = 0x00114000
}

SECTIONS
{
  . = 0x00010000;

  .text . : ALIGN(4) {
    KEEP(*(.text.start))
    *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.text*)))
    . = ALIGN(4);
  } >SRAM_A2

  .data . : ALIGN(4) {
    *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
    *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.data*)))
    . = ALIGN(4);
  } >SRAM_A2

  .bss . : ALIGN(4) {
    __bss_start = .;
    *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.bss*)))
    . = ALIGN(4);
    __bss_end = .;
  } >SRAM_A2

  __stack_start = .;
  . += 0x00000100;
  __stack_end = .;

  /*ASSERT(. <= 0x00013c00, "Firmware overflows into shared memory")*/

  __shared_mem = 0x00013c00;

  /DISCARD/ : {
    *(.comment*)
    *(.eh_frame_hdr*)
    *(.iplt*)
    *(.note*)
    *(.rela*)
    *( .compile_time_request )
  }
}
