// Code for parsing incoming commands and encoding outgoing messages
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdarg.h> // va_start
#include <string.h> // memcpy
#include "board/io.h" // readb
#include "board/irq.h" // irq_poll
#include "board/misc.h" // crc16_ccitt
#include "board/pgm.h" // READP
#include "command.h" // output_P
#include "sched.h" // DECL_TASK

#define MESSAGE_MIN 5
#define MESSAGE_MAX 64
#define MESSAGE_HEADER_SIZE  2
#define MESSAGE_TRAILER_SIZE 3
#define MESSAGE_POS_LEN 0
#define MESSAGE_POS_SEQ 1
#define MESSAGE_TRAILER_CRC  3
#define MESSAGE_TRAILER_SYNC 1
#define MESSAGE_PAYLOAD_MAX (MESSAGE_MAX - MESSAGE_MIN)
#define MESSAGE_SEQ_MASK 0x0f
#define MESSAGE_DEST 0x10
#define MESSAGE_SYNC 0x7E

static uint8_t next_sequence = MESSAGE_DEST;


/****************************************************************
 * Binary message parsing
 ****************************************************************/

// Encode an integer as a variable length quantity (vlq)
static char *
encode_int(char *p, uint32_t v)
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
parse_int(char **pp)
{
    char *p = *pp;
    uint8_t c = *p++;
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

// Parse an incoming command into 'args'
static char *
parsef(char *p, char *maxend, const struct command_parser *cp, uint32_t *args)
{
    if (sched_is_shutdown() && !(READP(cp->flags) & HF_IN_SHUTDOWN)) {
        sched_report_shutdown();
        return NULL;
    }
    uint8_t num_params = READP(cp->num_params);
    const uint8_t *param_types = READP(cp->param_types);
    while (num_params--) {
        if (p > maxend)
            goto error;
        uint8_t t = READP(*param_types);
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
            uint8_t len = *p++;
            if (p + len > maxend)
                goto error;
            *args++ = len;
            *args++ = (size_t)p;
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

static uint8_t in_sendf;

// Encode a message and transmit it
void
_sendf(const struct command_encoder *ce, ...)
{
    if (readb(&in_sendf))
        // This sendf call was made from an irq handler while the main
        // code was already in sendf - just drop this sendf request.
        return;
    writeb(&in_sendf, 1);

    uint8_t max_size = READP(ce->max_size);
    char *buf = console_get_output(max_size + MESSAGE_MIN);
    if (!buf)
        goto done;
    char *p = &buf[MESSAGE_HEADER_SIZE];
    if (max_size) {
        char *maxend = &p[max_size];
        va_list args;
        va_start(args, ce);
        uint8_t num_params = READP(ce->num_params);
        const uint8_t *param_types = READP(ce->param_types);
        *p++ = READP(ce->msg_id);
        while (num_params--) {
            if (p > maxend)
                goto error;
            uint8_t t = READP(*param_types);
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
                char *s = va_arg(args, char*), *lenp = p++;
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
                char *s = va_arg(args, char*);
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
        va_end(args);
    }

    // Send message to serial port
    uint8_t msglen = p+MESSAGE_TRAILER_SIZE - buf;
    buf[MESSAGE_POS_LEN] = msglen;
    buf[MESSAGE_POS_SEQ] = next_sequence;
    uint16_t crc = crc16_ccitt(buf, p-buf);
    *p++ = crc>>8;
    *p++ = crc;
    *p++ = MESSAGE_SYNC;
    console_push_output(msglen);
done:
    writeb(&in_sendf, 0);
    return;
error:
    shutdown("Message encode error");
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
command_get_handler(uint8_t cmdid)
{
    if (!cmdid || cmdid >= READP(command_index_size))
        shutdown("Invalid command");
    return &command_index[cmdid];
}

enum { CF_NEED_SYNC=1<<0, CF_NEED_VALID=1<<1 };

// Find the next complete message.
static char *
command_get_message(void)
{
    static uint8_t sync_state;
    uint8_t buf_len;
    char *buf = console_get_input(&buf_len);
    if (buf_len && sync_state & CF_NEED_SYNC)
        goto need_sync;
    if (buf_len < MESSAGE_MIN)
        // Not ready to run.
        return NULL;
    uint8_t msglen = buf[MESSAGE_POS_LEN];
    if (msglen < MESSAGE_MIN || msglen > MESSAGE_MAX)
        goto error;
    uint8_t msgseq = buf[MESSAGE_POS_SEQ];
    if ((msgseq & ~MESSAGE_SEQ_MASK) != MESSAGE_DEST)
        goto error;
    if (buf_len < msglen)
        // Need more data
        return NULL;
    if (buf[msglen-MESSAGE_TRAILER_SYNC] != MESSAGE_SYNC)
        goto error;
    uint16_t msgcrc = ((buf[msglen-MESSAGE_TRAILER_CRC] << 8)
                       | (uint8_t)buf[msglen-MESSAGE_TRAILER_CRC+1]);
    uint16_t crc = crc16_ccitt(buf, msglen-MESSAGE_TRAILER_SIZE);
    if (crc != msgcrc)
        goto error;
    sync_state &= ~CF_NEED_VALID;
    // Check sequence number
    if (msgseq != next_sequence) {
        // Lost message - discard messages until it is retransmitted
        console_pop_input(msglen);
        goto nak;
    }
    next_sequence = ((msgseq + 1) & MESSAGE_SEQ_MASK) | MESSAGE_DEST;
    sendf(""); // An empty message with a new sequence number is an ack
    return buf;

error:
    if (buf[0] == MESSAGE_SYNC) {
        // Ignore (do not nak) leading SYNC bytes
        console_pop_input(1);
        return NULL;
    }
    sync_state |= CF_NEED_SYNC;
need_sync: ;
    // Discard bytes until next SYNC found
    char *next_sync = memchr(buf, MESSAGE_SYNC, buf_len);
    if (next_sync) {
        sync_state &= ~CF_NEED_SYNC;
        console_pop_input(next_sync - buf + 1);
    } else {
        console_pop_input(buf_len);
    }
    if (sync_state & CF_NEED_VALID)
        return NULL;
    sync_state |= CF_NEED_VALID;
nak:
    sendf(""); // An empty message with a duplicate sequence number is a nak
    return NULL;
}

// Background task that reads commands from the board serial port
void
command_task(void)
{
    // Process commands.
    char *buf = command_get_message();
    if (!buf)
        return;
    uint8_t msglen = buf[MESSAGE_POS_LEN];
    char *p = &buf[MESSAGE_HEADER_SIZE];
    char *msgend = &buf[msglen-MESSAGE_TRAILER_SIZE];
    while (p < msgend) {
        uint8_t cmdid = *p++;
        const struct command_parser *cp = command_get_handler(cmdid);
        uint32_t args[READP(cp->num_args)];
        p = parsef(p, msgend, cp, args);
        if (!p)
            break;
        irq_poll();
        void (*func)(uint32_t*) = READP(cp->func);
        func(args);
    }
    console_pop_input(msglen);
}
DECL_TASK(command_task);
