#ifndef __BASECMD_H
#define __BASECMD_H

#include <stdint.h> // uint8_t

void move_free(void *m);
void *move_alloc(void);
void move_request_size(int size);
void *lookup_oid(uint8_t oid, void *type);
void *alloc_oid(uint8_t oid, void *type, uint16_t size);
void *next_oid(uint8_t *i, void *type);

#define foreach_oid(pos,data,oidtype)                   \
    for (pos=-1; (data=next_oid(&pos, oidtype)); )

#endif // basecmd.h
