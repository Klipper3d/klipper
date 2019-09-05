#ifndef __BASECMD_H
#define __BASECMD_H

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

void *alloc_chunk(size_t size);
void move_free(void *m);
void *move_alloc(void);
void move_request_size(int size);
void *oid_lookup(uint8_t oid, void *type);
void *oid_alloc(uint8_t oid, void *type, uint16_t size);
void *oid_next(uint8_t *i, void *type);
void stats_update(uint32_t start, uint32_t cur);
void config_reset(uint32_t *args);

#define foreach_oid(pos,data,oidtype)                   \
    for (pos=-1; (data=oid_next(&pos, oidtype)); )

#endif // basecmd.h
