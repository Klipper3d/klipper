// Commands for controlling GPIO pins
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef SRC_GPIOQUEUE_H_
#define SRC_GPIOQUEUE_H_

#include "basecmd.h" // oid_alloc

struct gpio_event {
    uint32_t waketime;
    uint16_t value;
    struct gpio_event *next;
};

struct gpio_queue {
    struct gpio_event *first, **plast;
};

/**
 * @return NULL if no current event
 */
struct gpio_event* get_current_event(struct gpio_queue* queue);
/**
 * @return NULL if no next event
 */
struct gpio_event* get_next_event(struct gpio_queue* queue);
void init_queue(struct gpio_queue* queue);
void free_gpio_event(struct gpio_event* current);
/**
 * @return NULL if queue was empty and a timer needs to be added
 */
uint8_t insert_gpio_event(struct gpio_queue* queue, uint32_t waketime,
                          uint16_t value);


#endif /* SRC_GPIOQUEUE_H_ */
