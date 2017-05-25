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

#endif // ctr.h
