#ifndef __BASECMD_H
#define __BASECMD_H

#include <stdint.h> // uint8_t

struct move {
    uint32_t interval;
    int16_t add;
    uint16_t count;
    struct move *next;
    uint8_t flags;
};

void move_free(struct move *m);
struct move *move_alloc(void);
void *lookup_oid(uint8_t oid, void *type);
void *alloc_oid(uint8_t oid, void *type, uint16_t size);
void *next_oid(uint8_t *i, void *type);

#define foreach_oid(pos,data,oidtype)                   \
    for (pos=-1; (data=next_oid(&pos, oidtype)); )

#endif // basecmd.h
