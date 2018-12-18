/*
LiteMD2
Public domain MD2 model loader
Can be used with LiteGFX
Created by Javier San Juan Cervera
No warranty implied. Use as you wish and at your own risk
*/

#ifndef LITE_MD2_H
#define LITE_MD2_H

#ifdef LITE_MD2_USE_GFX
#include "litegfx.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern float lmd2_normal_table[][3];

/* header */

typedef struct {
  int ident;                  /* magic number: "IDP2" */
  int version;                /* version: must be 8 */

  int skinwidth;              /* texture width */
  int skinheight;             /* texture height */

  int framesize;              /* size in bytes of a frame */

  int num_skins;              /* number of skins */
  int num_vertices;           /* number of vertices per frame */
  int num_st;                 /* number of texture coordinates */
  int num_tris;               /* number of triangles */
  int num_glcmds;             /* number of opengl commands */
  int num_frames;             /* number of frames */

  int offset_skins;           /* offset skin data */
  int offset_st;              /* offset texture coordinate data */
  int offset_tris;            /* offset triangle data */
  int offset_frames;          /* offset frame data */
  int offset_glcmds;          /* offset OpenGL command data */
  int offset_end;             /* offset end of file */
} lmd2_header_t;

/* texture name */

typedef struct {
  char name[64];              /* texture file name */
} lmd2_skin_t;

/* texture coords */

typedef struct {
  short s;
  short t;
} lmd2_texcoord_t;

/* triangle info */

typedef struct {
  unsigned short vertex[3];   /* vertex indices of the triangle */
  unsigned short st[3];       /* tex. coord. indices */
} lmd2_triangle_t;

/* compressed vertex */
typedef struct {
  unsigned char v[3];           /* position */
  unsigned char normal_index;   /* normal vector index */
} lmd2_vertex_t;

/* model frame */
typedef struct {
  float scale[3];         /* scale factor */
  float translate[3];     /* translation vector */
  char name[16];          /* frame name */
  lmd2_vertex_t *verts;   /* list of frame's vertices */
} lmd2_frame_t;

/* model */

typedef struct {
  lmd2_header_t header;
  lmd2_skin_t* skins;
  lmd2_texcoord_t* texcoords;
  lmd2_triangle_t* triangles;
  lmd2_frame_t* frames;
  int* glcmds;
} lmd2_model_t;

lmd2_model_t* lmd2_load(const char* filename);
void lmd2_free(lmd2_model_t* mdl);

#ifdef LITE_MD2_USE_GFX
lvert_t* lmd2_getvertices(const lmd2_model_t* mdl, int frame);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LITE_MD2_H */




/* IMPLEMENTATION */




#ifdef LITE_MD2_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* normal table */

