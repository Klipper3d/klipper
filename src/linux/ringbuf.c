#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdatomic.h>
#include "ringbuf.h"

// Simple Single Producer Single Consumer Ring Buffer

void ring_buffer_init(struct ring_buf *rb)
{
    atomic_store(&rb->head, 0);
    atomic_store(&rb->tail, 0);
}

int ring_buffer_available_to_read(const struct ring_buf *rb)
{
    int head = atomic_load_explicit(&rb->head, memory_order_acquire);
    int tail = atomic_load_explicit(&rb->tail, memory_order_acquire);
    return (head >= tail) ? (head - tail) : (RING_BUFFER_SIZE + head - tail);
}

int ring_buffer_available_to_write(const struct ring_buf *rb)
{
    return RING_BUFFER_SIZE - 1 - ring_buffer_available_to_read(rb);
}

int ring_buffer_write(struct ring_buf *rb, const uint8_t *data, int length)
{
    int available = ring_buffer_available_to_write(rb);
    int to_write = (length < available) ? length : available;

    int head = atomic_load_explicit(&rb->head, memory_order_acquire);
    int first_chunk = RING_BUFFER_SIZE - head;
    if (first_chunk > to_write) {
        first_chunk = to_write;
    }
    memcpy(&rb->buffer[head], data, first_chunk);
    head = (head + first_chunk) % RING_BUFFER_SIZE;
    atomic_store_explicit(&rb->head, head, memory_order_release);

    int second_chunk = to_write - first_chunk;
    if (second_chunk > 0) {
        memcpy(&rb->buffer[0], data + first_chunk, second_chunk);
    }
    head = (head + second_chunk) % RING_BUFFER_SIZE;
    atomic_store_explicit(&rb->head, head, memory_order_release);

    return to_write;
}

int ring_buffer_read(struct ring_buf *rb, uint8_t *data, int length)
{
    int available = ring_buffer_available_to_read(rb);
    int to_read = (length < available) ? length : available;
    if (to_read == 0)
        return 0;

    int tail = atomic_load_explicit(&rb->tail, memory_order_acquire);
    int first_chunk = RING_BUFFER_SIZE - tail;
    if (first_chunk > to_read) {
        first_chunk = to_read;
    }
    memcpy(data, &rb->buffer[tail], first_chunk);
    tail = (tail + first_chunk) % RING_BUFFER_SIZE;
    atomic_store_explicit(&rb->tail, tail, memory_order_release);

    int second_chunk = to_read - first_chunk;
    if (second_chunk > 0) {
        memcpy(data + first_chunk, &rb->buffer[0], second_chunk);
        tail = (tail + second_chunk) % RING_BUFFER_SIZE;
    }
    atomic_store_explicit(&rb->tail, tail, memory_order_release);

    return to_read;
}
