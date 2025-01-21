// Helper code for the Klipper mcu protocol "message blocks"
//
// Copyright (C) 2016-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "msgblock.h" // message_alloc
#include "pyhelper.h" // errorf


/****************************************************************
 * Serial protocol helpers
 ****************************************************************/

// Implement the standard crc "ccitt" algorithm on the given buffer
uint16_t
msgblock_crc16_ccitt(uint8_t *buf, uint8_t len)
{
    uint16_t crc = 0xffff;
    while (len--) {
        uint8_t data = *buf++;
        data ^= crc & 0xff;
        data ^= data << 4;
        crc = ((((uint16_t)data << 8) | (crc >> 8)) ^ (uint8_t)(data >> 4)
               ^ ((uint16_t)data << 3));
    }
    return crc;
}

// Verify a buffer starts with a valid mcu message
int
msgblock_check(uint8_t *need_sync, uint8_t *buf, int buf_len)
{
    if (buf_len < MESSAGE_MIN)
        // Need more data
        return 0;
    if (*need_sync)
        goto error;
    uint8_t msglen = buf[MESSAGE_POS_LEN];
    if (msglen < MESSAGE_MIN || msglen > MESSAGE_MAX)
        goto error;
    uint8_t msgseq = buf[MESSAGE_POS_SEQ];
    if ((msgseq & ~MESSAGE_SEQ_MASK) != MESSAGE_DEST)
        goto error;
    if (buf_len < msglen)
        // Need more data
        return 0;
    if (buf[msglen-MESSAGE_TRAILER_SYNC] != MESSAGE_SYNC)
        goto error;
    uint16_t msgcrc = ((buf[msglen-MESSAGE_TRAILER_CRC] << 8)
                       | (uint8_t)buf[msglen-MESSAGE_TRAILER_CRC+1]);
    uint16_t crc = msgblock_crc16_ccitt(buf, msglen-MESSAGE_TRAILER_SIZE);
    if (crc != msgcrc)
        goto error;
    return msglen;

error: ;
    // Discard bytes until next SYNC found
    uint8_t *next_sync = memchr(buf, MESSAGE_SYNC, buf_len);
    if (next_sync) {
        *need_sync = 0;
        return -(next_sync - buf + 1);
    }
    *need_sync = 1;
    return -buf_len;
}

// Encode an integer as a variable length quantity (vlq)
static uint8_t *
encode_int(uint8_t *p, uint32_t v)
{
    int32_t sv = v;
    if (sv < (3L<<5)  && sv >= -(1L<<5))  goto f4;
    if (sv < (3L<<12) && sv >= -(1L<<12)) goto f3;
    if (sv < (3L<<19) && sv >= -(1L<<19)) goto f2;
    if (sv < (3L<<26) && sv >= -(1L<<26)) goto f1;
    *p++ = (v>>28) | 0x80;
f1: *p++ = ((v>>21) & 0x7f) | 0x80;
f2: *p++ = ((v>>14) & 0x7f) | 0x80;
f3: *p++ = ((v>>7) & 0x7f) | 0x80;
f4: *p++ = v & 0x7f;
    return p;
}

// Parse an integer that was encoded as a "variable length quantity"
static uint32_t
parse_int(uint8_t **pp)
{
    uint8_t *p = *pp, c = *p++;
    uint32_t v = c & 0x7f;
    if ((c & 0x60) == 0x60)
        v |= -0x20;
    while (c & 0x80) {
        c = *p++;
        v = (v<<7) | (c & 0x7f);
    }
    *pp = p;
    return v;
}

// Parse the VLQ contents of a message
int
msgblock_decode(uint32_t *data, int data_len, uint8_t *msg, int msg_len)
{
    uint8_t *p = &msg[MESSAGE_HEADER_SIZE];
    uint8_t *end = &msg[msg_len - MESSAGE_TRAILER_SIZE];
    while (data_len--) {
        if (p >= end)
            return -1;
        *data++ = parse_int(&p);
    }
    if (p != end)
        // Invalid message
        return -1;
    return 0;
}


/****************************************************************
 * Command queues
 ****************************************************************/

// Allocate a 'struct queue_message' object
struct queue_message *
message_alloc(void)
{
    struct queue_message *qm = malloc(sizeof(*qm));
    memset(qm, 0, sizeof(*qm));
    return qm;
}

// Allocate a queue_message and fill it with the specified data
struct queue_message *
message_fill(uint8_t *data, int len)
{
    struct queue_message *qm = message_alloc();
    memcpy(qm->msg, data, len);
    qm->len = len;
    return qm;
}

// Allocate a queue_message and fill it with a series of encoded vlq integers
struct queue_message *
message_alloc_and_encode(uint32_t *data, int len)
{
    struct queue_message *qm = message_alloc();
    int i;
    uint8_t *p = qm->msg;
    for (i=0; i<len; i++) {
        p = encode_int(p, data[i]);
        if (p > &qm->msg[MESSAGE_PAYLOAD_MAX])
            goto fail;
    }
    qm->len = p - qm->msg;
    return qm;

fail:
    errorf("Encode error");
    qm->len = 0;
    return qm;
}

// Free the storage from a previous message_alloc() call
void
message_free(struct queue_message *qm)
{
    free(qm);
}

// Free all the messages on a queue
void
message_queue_free(struct list_head *root)
{
    while (!list_empty(root)) {
        struct queue_message *qm = list_first_entry(
            root, struct queue_message, node);
        list_del(&qm->node);
        message_free(qm);
    }
}


/****************************************************************
 * Clock estimation
 ****************************************************************/

// Extend a 32bit clock value to its full 64bit value
uint64_t
clock_from_clock32(struct clock_estimate *ce, uint32_t clock32)
{
    return ce->last_clock + (int32_t)(clock32 - ce->last_clock);
}

// Convert a clock to its estimated time
double
clock_to_time(struct clock_estimate *ce, uint64_t clock)
{
    return ce->conv_time + (int64_t)(clock - ce->conv_clock) / ce->est_freq;
}

// Convert a time to the nearest clock value
uint64_t
clock_from_time(struct clock_estimate *ce, double time)
{
    return (int64_t)((time - ce->conv_time)*ce->est_freq + .5) + ce->conv_clock;
}
