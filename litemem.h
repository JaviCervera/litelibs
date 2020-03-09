/*
LiteMem
Public domain memory library with reference counting support
Created by Javier San Juan Cervera
No warranty implied. Use as you wish and at your own risk
*/

#ifndef LITE_MEM_H
#define LITE_MEM_H

#include <stddef.h>

#define lmem_alloc(T, F) (T*)_lmem_alloc(sizeof(T), F)
#define lmem_allocauto(T, F) (T*)lmem_autorelease(_lmem_alloc(sizeof(T), F))
#define lmem_assign(V, E) (lmem_retain(E), lmem_release(V), V = E)

#ifdef __cplusplus
extern "C"
{
#endif

void* _lmem_alloc(size_t size, void* func);
size_t lmem_retain(void* block);
size_t lmem_release(void* block);
void* lmem_autorelease(void* block);
void lmem_doautorelease();

#ifdef __cplusplus
}
#endif

#endif /* LITE_MEM_H */

#ifdef LITE_MEM_IMPLEMENTATION

#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
  size_t count;
  void (* delfunc)(void*);
} lmem_rc_t;

typedef struct
{
  void** blocks;
  size_t numblocks;
} lmem_pool_t;

static lmem_pool_t _lmem_pool = {};

void* _lmem_alloc(size_t size, void* func)
{
  lmem_rc_t* rc = (lmem_rc_t*)calloc(1, sizeof(lmem_rc_t) + size);
  rc->count = 1;
  rc->delfunc = (void (*)(void*))func;
  return rc + 1;
}

size_t lmem_retain(void* block)
{
  if (block)
  {
    lmem_rc_t* rc = (lmem_rc_t*)block - 1;
    return ++rc->count;
  }
  else
  {
    return 0;
  }
}

size_t lmem_release(void* block)
{
  if (block)
  {
    size_t count;
    lmem_rc_t* rc = (lmem_rc_t*)block - 1;
    count = --rc->count;
    if (count == 0)
    {
      if (rc->delfunc) rc->delfunc(block);
      free(rc);
    }
    return count;
  }
  else
  {
    return 0;
  }
}

void* lmem_autorelease(void* block)
{
  _lmem_pool.blocks = (void**)realloc(
    _lmem_pool.blocks,
    ++_lmem_pool.numblocks * sizeof(void*));
  _lmem_pool.blocks[_lmem_pool.numblocks - 1] = block;
  return block;
}

void lmem_doautorelease()
{
  size_t i;
  for (i = 0; i < _lmem_pool.numblocks; ++i)
  {
    lmem_release(_lmem_pool.blocks[i]);
  }
  free(_lmem_pool.blocks);
  _lmem_pool.blocks = NULL;
  _lmem_pool.numblocks = 0;
}

#ifdef __cplusplus
}
#endif

#endif /* LITE_MEM_IMPLEMENTATION */
