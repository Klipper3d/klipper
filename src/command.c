// Code for parsing incoming commands and encoding outgoing messages
//
// Copyright (C) 2016-2024  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdarg.h> // va_start
#include <string.h> // memcpy
#include "board/io.h" // readb
#include "board/irq.h" // irq_poll
#include "board/misc.h" // crc16_ccitt
#include "board/pgm.h" // READP
#include "command.h" // output_P
#include "sched.h" // sched_is_shutdown

static uint8_t next_sequence = MESSAGE_DEST;

static uint32_t
command_encode_ptr(void *p)
{
    if (sizeof(size_t) > sizeof(uint32_t))
        return p - console_receive_buffer();
    return (size_t)p;
}

void *
command_decode_ptr(uint32_t v)
{
    if (sizeof(size_t) > sizeof(uint32_t))
        return console_receive_buffer() + v;
    return (void*)(size_t)v;
}


/****************************************************************
 * Binary message parsing
 ****************************************************************/

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

// Write an encoded msgid (optimized 2-byte VLQ encoder)
static uint8_t *
encode_msgid(uint8_t *p, uint_fast16_t encoded_msgid)
{
    if (encoded_msgid >= 0x80)
        *p++ = (encoded_msgid >> 7) | 0x80;
    *p++ = encoded_msgid & 0x7f;
    return p;
}

// Parse an encoded msgid (optimized 2-byte parser, return as positive number)
uint_fast16_t
command_parse_msgid(uint8_t **pp)
{
    uint8_t *p = *pp;
    uint_fast16_t encoded_msgid = *p++;
    if (encoded_msgid & 0x80)
        encoded_msgid = ((encoded_msgid & 0x7f) << 7) | (*p++);
    *pp = p;
    return encoded_msgid;
}

// Parse an incoming command into 'args'
uint8_t *
command_parsef(uint8_t *p, uint8_t *maxend
               , const struct command_parser *cp, uint32_t *args)
{
    uint_fast8_t num_params = READP(cp->num_params);
    const uint8_t *param_types = READP(cp->param_types);
    while (num_params--) {
        if (p > maxend)
            goto error;
        uint_fast8_t t = READP(*param_types);
        param_types++;
        switch (t) {
        case PT_uint32:
        case PT_int32:
        case PT_uint16:
        case PT_int16:
        case PT_byte:
            *args++ = parse_int(&p);
            break;
        case PT_buffer: {
            uint_fast8_t len = *p++;
            if (p + len > maxend)
                goto error;
            *args++ = len;
            *args++ = command_encode_ptr(p);
            p += len;
            break;
        }
        default:
            goto error;
        }
    }
    return p;
error:
    shutdown("Command parser error");
}

// Encode a message
static uint_fast8_t
command_encodef(uint8_t *buf, const struct command_encoder *ce, va_list args)
{
    uint_fast8_t max_size = READP(ce->max_size);
    if (max_size <= MESSAGE_MIN)
        // Ack/Nak message
        return max_size;
    uint8_t *p = &buf[MESSAGE_HEADER_SIZE];
    uint8_t *maxend = &p[max_size - MESSAGE_MIN];
    uint_fast8_t num_params = READP(ce->num_params);
    const uint8_t *param_types = READP(ce->param_types);
    p = encode_msgid(p, READP(ce->encoded_msgid));
    while (num_params--) {
        if (p > maxend)
            goto error;
        uint_fast8_t t = READP(*param_types);
        param_types++;
        uint32_t v;
        switch (t) {
        case PT_uint32:
        case PT_int32:
        case PT_uint16:
        case PT_int16:
        case PT_byte:
            if (sizeof(v) > sizeof(int) && t >= PT_uint16)
                if (t == PT_int16)
                    v = (int32_t)va_arg(args, int);
                else
                    v = va_arg(args, unsigned int);
            else
                v = va_arg(args, uint32_t);
            p = encode_int(p, v);
            break;
        case PT_string: {
            uint8_t *s = va_arg(args, uint8_t*), *lenp = p++;
            while (*s && p<maxend)
                *p++ = *s++;
            *lenp = p-lenp-1;
            break;
        }
        case PT_progmem_buffer:
        case PT_buffer: {
            v = va_arg(args, int);
            if (v > maxend-p)
                v = maxend-p;
            *p++ = v;
            uint8_t *s = va_arg(args, uint8_t*);
            if (t == PT_progmem_buffer)
                memcpy_P(p, s, v);
            else
                memcpy(p, s, v);
            p += v;
            break;
        }
        default:
            goto error;
        }
    }
    return p - buf + MESSAGE_TRAILER_SIZE;
error:
    shutdown("Message encode error");
}

// Add header and trailer bytes to a message block
static void
command_add_frame(uint8_t *buf, uint_fast8_t msglen)
{
    buf[MESSAGE_POS_LEN] = msglen;
    buf[MESSAGE_POS_SEQ] = next_sequence;
    uint16_t crc = crc16_ccitt(buf, msglen - MESSAGE_TRAILER_SIZE);
    buf[msglen - MESSAGE_TRAILER_CRC + 0] = crc >> 8;
    buf[msglen - MESSAGE_TRAILER_CRC + 1] = crc;
    buf[msglen - MESSAGE_TRAILER_SYNC] = MESSAGE_SYNC;
}