float lmd2_normal_table[][3] = {
  { -0.525731f,  0.000000f,  0.850651f },
  { -0.442863f,  0.238856f,  0.864188f },
  { -0.295242f,  0.000000f,  0.955423f },
  { -0.309017f,  0.500000f,  0.809017f },
  { -0.162460f,  0.262866f,  0.951056f },
  {  0.000000f,  0.000000f,  1.000000f },
  {  0.000000f,  0.850651f,  0.525731f },
  { -0.147621f,  0.716567f,  0.681718f },
  {  0.147621f,  0.716567f,  0.681718f },
  {  0.000000f,  0.525731f,  0.850651f },
  {  0.309017f,  0.500000f,  0.809017f },
  {  0.525731f,  0.000000f,  0.850651f },
  {  0.295242f,  0.000000f,  0.955423f },
  {  0.442863f,  0.238856f,  0.864188f },
  {  0.162460f,  0.262866f,  0.951056f },
  { -0.681718f,  0.147621f,  0.716567f },
  { -0.809017f,  0.309017f,  0.500000f },
  { -0.587785f,  0.425325f,  0.688191f },
  { -0.850651f,  0.525731f,  0.000000f },
  { -0.864188f,  0.442863f,  0.238856f },
  { -0.716567f,  0.681718f,  0.147621f },
  { -0.688191f,  0.587785f,  0.425325f },
  { -0.500000f,  0.809017f,  0.309017f },
  { -0.238856f,  0.864188f,  0.442863f },
  { -0.425325f,  0.688191f,  0.587785f },
  { -0.716567f,  0.681718f, -0.147621f },
  { -0.500000f,  0.809017f, -0.309017f },
  { -0.525731f,  0.850651f,  0.000000f },
  {  0.000000f,  0.850651f, -0.525731f },
  { -0.238856f,  0.864188f, -0.442863f },
  {  0.000000f,  0.955423f, -0.295242f },
  { -0.262866f,  0.951056f, -0.162460f },
  {  0.000000f,  1.000000f,  0.000000f },
  {  0.000000f,  0.955423f,  0.295242f },
  { -0.262866f,  0.951056f,  0.162460f },
  {  0.238856f,  0.864188f,  0.442863f },
  {  0.262866f,  0.951056f,  0.162460f },
  {  0.500000f,  0.809017f,  0.309017f },
  {  0.238856f,  0.864188f, -0.442863f },
  {  0.262866f,  0.951056f, -0.162460f },
  {  0.500000f,  0.809017f, -0.309017f },
  {  0.850651f,  0.525731f,  0.000000f },
  {  0.716567f,  0.681718f,  0.147621f },
  {  0.716567f,  0.681718f, -0.147621f },
  {  0.525731f,  0.850651f,  0.000000f },
  {  0.425325f,  0.688191f,  0.587785f },
  {  0.864188f,  0.442863f,  0.238856f },
  {  0.688191f,  0.587785f,  0.425325f },
  {  0.809017f,  0.309017f,  0.500000f },
  {  0.681718f,  0.147621f,  0.716567f },
  {  0.587785f,  0.425325f,  0.688191f },
  {  0.955423f,  0.295242f,  0.000000f },
  {  1.000000f,  0.000000f,  0.000000f },
  {  0.951056f,  0.162460f,  0.262866f },
  {  0.850651f, -0.525731f,  0.000000f },
  {  0.955423f, -0.295242f,  0.000000f },
  {  0.864188f, -0.442863f,  0.238856f },
  {  0.951056f, -0.162460f,  0.262866f },
  {  0.809017f, -0.309017f,  0.500000f },
  {  0.681718f, -0.147621f,  0.716567f },
  {  0.850651f,  0.000000f,  0.525731f },
  {  0.864188f,  0.442863f, -0.238856f },
  {  0.809017f,  0.309017f, -0.500000f },
  {  0.951056f,  0.162460f, -0.262866f },
  {  0.525731f,  0.000000f, -0.850651f },
  {  0.681718f,  0.147621f, -0.716567f },
  {  0.681718f, -0.147621f, -0.716567f },
  {  0.850651f,  0.000000f, -0.525731f },
  {  0.809017f, -0.309017f, -0.500000f },
  {  0.864188f, -0.442863f, -0.238856f },
  {  0.951056f, -0.162460f, -0.262866f },
  {  0.147621f,  0.716567f, -0.681718f },
  {  0.309017f,  0.500000f, -0.809017f },
  {  0.425325f,  0.688191f, -0.587785f },
  {  0.442863f,  0.238856f, -0.864188f },
  {  0.587785f,  0.425325f, -0.688191f },
  {  0.688191f,  0.587785f, -0.425325f },
  { -0.147621f,  0.716567f, -0.681718f },
  { -0.309017f,  0.500000f, -0.809017f },
  {  0.000000f,  0.525731f, -0.850651f },
  { -0.525731f,  0.000000f, -0.850651f },
  { -0.442863f,  0.238856f, -0.864188f },
  { -0.295242f,  0.000000f, -0.955423f },
  { -0.162460f,  0.262866f, -0.951056f },
  {  0.000000f,  0.000000f, -1.000000f },
  {  0.295242f,  0.000000f, -0.955423f },
  {  0.162460f,  0.262866f, -0.951056f },
  { -0.442863f, -0.238856f, -0.864188f },
  { -0.309017f, -0.500000f, -0.809017f },
  { -0.162460f, -0.262866f, -0.951056f },
  {  0.000000f, -0.850651f, -0.525731f },
  { -0.147621f, -0.716567f, -0.681718f },
  {  0.147621f, -0.716567f, -0.681718f },
  {  0.000000f, -0.525731f, -0.850651f },
  {  0.309017f, -0.500000f, -0.809017f },
  {  0.442863f, -0.238856f, -0.864188f },
  {  0.162460f, -0.262866f, -0.951056f },
  {  0.238856f, -0.864188f, -0.442863f },
  {  0.500000f, -0.809017f, -0.309017f },
  {  0.425325f, -0.688191f, -0.587785f },
  {  0.716567f, -0.681718f, -0.147621f },
  {  0.688191f, -0.587785f, -0.425325f },
  {  0.587785f, -0.425325f, -0.688191f },
  {  0.000000f, -0.955423f, -0.295242f },
  {  0.000000f, -1.000000f,  0.000000f },
  {  0.262866f, -0.951056f, -0.162460f },
  {  0.000000f, -0.850651f,  0.525731f },
  {  0.000000f, -0.955423f,  0.295242f },
  {  0.238856f, -0.864188f,  0.442863f },
  {  0.262866f, -0.951056f,  0.162460f },
  {  0.500000f, -0.809017f,  0.309017f },
  {  0.716567f, -0.681718f,  0.147621f },
  {  0.525731f, -0.850651f,  0.000000f },
  { -0.238856f, -0.864188f, -0.442863f },
  { -0.500000f, -0.809017f, -0.309017f },
  { -0.262866f, -0.951056f, -0.162460f },
  { -0.850651f, -0.525731f,  0.000000f },
  { -0.716567f, -0.681718f, -0.147621f },
  { -0.716567f, -0.681718f,  0.147621f },
  { -0.525731f, -0.850651f,  0.000000f },
  { -0.500000f, -0.809017f,  0.309017f },
  { -0.238856f, -0.864188f,  0.442863f },
  { -0.262866f, -0.951056f,  0.162460f },
  { -0.864188f, -0.442863f,  0.238856f },
  { -0.809017f, -0.309017f,  0.500000f },
  { -0.688191f, -0.587785f,  0.425325f },
  { -0.681718f, -0.147621f,  0.716567f },
  { -0.442863f, -0.238856f,  0.864188f },
  { -0.587785f, -0.425325f,  0.688191f },
  { -0.309017f, -0.500000f,  0.809017f },
  { -0.147621f, -0.716567f,  0.681718f },
  { -0.425325f, -0.688191f,  0.587785f },
  { -0.162460f, -0.262866f,  0.951056f },
  {  0.442863f, -0.238856f,  0.864188f },
  {  0.162460f, -0.262866f,  0.951056f },
  {  0.309017f, -0.500000f,  0.809017f },
  {  0.147621f, -0.716567f,  0.681718f },
  {  0.000000f, -0.525731f,  0.850651f },
  {  0.425325f, -0.688191f,  0.587785f },
  {  0.587785f, -0.425325f,  0.688191f },
  {  0.688191f, -0.587785f,  0.425325f },
  { -0.955423f,  0.295242f,  0.000000f },
  { -0.951056f,  0.162460f,  0.262866f },
  { -1.000000f,  0.000000f,  0.000000f },
  { -0.850651f,  0.000000f,  0.525731f },
  { -0.955423f, -0.295242f,  0.000000f },
  { -0.951056f, -0.162460f,  0.262866f },
  { -0.864188f,  0.442863f, -0.238856f },
  { -0.951056f,  0.162460f, -0.262866f },
  { -0.809017f,  0.309017f, -0.500000f },
  { -0.864188f, -0.442863f, -0.238856f },
  { -0.951056f, -0.162460f, -0.262866f },
  { -0.809017f, -0.309017f, -0.500000f },
  { -0.681718f,  0.147621f, -0.716567f },
  { -0.681718f, -0.147621f, -0.716567f },
  { -0.850651f,  0.000000f, -0.525731f },
  { -0.688191f,  0.587785f, -0.425325f },
  { -0.587785f,  0.425325f, -0.688191f },
  { -0.425325f,  0.688191f, -0.587785f },
  { -0.425325f, -0.688191f, -0.587785f },
  { -0.587785f, -0.425325f, -0.688191f },
  { -0.688191f, -0.587785f, -0.425325f }
};

