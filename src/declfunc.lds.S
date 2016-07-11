// Linker script that defines symbols around sections.  The DECL_X()
// macros need this linker script to place _start and _end symbols
// around the list of declared items.
#include "autoconf.h" // CONFIG_MACH_AVR
#include "compiler.h" // __PASTE

#if CONFIG_MACH_AVR
#define DECL_SECTION .progmem.data.
#else
#define DECL_SECTION .rodata.
#endif

#define DECLWRAPPER(NAME)                       \
    __PASTE(DECL_SECTION, NAME) : SUBALIGN(1) { \
        NAME ## _start = . ;                    \
        *( .rodata. ## NAME ##.pre* )           \
        *( .rodata. ## NAME ##* )               \
        *( .rodata. ## NAME ##.post* )          \
        NAME ## _end = . ;                      \
    }

SECTIONS
{
    DECLWRAPPER(taskfuncs)
    DECLWRAPPER(initfuncs)
    DECLWRAPPER(shutdownfuncs)

    .compile_time_request.static_strings 0 (INFO) : {
        *( .compile_time_request.static_strings )
    }
    .compile_time_request.parsers 0 (INFO) : {
        *( .compile_time_request.parsers )
    }
}
