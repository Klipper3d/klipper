// Helper with queueing single-value events
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "gpioqueue.h"
#include "board/irq.h" // irq_disable

/**
 * @return NULL if no current event
 */
struct gpio_event* get_current_event(struct gpio_queue* queue) {
    return queue->first;
}

/**
 * @return NULL if no next event
 */
struct gpio_event* get_next_event(struct gpio_queue* queue) {
    if(!queue->first) {
        //no current! event
        return NULL;
    }
    queue->first = queue->first->next;
    return queue->first;
}
void init_queue(struct gpio_queue* queue)
{
    queue->first = NULL;
    queue->plast = NULL;

    move_request_size(sizeof(struct gpio_event));
}
void free_gpio_event(struct gpio_event* event)
{
    irq_disable();
    move_free(event);
    irq_enable();
}

/**
 * @return NULL if queue was empty and a timer needs to be added
 */
uint8_t insert_gpio_event(struct gpio_queue* queue, uint32_t waketime,
                          uint16_t value) {
    uint8_t needs_adding_timer = 0;
    struct gpio_event* event = move_alloc();
    event->waketime = waketime;
    event->value = value;
    event->next = NULL;


    irq_disable();
    if(queue->first) {
        //there exists an element in queue
        //if there is a queue->first, there has to be a queue->plast
        //if there is no first, plast is invalid.
        //enqueue new event into the last's "next" element
        *queue->plast = event;
    }
    else {
        // no first element set
        queue->first = event;
        needs_adding_timer = 1;
    }

    //plast to point to this new element's next pointer
    queue->plast = &event->next;
    irq_enable();
    return !needs_adding_timer;
}
