#ifndef MEMPOOL_H_
#define MEMPOOL_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct pool
{
  uint8_t * next;
  uint8_t * end;
} POOL;

POOL * pool_create( void * location, size_t size );
void   pool_destroy( POOL *p );
size_t pool_available( POOL *p );
void * pool_alloc( POOL *p, size_t size );

#endif // MEMPOOL_H_
