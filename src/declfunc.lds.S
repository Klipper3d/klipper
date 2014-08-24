// Linker script that defines symbols around sections.  The DECL_X()
// macros need this linker script to place _start and _end symbols
// around the list of declared items.

#define DECLWRAPPER(NAME)                       \
    .progmem.data. ## NAME : SUBALIGN(1) {      \
        NAME ## _start = . ;                    \
        *( .progmem.data. ## NAME ##.pre* )     \
        *( .progmem.data. ## NAME ##* )         \
        *( .progmem.data. ## NAME ##.post* )    \
        NAME ## _end = . ;                      \
    }

SECTIONS
{
    DECLWRAPPER(taskfuncs)
    DECLWRAPPER(initfuncs)
    DECLWRAPPER(shutdownfuncs)
    DECLWRAPPER(commandfuncs)
}
