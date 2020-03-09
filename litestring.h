/*
LiteString
Public domain string library
Created by Javier San Juan Cervera
No warranty implied. Use as you wish and at your own risk
*/

#ifndef LITE_STRING_H
#define LITE_STRING_H

#ifdef __cplusplus
extern "C"
{
#endif

const char* lstr_alloc(const char* s);
char* lstr_allocempty(size_t n);
const char* lstr_get(const char* s);

#ifdef __cplusplus
}
#endif

#endif /* LITE_STRING_H */

#ifdef LITE_STRING_IMPLEMENTATION

#include "litemem.h"
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

const char* lstr_alloc(const char* s)
{
  char* string = (char*)_lmem_alloc((strlen(s) + 1) * sizeof(char), NULL);
  strcpy(string, s);
  return string;
}

char* lstr_allocempty(size_t n)
{
  return (char*)_mem_alloc((n + 1) * sizeof(char), NULL);
}

const char* lstr_get(const char* s)
{
  return (const char*)lmem_autorelease((char*)lstr_alloc(s));
}

#ifdef __cplusplus
}
#endif

#endif /* LITE_STRING_IMPLEMENTATION */
