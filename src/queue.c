// Helper with queueing events using the move queue
//
// Copyright (C) 2016 Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_disable
#include "basecmd.h" // oid_alloc
#include "queue.h"

/**
 * @return NULL if no current event
 */
struct mq_event* mq_event_peek(struct mq_list* queue) {
    return queue->first;
}

/**
 * @return NULL if no current event
 */
struct mq_event* mq_event_pop(struct mq_list* queue) {
    struct mq_event* current = queue->first;
    if(current) {
        queue->first = queue->first->next;
    }

    return current;
}
void mq_init(struct mq_list* queue, size_t size_of_event) {
    queue->first = NULL;
    queue->plast = NULL;

    move_request_size(size_of_event);
}

void* mq_alloc_event(void) {
    return move_alloc();
}

// Free previously allocated storage from move_alloc(). Caller must
// disable irqs.
void mq_free_event(void* event) {
    move_free(event);
}

/**
 * @return 0 if queue was empty and a timer needs to be added
 */
uint8_t mq_event_insert(struct mq_list* queue, struct mq_event* event) {
    uint8_t needs_adding_timer = 0;
    event->next = NULL;

    irqstatus_t irq = irq_save();
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
    irq_restore(irq);
    return !needs_adding_timer;
}
