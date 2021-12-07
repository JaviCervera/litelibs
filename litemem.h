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
#define lmem_assign(V, E) (_lmem_assign((void**)&V, E), V)

#ifdef __cplusplus
extern "C" {
#endif


void* _lmem_alloc(size_t size, void* func);
size_t lmem_retain(void* block);
size_t lmem_release(void* block);
size_t lmem_count(void* block);
void* lmem_autorelease(void* block);
void lmem_doautorelease();
void _lmem_assign(void** varptr, void* data);


char* lstr_alloc(const char* s);
char* lstr_allocempty(size_t n);
char* lstr_get(const char* s);


#ifdef __cplusplus
}
#endif

#endif /* LITE_MEM_H */




/* IMPLEMENTATION */




#ifdef LITE_MEM_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
  size_t count;
  void (* delfunc)(void*);
} lmem_rc_t;


typedef struct {
  void** blocks;
  size_t numblocks;
} lmem_pool_t;


static lmem_pool_t _lmem_pool = {};


void* _lmem_alloc(size_t size, void* func) {
  lmem_rc_t* rc = (lmem_rc_t*)calloc(1, sizeof(lmem_rc_t) + size);
  rc->count = 1;
  rc->delfunc = (void (*)(void*))func;
  return rc + 1;
}


size_t lmem_retain(void* block) {
  if (block) {
    lmem_rc_t* rc = (lmem_rc_t*)block - 1;
    return ++rc->count;
  } else {
    return 0;
  }
}


size_t lmem_release(void* block) {
  if (block) {
    size_t count;
    lmem_rc_t* rc = (lmem_rc_t*)block - 1;
    count = --rc->count;
    if (count == 0) {
      if (rc->delfunc) rc->delfunc(block);
      free(rc);
    }
    return count;
  } else {
    return 0;
  }
}


size_t lmem_count(void* block) {
  if (block) {
    return ((lmem_rc_t*)block - 1)->count;
  } else {
    return 0;
  }
}


void* lmem_autorelease(void* block) {
  _lmem_pool.blocks = (void**)realloc(
    _lmem_pool.blocks,
    ++_lmem_pool.numblocks * sizeof(void*));
  _lmem_pool.blocks[_lmem_pool.numblocks - 1] = block;
  return block;
}


void lmem_doautorelease() {
  size_t i;
  for (i = 0; i < _lmem_pool.numblocks; ++i) {
    lmem_release(_lmem_pool.blocks[i]);
  }
  free(_lmem_pool.blocks);
  _lmem_pool.blocks = NULL;
  _lmem_pool.numblocks = 0;
}


void _lmem_assign(void** varptr, void* data) {
  lmem_retain(data);
  lmem_release(*varptr);
  memcpy(varptr, &data, sizeof(void*));
}


char* lstr_alloc(const char* s) {
  char* string = (char*)_lmem_alloc((strlen(s) + 1) * sizeof(char), NULL);
  strcpy(string, s);
  return string;
}


char* lstr_allocempty(size_t n) {
  return (char*)_lmem_alloc((n + 1) * sizeof(char), NULL);
}


char* lstr_get(const char* s) {
  return (char*)lmem_autorelease(lstr_alloc(s));
}


#ifdef __cplusplus
}
#endif

#endif /* LITE_MEM_IMPLEMENTATION */
