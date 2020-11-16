#ifndef SRC__GENERIC_QUEUE_H_
#define SRC__GENERIC_QUEUE_H_

struct mq_event {
    struct mq_event *next;
};

struct mq_list {
    struct mq_event *first, **plast;
};

/**
 * @return NULL if no current event
 */
struct mq_event* mq_event_peek(struct mq_list* queue);
/**
 * @return NULL if no current event
 */
struct mq_event* mq_event_pop(struct mq_list* queue);

// discards queue _without_ freeing its elements
void mq_discard(struct mq_list* queue);

void mq_init(struct mq_list* queue, size_t size_of_event);
void* mq_alloc_event(void);

// Free previously allocated storage from move_alloc(). Caller must
// disable irqs.
void mq_free_event(void* current);
/**
 * @return 0 if queue was empty and a timer needs to be added
 */
uint8_t mq_event_insert(struct mq_list* queue, struct mq_event* event);

#endif /* SRC__GENERIC_QUEUE_H_ */
