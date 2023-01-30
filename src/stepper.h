#ifndef __STEPPER_H
#define __STEPPER_H

#include <stdint.h> // uint8_t
#include "sched.h" // struct timer

uint_fast8_t stepper_event(struct timer *t);
void stepper_manual_stop(uint32_t oid);

#endif // stepper.h
