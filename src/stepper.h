#ifndef __STEPPER_H
#define __STEPPER_H

#include <stdint.h> // uint8_t

uint_fast8_t stepper_event(struct timer *t);
struct stepper *stepper_oid_lookup(uint8_t oid);
void stepper_stop(struct stepper *s);
void stepper_schedule_position_mode(struct stepper *s, uint32_t clock);
void stepper_set_speed_mode(struct stepper *s, int32_t min_pos,
                            int32_t max_pos);
void stepper_set_target_speed(struct stepper* s, int32_t target_speed);
int32_t stepper_position(struct stepper *s);
uint16_t stepper_speed(struct stepper *s);

#endif // stepper.h
