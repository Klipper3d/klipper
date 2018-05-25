#include "mempool.h"

POOL * pool_create( void * location, size_t size ) {
    //POOL * p = (POOL*)malloc( size + sizeof(POOL) );
    POOL * p = (POOL*)location;
    p->next = (uint8_t*)&p[1];
    size = ((size - sizeof(POOL)) & ~3);
    p->end = p->next + size;
    return p;
}

void pool_destroy( POOL *p ) {
    free(p);
}

size_t pool_available( POOL *p ) {
    return p->end - p->next;
}

void * pool_alloc( POOL *p, size_t size ) {
    if( pool_available(p) < size ) return NULL;
    size = (size + 3) & ~3; // Round to next 32bit alignment
    void *mem = (void*)p->next;
    p->next += size;
    return mem;
}
