// Copyright (C) 2020  Elias Bakken <elias@iagent.no>
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h>

inline void  write_reg(uint32_t addr, uint32_t val){
  *((volatile unsigned long *)(addr)) = val;
}

inline uint32_t read_reg(uint32_t addr){
  return *((volatile unsigned long *)(addr));
}

inline void set_bit(uint32_t addr, uint8_t bit){
  write_reg(addr, read_reg(addr) | (1<<bit));
}

inline void clear_bit(uint32_t addr, uint8_t bit){
  write_reg(addr, read_reg(addr) & ~(1<<bit));
}