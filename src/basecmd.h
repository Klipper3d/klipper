#ifndef __BASECMD_H
#define __BASECMD_H

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

struct move_node {
    struct move_node *next;
};
struct move_queue_head {
    struct move_node *first, *last;
};

void *alloc_chunk(size_t size);
void move_free(void *m);
void *move_alloc(void);
int move_queue_empty(struct move_queue_head *mh);
struct move_node *move_queue_first(struct move_queue_head *mh);
int move_queue_push(struct move_node *m, struct move_queue_head *mh);
struct move_node *move_queue_pop(struct move_queue_head *mh);
void move_queue_clear(struct move_queue_head *mh);
void move_queue_setup(struct move_queue_head *mh, int size);
void *oid_lookup(uint8_t oid, void *type);
void *oid_alloc(uint8_t oid, void *type, uint16_t size);
void *oid_next(uint8_t *i, void *type);
void stats_update(uint32_t start, uint32_t cur);
void config_reset(uint32_t *args);

#define foreach_oid(pos,data,oidtype)                   \
    for (pos=-1; (data=oid_next(&pos, oidtype)); )

#endif // basecmd.h
