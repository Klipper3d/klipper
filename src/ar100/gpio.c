// Copyright (C) 2020  Elias Bakken <elias@iagent.no>
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "gpio.h"
#include "util.h"

#define BANK(x) (x/32)
#define PIN(x) (x%32)
#define CFG_REG(x) ((x/8)*4)
#define CFG_OFF(x) ((x%8)*4)

struct gpio_mux gpio_mux_setup(uint8_t pin, enum pin_func func){
  uint8_t bank = BANK(pin);
  uint8_t p = PIN(pin);
  uint32_t data_reg = PIO_BASE + bank*0x24 + 0x10;
  uint32_t cfg_reg = PIO_BASE + bank*0x24 + CFG_REG(p);
  uint8_t cfg_off = CFG_OFF(p);

  if(bank == 0){ // Handle R_PIO
    data_reg = R_PIO_BASE + 0x10;
    cfg_reg = R_PIO_BASE + CFG_REG(p);
  }

  uint32_t curr_val = read_reg(cfg_reg) & ~(0xF<<cfg_off);
  write_reg(cfg_reg, curr_val | func<<cfg_off);

  struct gpio_mux ret = {
    .pin = p,
    .reg = data_reg
  };
  return ret;
}

struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val){
  struct gpio_mux mux = gpio_mux_setup(pin, PIO_OUTPUT);
  struct gpio_out ret = {
    .pin = mux.pin,
    .val = val,
    .reg = mux.reg
  };
  gpio_out_write(ret, val);

  return ret;
}

void gpio_out_write(struct gpio_out pin, uint8_t val){
  if(val)
    set_bit(pin.reg, pin.pin);
  else
    clear_bit(pin.reg, pin.pin);
  pin.val = val;
}

void gpio_out_toggle_noirq(struct gpio_out pin){
  gpio_out_write(pin, !pin.val);
}

uint8_t gpio_in_read(struct gpio_in pin){
  return !!(read_reg(pin.reg) & (1<<pin.pin));
}

struct gpio_in gpio_in_setup(uint8_t pin, uint8_t val){
  struct gpio_mux mux = gpio_mux_setup(pin, PIO_INPUT);
  struct gpio_in in = {
    .pin = mux.pin,
    .reg = mux.reg
  };

  in.val = gpio_in_read(in);

  return in;
}