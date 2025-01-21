// Helper functions for ar100
//
// Copyright (C) 2020-2021  Elias Bakken <elias@iagent.no>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "util.h"

void *memcpy(void *restrict dest, const void *restrict src, size_t n){
   // Typecast src and dest addresses to (char *)
   char *csrc = (char *)src;
   char *cdest = (char *)dest;

   // Copy contents of src[] to dest[]
   for (int i=0; i<n; i++)
       cdest[i] = csrc[i];
   return dest;
}

void *memset(void *dest, int c, size_t n){
  unsigned char *s = dest;
  for(; n; n--){
    *s++ = c;
  }
  return dest;
}

void set_bit(uint32_t addr, uint8_t bit){
  write_reg(addr, read_reg(addr) | (1<<bit));
}

void clear_bit(uint32_t addr, uint8_t bit){
  write_reg(addr, read_reg(addr) & ~(1<<bit));
}
