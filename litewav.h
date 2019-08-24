/*
LiteWav
Public domain WAV file loading library
Created by Javier San Juan Cervera
No warranty implied. Use as you wish and at your own risk
*/

#ifndef LITE_WAV_H
#define LITE_WAV_H

#ifdef __cplusplus
extern "C" {
#endif

void* lwav_load(const char* filename);
void* lwav_loadmem(const char* mem);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LITE_WAV_H */



/* IMPLEMENTATION */



#ifdef LITE_WAV_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  char chunk_id[4];
  int riff_chunk_size;
  char format[4];
  char subchunk_id[4];
  int fmt_chunk_size;
  unsigned short audio_format;
  unsigned short num_channels;
  int sample_rate;
  int byte_rate;
  unsigned short block_align;
  unsigned short bits_per_sample;
  unsigned short extra_params_size;
} lwav_header_t;

void* lwav_load(const char* filename) 
{
  FILE* fhandle;
  long size;
  char* buffer;
  void* wav;

  /* read file */
  fhandle = fopen(filename, "rb");
  if (!fhandle) return 0;
  fseek(fhandle, 0,, SEEK_END);
  size = ftell(fhandle);
  fseek(fhandle, 0, SEEK_SET);
  buffer = (char*)malloc(size);
  fread(buffer, size, 1, fhandle);
  fclose(fhandle);

  /* load wav */
  wav = lwav_loadmem(buffer);
  free(buffer);

  return wav;
}

void* lwav_loadmem(const char* mem)
{
  size_t offset;

  /* read header */
  fread();

  offset = 0;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LITE_WAV_IMPLEMENTATION */