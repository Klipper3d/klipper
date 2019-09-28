#ifndef __CTR_H
#define __CTR_H
// Definitions for creating compile time requests.  The DECL_CTR macro
// produces requests (text strings) that are placed in a special
// section of the intermediate object files (*.o).  The build extracts
// these strings and places them in out/compile_time_requests.txt.
// The scripts/buildcommand.py code then generates
// out/compile_time_request.c from these requests.

#include "compiler.h" // __section

// Declare a compile time request
#define DECL_CTR(REQUEST)                                               \
    static char __PASTE(_DECLS_, __LINE__)[] __attribute__((used))      \
        __section(".compile_time_request") = (REQUEST)

// Macro to encode an integer for use with DECL_CTR_INT()
#define _CTR_HEX(H) ((H) > 9 ? (H) - 10 + 'A' : (H) + '0')
#define _CTR_SHIFT(V, S) _CTR_HEX(((uint32_t)(V) >> (S)) & 0x0f)
#define _CTR_INT(V, S) ((V) < 0 ? _CTR_SHIFT(-(V), (S)) : _CTR_SHIFT((V), (S)))
#define CTR_INT(VALUE) {                                \
        ' ', (VALUE) < 0 ? '-' : '+', '0', 'x',         \
        _CTR_INT((VALUE),28), _CTR_INT((VALUE),24),     \
        _CTR_INT((VALUE),20), _CTR_INT((VALUE),16),     \
        _CTR_INT((VALUE),12), _CTR_INT((VALUE),8),      \
        _CTR_INT((VALUE),4), _CTR_INT((VALUE),0) }

// Declare a compile time request with an integer expression
#define DECL_CTR_INT(REQUEST, PARAM_COUNT, args...)     \
    static struct {                                     \
        char _request[sizeof(REQUEST)-1];               \
        char _values[(PARAM_COUNT)][12];                \
        char _end_of_line;                              \
    } __PASTE(_DECLI_, __LINE__) __attribute__((used))  \
        __section(".compile_time_request") = {          \
        (REQUEST), { args }, 0 }

#endif // ctr.h