// Encode a message and then add a message block frame around it
uint_fast8_t
command_encode_and_frame(uint8_t *buf, const struct command_encoder *ce
                         , va_list args)
{
    uint_fast8_t msglen = command_encodef(buf, ce, args);
    command_add_frame(buf, msglen);
    return msglen;
}

static uint8_t in_sendf;

// Encode and transmit a "response" message
void
command_sendf(const struct command_encoder *ce, ...)
{
    if (readb(&in_sendf))
        // This sendf call was made from an irq handler while the main
        // code was already in sendf - just drop this sendf request.
        return;
    writeb(&in_sendf, 1);

    va_list args;
    va_start(args, ce);
    console_sendf(ce, args);
    va_end(args);

    writeb(&in_sendf, 0);
}

void
sendf_shutdown(void)
{
    writeb(&in_sendf, 0);
}
DECL_SHUTDOWN(sendf_shutdown);


/****************************************************************
 * Command routing
 ****************************************************************/

// Find the command handler associated with a command
static const struct command_parser *
command_lookup_parser(uint_fast16_t cmdid)
{
    if (!cmdid || cmdid >= READP(command_index_size))
        shutdown("Invalid command");
    return &command_index[cmdid];
}

// Empty message (for ack/nak transmission)
const struct command_encoder encode_acknak PROGMEM = {
    .max_size = MESSAGE_MIN,
};

enum { CF_NEED_SYNC=1<<0, CF_NEED_VALID=1<<1 };

// Find the next complete message block
int_fast8_t
command_find_block(uint8_t *buf, uint_fast8_t buf_len, uint_fast8_t *pop_count)
{
    static uint8_t sync_state;
    if (buf_len && sync_state & CF_NEED_SYNC)
        goto need_sync;
    if (buf_len < MESSAGE_MIN)
        goto need_more_data;
    uint_fast8_t msglen = buf[MESSAGE_POS_LEN];
    if (msglen < MESSAGE_MIN || msglen > MESSAGE_MAX)
        goto error;
    uint_fast8_t msgseq = buf[MESSAGE_POS_SEQ];
    if ((msgseq & ~MESSAGE_SEQ_MASK) != MESSAGE_DEST)
        goto error;
    if (buf_len < msglen)
        goto need_more_data;
    if (buf[msglen-MESSAGE_TRAILER_SYNC] != MESSAGE_SYNC)
        goto error;
    uint16_t msgcrc = ((buf[msglen-MESSAGE_TRAILER_CRC] << 8)
                       | buf[msglen-MESSAGE_TRAILER_CRC+1]);
    uint16_t crc = crc16_ccitt(buf, msglen-MESSAGE_TRAILER_SIZE);
    if (crc != msgcrc)
        goto error;
    sync_state &= ~CF_NEED_VALID;
    *pop_count = msglen;
    // Check sequence number
    if (msgseq != next_sequence) {
        // Lost message - discard messages until it is retransmitted
        goto nak;
    }
    next_sequence = ((msgseq + 1) & MESSAGE_SEQ_MASK) | MESSAGE_DEST;
    return 1;

need_more_data:
    *pop_count = 0;
    return 0;
error:
    if (buf[0] == MESSAGE_SYNC) {
        // Ignore (do not nak) leading SYNC bytes
        *pop_count = 1;
        return -1;
    }
    sync_state |= CF_NEED_SYNC;
need_sync: ;
    // Discard bytes until next SYNC found
    uint8_t *next_sync = memchr(buf, MESSAGE_SYNC, buf_len);
    if (next_sync) {
        sync_state &= ~CF_NEED_SYNC;
        *pop_count = next_sync - buf + 1;
    } else {
        *pop_count = buf_len;
    }
    if (sync_state & CF_NEED_VALID)
        return -1;
    sync_state |= CF_NEED_VALID;
nak:
    command_sendf(&encode_acknak);
    return -1;
}

// Dispatch all the commands found in a message block
void
command_dispatch(uint8_t *buf, uint_fast8_t msglen)
{
    uint8_t *p = &buf[MESSAGE_HEADER_SIZE];
    uint8_t *msgend = &buf[msglen-MESSAGE_TRAILER_SIZE];
    while (p < msgend) {
        uint_fast16_t cmdid = command_parse_msgid(&p);
        const struct command_parser *cp = command_lookup_parser(cmdid);
        uint32_t args[READP(cp->num_args)];
        p = command_parsef(p, msgend, cp, args);
        if (sched_is_shutdown() && !(READP(cp->flags) & HF_IN_SHUTDOWN)) {
            sched_report_shutdown();
            continue;
        }
        irq_poll();
        void (*func)(uint32_t*) = READP(cp->func);
        func(args);
    }
}

// Send an ack message to the host (notifying that it can send more data)
void
command_send_ack(void)
{
    command_sendf(&encode_acknak);
}

// Find a message block and then dispatch all the commands in it
int_fast8_t
command_find_and_dispatch(uint8_t *buf, uint_fast8_t buf_len
                          , uint_fast8_t *pop_count)
{
    int_fast8_t ret = command_find_block(buf, buf_len, pop_count);
    if (ret > 0) {
        command_dispatch(buf, *pop_count);
        command_send_ack();
    }
    return ret;
}
