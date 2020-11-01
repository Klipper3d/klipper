// Helper with queueing single-value events
//
// Copyright (C) 2016 Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_disable
#include "queue.h"

/**
 * @return NULL if no current event
 */
struct event* get_current_event(struct queue* queue) {
    return queue->first;
}

/**
 * @return NULL if no next event
 */
struct event* get_next_event(struct queue* queue) {
    if(!queue->first) {
        //no current! event
        return NULL;
    }
    queue->first = queue->first->next;
    return queue->first;
}
void init_queue(struct queue* queue, size_t size_of_event)
{
    queue->first = NULL;
    queue->plast = NULL;

    move_request_size(size_of_event);
}

void* alloc_event(void)
{
    return move_alloc();
}

void free_event(void* event)
{
    irq_disable();
    move_free(event);
    irq_enable();
}

/**
 * @return NULL if queue was empty and a timer needs to be added
 */
uint8_t insert_event(struct queue* queue, struct event* event) {
    uint8_t needs_adding_timer = 0;
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
