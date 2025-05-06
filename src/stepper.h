#ifndef __STEPPER_H
#define __STEPPER_H

#include <stdint.h> // uint8_t

uint_fast8_t stepper_event(struct timer *t);
void stepper_stop_all(void);

#endif // stepper.h
