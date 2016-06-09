#ifndef __STEPPER_H
#define __STEPPER_H

#include <stdint.h> // uint8_t

enum { STEPPER_POSITION_BIAS=0x40000000 };

uint_fast8_t stepper_event(struct timer *t);
void command_config_stepper(uint32_t *args);
struct stepper;
uint32_t stepper_get_position(struct stepper *s);
void stepper_stop(struct stepper *s);

#endif // stepper.h
