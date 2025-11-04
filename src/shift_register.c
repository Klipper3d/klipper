// Support for 74HC595 shift registers
//
// Copyright (C) 2023  <Your Name>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "shift_register.h" // self
#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_disable
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_shutdown
#include <stdlib.h> // malloc


// Lookup a shift register by its oid
struct shift_register *
shift_register_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_shift_register);
}

// Update the physical shift register with the current state
void
shift_register_update(struct shift_register *sr)
{
    irqstatus_t flag = irq_save();
    
    // Pull latch low to start data transfer
    gpio_out_write(sr->latch_pin, 0);
    
    // Shift out data MSB first
    for (int i = sr->num_registers - 1; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            uint8_t bit = (sr->state[i] >> j) & 1;
            gpio_out_write(sr->data_pin, bit);
            
            // Pulse clock
            gpio_out_write(sr->clock_pin, 1);
            gpio_out_write(sr->clock_pin, 0);
        }
    }
    
    // Pull latch high to latch the data
    gpio_out_write(sr->latch_pin, 1);
    
    irq_restore(flag);
}

// Set a specific pin on the shift register
void
shift_register_set_pin(struct shift_register *sr, uint8_t pin, uint8_t value)
{
    uint8_t reg_index = pin / 8;
    uint8_t bit_index = pin % 8;
    
    if (reg_index >= sr->num_registers)
        return;
    
    irqstatus_t flag = irq_save();
    
    if (value)
        sr->state[reg_index] |= (1 << bit_index);
    else
        sr->state[reg_index] &= ~(1 << bit_index);
    
    irq_restore(flag);
    
    // Update the physical shift register
    shift_register_update(sr);
}

// Get the current state of a specific pin on the shift register
uint8_t
shift_register_get_pin(struct shift_register *sr, uint8_t pin)
{
    uint8_t reg_index = pin / 8;
    uint8_t bit_index = pin % 8;
    
    if (reg_index >= sr->num_registers)
        return 0;
    
    return (sr->state[reg_index] >> bit_index) & 1;
}

// Setup a shift register
void
command_config_shift_register(uint32_t *args)
{
    uint8_t oid = args[0];
    struct shift_register *sr = oid_alloc(oid, command_config_shift_register, sizeof(*sr));
    
    sr->data_pin = gpio_out_setup(args[1], 0);
    sr->clock_pin = gpio_out_setup(args[2], 0);
    sr->latch_pin = gpio_out_setup(args[3], 1);  // Latch high by default
    sr->num_registers = args[4];
    
    // Allocate memory for state
    sr->state = malloc(sr->num_registers);
    if (!sr->state)
        shutdown("Failed to allocate memory for shift register state");
    
    // Initialize state to all zeros
    for (int i = 0; i < sr->num_registers; i++)
        sr->state[i] = 0;
    
    // Initial update to ensure all outputs are low
    shift_register_update(sr);
}
DECL_COMMAND(command_config_shift_register, 
             "config_shift_register oid=%c data_pin=%c clock_pin=%c latch_pin=%c num_registers=%c");


// Set a specific pin on a shift register
void
command_shift_register_set_pin(uint32_t *args)
{
    struct shift_register *sr = shift_register_oid_lookup(args[0]);
    uint8_t pin = args[1];
    uint8_t value = args[2];
    
    shift_register_set_pin(sr, pin, value);
}
DECL_COMMAND(command_shift_register_set_pin, "shift_register_set_pin oid=%c pin=%c value=%c");

// Shutdown handler to ensure all shift register pins are set to a safe state
void
shift_register_shutdown(void)
{
    uint8_t i;
    struct shift_register *sr;
    foreach_oid(i, sr, command_config_shift_register) {
        // Set all pins to 0
        for (int j = 0; j < sr->num_registers; j++)
            sr->state[j] = 0;
        shift_register_update(sr);
    }
}
DECL_SHUTDOWN(shift_register_shutdown);