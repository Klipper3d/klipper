OUTPUT_ARCH(or1k)
OUTPUT_FORMAT(elf32-or1k)
ENTRY (start)

STACK_SIZE = 0x200;
SRAM_SIZE = 16M;
MEMORY {
  SRAM_A2 (rwx): ORIGIN = 0x00010000, LENGTH = SRAM_SIZE
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

      __stack_start = .;
      . += STACK_SIZE;
      __stack_end = .;
  } >SRAM_A2

  /DISCARD/ : {
    *(.comment*)
    *(.eh_frame_hdr*)
    *(.iplt*)
    *(.note*)
    *(.rela*)
    *( .compile_time_request )
  }
}
