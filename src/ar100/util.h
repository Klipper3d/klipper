#include <stdint.h>
#include <stddef.h>

void *memcpy(void *restrict dest, const void *restrict src, size_t n);
void *memset(void *restrict dest, int c, size_t n);

inline void write_reg(uint32_t addr, uint32_t val){
  *((volatile unsigned long *)(addr)) = val;
}

inline uint32_t read_reg(uint32_t addr){
  return *((volatile unsigned long *)(addr));
}

void set_bit(uint32_t addr, uint8_t bit);

void clear_bit(uint32_t addr, uint8_t bit);
