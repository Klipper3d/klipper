#ifndef __TRSYNC_H
#define __TRSYNC_H

#include <stdint.h> // uint16_t

struct trsync_signal;
typedef void (*trsync_callback_t)(struct trsync_signal *tss, uint8_t reason);

struct trsync_signal {
    struct trsync_signal *next;
    trsync_callback_t func;
};

struct trsync *trsync_oid_lookup(uint8_t oid);
void trsync_do_trigger(struct trsync *ts, uint8_t reason);
void trsync_add_signal(struct trsync *ts, struct trsync_signal *tss
                       , trsync_callback_t func);

#endif // trsync.h
