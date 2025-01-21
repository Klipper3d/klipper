#ifndef __AVR_PGM_H
#define __AVR_PGM_H
// This header provides the avr/pgmspace.h definitions for "PROGMEM"
// on AVR platforms.

#include <avr/pgmspace.h>

#define NEED_PROGMEM 1

#define READP(VAR) ({                                                   \
    _Pragma("GCC diagnostic push");                                     \
    _Pragma("GCC diagnostic ignored \"-Wint-to-pointer-cast\"");        \
    typeof(VAR) __val =                                                 \
        __builtin_choose_expr(sizeof(VAR) == 1,                         \
            (typeof(VAR))pgm_read_byte(&(VAR)),                         \
        __builtin_choose_expr(sizeof(VAR) == 2,                         \
            (typeof(VAR))pgm_read_word(&(VAR)),                         \
        __builtin_choose_expr(sizeof(VAR) == 4,                         \
            (typeof(VAR))pgm_read_dword(&(VAR)),                        \
        __force_link_error__unknown_type)));                            \
    _Pragma("GCC diagnostic pop");                                      \
    __val;                                                              \
    })

extern void __force_link_error__unknown_type(void);

#endif // pgm.h
