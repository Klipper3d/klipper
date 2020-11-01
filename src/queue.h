// Commands for controlling GPIO pins
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef SRC__GENERIC_QUEUE_H_
#define SRC__GENERIC_QUEUE_H_

#include "basecmd.h" // oid_alloc

struct event {
    struct event *next;
};

struct queue {
    struct event *first, **plast;
};

/**
 * @return NULL if no current event
 */
struct event* get_current_event(struct queue* queue);
/**
 * @return NULL if no next event
 */
struct event* get_next_event(struct queue* queue);
void init_queue(struct queue* queue, size_t size_of_event);
void* alloc_event(void);
void free_event(void* current);
/**
 * @return NULL if queue was empty and a timer needs to be added
 */
uint8_t insert_event(struct queue* queue, struct event* event);

#define event_get_data(event_ptr, type) \
        container_of(event_ptr, type, event)

#endif /* SRC__GENERIC_QUEUE_H_ */