lmd2_model_t* lmd2_load(const char* filename)
{
  lmd2_model_t* mdl;
  FILE *fp;
  int i;

  fp = fopen(filename, "rb");
  if (!fp) return 0;

  /* create model */
  mdl = (lmd2_model_t*) malloc(sizeof(lmd2_model_t));

  /* read header */
  fread(&mdl->header, 1, sizeof(lmd2_header_t), fp);

  if ((mdl->header.ident != 844121161 ) || (mdl->header.version != 8))
  {
    free(mdl);
    fclose(fp);
    return 0;
  }

  /* memory allocations */
  mdl->skins = (lmd2_skin_t*) malloc(sizeof(lmd2_skin_t) * mdl->header.num_skins);
  mdl->texcoords = (lmd2_texcoord_t*) malloc(sizeof(lmd2_texcoord_t) * mdl->header.num_st);
  mdl->triangles = (lmd2_triangle_t*) malloc(sizeof(lmd2_triangle_t) * mdl->header.num_tris);
  mdl->frames = (lmd2_frame_t*) malloc(sizeof(lmd2_frame_t) * mdl->header.num_frames);
  mdl->glcmds = (int*) malloc(sizeof(int) * mdl->header.num_glcmds);

  /* read model data */
  fseek(fp, mdl->header.offset_skins, SEEK_SET);
  fread(mdl->skins, sizeof(lmd2_skin_t), mdl->header.num_skins, fp);
  fseek(fp, mdl->header.offset_st, SEEK_SET);
  fread(mdl->texcoords, sizeof(lmd2_texcoord_t), mdl->header.num_st, fp);
  fseek(fp, mdl->header.offset_tris, SEEK_SET);
  fread(mdl->triangles, sizeof(lmd2_triangle_t), mdl->header.num_tris, fp);
  fseek(fp, mdl->header.offset_glcmds, SEEK_SET);
  fread(mdl->glcmds, sizeof(int), mdl->header.num_glcmds, fp);

  /* read frames */
  fseek(fp, mdl->header.offset_frames, SEEK_SET);
  for (i = 0; i < mdl->header.num_frames; ++i)
  {
    /* memory allocation for vertices of this frame */
    mdl->frames[i].verts = (lmd2_vertex_t*) malloc(sizeof(lmd2_vertex_t) * mdl->header.num_vertices);

    /* read frame data */
    fread(mdl->frames[i].scale, sizeof(float), 3, fp);
    fread(mdl->frames[i].translate, sizeof(float), 3, fp);
    fread(mdl->frames[i].name, sizeof(char), 16, fp);
    fread(mdl->frames[i].verts, sizeof(lmd2_vertex_t), mdl->header.num_vertices, fp);
  }

  fclose (fp);
  return mdl;
}

