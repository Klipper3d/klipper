#ifndef __AR100_INTERNAL_H
#define __AR100_INTERNAL_H

#define R_PIO_BASE  0x01F02C00
#define PIO_BASE    0x01C20800

enum pin_func {
    PIO_INPUT,
    PIO_OUTPUT,
    PIO_ALT1,
    PIO_ALT2,
    PIO_ALT3,
    PIO_ALT4,
    PIO_ALT5,
    PIO_DISABLE
};

struct gpio_mux {
    uint32_t pin;
    uint8_t bank;
    uint32_t reg;
};

struct gpio_mux gpio_mux_setup(uint8_t pin, enum pin_func func);
static inline unsigned long mfspr(unsigned long add){
    unsigned long ret;
    __asm__ __volatile__ ("l.mfspr %0,r0,%1" : "=r" (ret) : "K" (add));
    return ret;
}
static inline void mtspr(unsigned long add, unsigned long val){
    __asm__ __volatile__ ("l.mtspr r0,%1,%0" : : "K" (add), "r" (val));
}

#endif // internal.h
