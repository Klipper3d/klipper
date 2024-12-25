#include <stdint.h>

#define RING_BUFFER_SIZE (2048 - sizeof(int) * 2)
struct ring_buf
{
    uint8_t buffer[RING_BUFFER_SIZE];
    _Atomic int head;
    _Atomic int tail;
};

void ring_buffer_init(struct ring_buf *rb);
int ring_buffer_available_to_read(const struct ring_buf *rb);
int ring_buffer_available_to_write(const struct ring_buf *rb);
int ring_buffer_write(struct ring_buf *rb, const uint8_t *data, int length);
int ring_buffer_read(struct ring_buf *rb, uint8_t *data, int length);
