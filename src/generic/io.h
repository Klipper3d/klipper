#ifndef __GENERIC_IO_H
#define __GENERIC_IO_H

#include <stdint.h> // uint32_t
#include "compiler.h" // barrier

static inline void writel(void *addr, uint32_t val) {
    barrier();
    *(volatile uint32_t *)addr = val;
}
static inline void writew(void *addr, uint16_t val) {
    barrier();
    *(volatile uint16_t *)addr = val;
}
static inline void writeb(void *addr, uint8_t val) {
    barrier();
    *(volatile uint8_t *)addr = val;
}
static inline uint32_t readl(const void *addr) {
    uint32_t val = *(volatile const uint32_t *)addr;
    barrier();
    return val;
}
static inline uint16_t readw(const void *addr) {
    uint16_t val = *(volatile const uint16_t *)addr;
    barrier();
    return val;
}
static inline uint8_t readb(const void *addr) {
    uint8_t val = *(volatile const uint8_t *)addr;
    barrier();
    return val;
}

#endif // io.h
