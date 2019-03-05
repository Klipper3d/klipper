#ifndef __CTR_H
#define __CTR_H
// Definitions for creating compile time requests.  The DECL_CTR macro
// produces requests (text strings) that are placed in a special
// section of the intermediate object files.  The requests are then
// extracted during the build and passed to scripts/buildcommand.py.
// The scripts/buildcommand.py code then generates
// out/compile_time_request.c from these requests.

#include "compiler.h" // __section

// Declare a compile time request
#define DECL_CTR(REQUEST)                                               \
    static char __PASTE(_DECLS_, __LINE__)[] __attribute__((used))      \
        __section(".compile_time_request") = (REQUEST)

#define CTR_INT(V, S) ((((uint32_t)(V) >> S) & 0x3f) + 48)

// Declare a compile time request with an integer expression
#define DECL_CTR_INT(REQUEST, VALUE)                            \
    static struct { char _r[sizeof(REQUEST)]; char _v[8]; }     \
        __PASTE(_DECLI_, __LINE__) __attribute__((used))        \
            __section(".compile_time_request") = {              \
            REQUEST " ", {                                      \
                (VALUE) < 0 ? '-' : '+',                        \
                CTR_INT((VALUE),0), CTR_INT((VALUE),6),         \
                CTR_INT((VALUE),12), CTR_INT((VALUE),18),       \
                CTR_INT((VALUE),24), CTR_INT((VALUE),30), 0 } }

#endif // ctr.h
