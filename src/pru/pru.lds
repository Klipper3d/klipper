/* Final linker script klipper.elf on the PRU */
SECTIONS
{
    /* binutils on the PRU doesn't support --gc-sections so manually
     * discard the .compile_time_request section */
    /DISCARD/ : {
        *( .compile_time_request )
    }
}
