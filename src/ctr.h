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

// Helper macros for encoding an integer
#define CTR_HEX(H) ((H) > 9 ? (H) - 10 + 'A' : (H) + '0')
#define CTR_INT(V, S) CTR_HEX(((uint32_t)(V) >> (S)) & 0x0f)

// Declare a compile time request with an integer expression
#define DECL_CTR_INT(REQUEST, VALUE)                            \
    static struct { char _r[sizeof(REQUEST)]; char _v[12]; }    \
        __PASTE(_DECLI_, __LINE__) __attribute__((used))        \
            __section(".compile_time_request") = {              \
            REQUEST " ", {                                      \
                (VALUE) < 0 ? '-' : '+', '0', 'x',              \
                CTR_INT((VALUE),28), CTR_INT((VALUE),24),       \
                CTR_INT((VALUE),20), CTR_INT((VALUE),16),       \
                CTR_INT((VALUE),12), CTR_INT((VALUE),8),        \
                CTR_INT((VALUE),4), CTR_INT((VALUE),0), 0 } }

#endif // ctr.h