void lmd2_free(lmd2_model_t* mdl)
{
  free(mdl->skins);
  free(mdl->texcoords);
  free(mdl->triangles);
  free(mdl->frames);
  free(mdl->glcmds);
  free(mdl);
}

#ifdef LITE_MD2_USE_GFX

lvert_t* lmd2_getvertices(const lmd2_model_t* mdl, int frame)
{
  int i, j;
  float v[3];
  float n[3];
  float t[2];
  lmd2_frame_t *pframe;
  lmd2_vertex_t *pvert;
  lvert_t* verts;

  /* check if n is in a valid range */
  if ((frame < 0) || (frame >= mdl->header.num_frames)) return 0;

  /* get frame */
  pframe = &mdl->frames[frame];

  /* create buffer */
  verts = (lvert_t*) malloc(sizeof(lvert_t) * mdl->header.num_tris * 3);

  /* get each triangle */
  for (i = 0; i < mdl->header.num_tris; ++i)
  {
    /* get each vertex */
    for (j = 0; j < 3; ++j)
    {
      /* get vertex */
      pvert = &pframe->verts[mdl->triangles[i].vertex[j]];

      /* position */
      v[0] = (pvert->v[0] * pframe->scale[0]) + pframe->translate[0];
      v[1] = (pvert->v[1] * pframe->scale[1]) + pframe->translate[1];
      v[2] = (pvert->v[2] * pframe->scale[2]) + pframe->translate[2];

      /* normal vector */
      n[0] = lmd2_normal_table[pvert->normal_index][0];
      n[1] = lmd2_normal_table[pvert->normal_index][1];
      n[2] = lmd2_normal_table[pvert->normal_index][2];

      /* compute texture coordinates */
      t[0] = (float)mdl->texcoords[mdl->triangles[i].st[j]].s / mdl->header.skinwidth;
      t[1] = (float)mdl->texcoords[mdl->triangles[i].st[j]].t / mdl->header.skinheight;

      /* set vertex */
      verts[i*3 + j] = lvert(v[0], v[2], v[1], n[0], n[2], n[1], t[0], t[1], 1, 1, 1, 1);
    }
  }

  return verts;
}

#endif /* LITE_MD2_USE_GFX */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LITE_MD2_IMPLEMENTATION */
