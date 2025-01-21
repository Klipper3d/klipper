OUTPUT_ARCH(or1k)
OUTPUT_FORMAT(elf32-or1k)
ENTRY (start)

STACK_SIZE = 0x200;
SRAM_A2_SIZE = 64K;
ORIG = 0x4000;
MEMORY {
  SRAM_A2 (rwx): ORIGIN = ORIG, LENGTH = SRAM_A2_SIZE
}

SECTIONS
{
  . = ORIG;

  .text . : ALIGN(4) {
    KEEP(*(.text.start))
    *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.text*)))
    . = ALIGN(4);
  } >SRAM_A2

  .data . : ALIGN(4) {
    *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
    __data_start = .;
    *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.data*)))
    . = ALIGN(4);
    __data_end = .;
  } >SRAM_A2

  .copy . : ALIGN(4) {
    __copy_start = .;
    . += __data_end - __data_start;
    __copy_end = .;
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

  ASSERT(. <= (SRAM_A2_SIZE), "Klipper image is too large")

  /DISCARD/ : {
    *(.comment*)
    *(.eh_frame_hdr*)
    *(.iplt*)
    *(.note*)
    *(.rela*)
    *( .compile_time_request )
  }
}
