#ifndef __COMPILER_H
#define __COMPILER_H
// Low level definitions for C languange and gcc compiler.

#define barrier() __asm__ __volatile__("": : :"memory")

#define noinline __attribute__((noinline))
#define __visible __attribute__((externally_visible))

#define PACKED __attribute__((packed))
#define __aligned(x) __attribute__((aligned(x)))
#define __section(S) __attribute__((section(S)))

#define __stringify_1(x)        #x
#define __stringify(x)          __stringify_1(x)

#define ___PASTE(a,b) a##b
#define __PASTE(a,b) ___PASTE(a,b)

#endif // compiler.h
