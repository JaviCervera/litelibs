/*
LiteB3D
Public domain B3D model loader
It does not support animations
Can be used with LiteGFX
Created by Javier San Juan Cervera
No warranty implied. Use as you wish and at your own risk
*/

#ifndef LITE_B3D_H
#define LITE_B3D_H

#ifdef LITE_B3D_USE_GFX
#include "litegfx.h"
#endif
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* header */

typedef struct {
  char tag[4];          /* 4 char tag, i.e. BB3D */
  int length;           /* length of the chunk (not counting this header) */
} lb3d_chunk_header_t;

/* texture */

typedef struct {
  char file[FILENAME_MAX];
  int flags;
  int blend;
  float x_pos;
  float y_pos;
  float x_scale;
  float y_scale;
  float rotation;
} lb3d_tex_t;

/* brush */

typedef struct {
  char name[FILENAME_MAX];
  float r, g, b, a;
  float shininess;
  int blend;
  int fx;
  int texture_id[8];
} lb3d_brush_t;

/* vertex */

typedef struct {
  float x, y, z;
  float nx, ny, nz;
  float r, g, b, a;
  float tex_coords[8][4];
} lb3d_vert_t;

/* triangle */

typedef struct {
  int vertex_id[3];
} lb3d_tri_t;

/* mesh */

typedef struct {
  int           brush_id;
  int           num_sets;
  lb3d_vert_t*  vertices;
  lb3d_tri_t*   triangles[];
} lb3d_mesh_t;

/* node */

typedef struct {
  char          name[1024];
  float         position[3];
  float         scale[3];
  float         rotation[4];
  lb3d_mesh_t*  mesh;
} lb3d_node_t;

/* b3d */

typedef struct {
  int           version;
  int           num_texs;
  int           num_brushes;
  int           num_nodes;
  lb3d_tex_t*   texs;
  lb3d_brush_t* brushes;
  lb3d_node_t*  nodes;
} lb3d_t;

lb3d_t* lb3d_load(const char* filename);
void lb3d_free(lb3d_t* b3d);

#ifdef LITE_B3D_USE_GFX
//lvert_t* lb3d_getvertices(const lmd2_model_t* mdl, int frame);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LITE_B3D_H */




/* IMPLEMENTATION */




#ifdef LITE_B3D_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

lb3d_t* lb3d_load(const char* filename)
{
  lb3d_chunk_header_t header;
  lb3d_t* b3d;
  FILE* fp;
  int num_textures; /* number of layers per brush (should be 8 or less) */
  int remaining;
  char c;
  int i;

  /* open file for reading */
  fp = fopen(filename, "rb");
  if (!fp) return 0;

  /* read header and compare tag */
  fread(&header, 1, sizeof(lb3d_chunk_header_t), fp);
  if (strncmp(header.tag, "BB3D", 4) != 0)
  {
    fclose(fp);
    return 0;
  }

  /* create model */
  b3d = (lb3d_t*) malloc(sizeof(lb3d_t));
  memset(b3d, 0, sizeof(lb3d_t));

  /* read version */
  fread(&b3d->version, 1, sizeof(int), fp);
  if (b3d->version != 1)
  {
    free(b3d);
    fclose(fp);
    return 0;
  }

  /* read next chunk header */
  if (!feof(fp)) fread(&header, 1, sizeof(lb3d_chunk_header_t), fp);

  /* read texture chunk */
  if (!feof(fp) && strncmp(header.tag, "TEXS", 4) == 0)
  {
    remaining = header.length;
    while (remaining > 0)
    {
      /* create new texture */
      ++b3d->num_texs;
      b3d->texs = (lb3d_tex_t*)realloc(b3d->texs, b3d->num_texs * sizeof(lb3d_tex_t));

      /* read filename */
      i = 0;
      fread(&c, 1, sizeof(char), fp);
      --remaining;
      while (c != 0)
      {
        b3d->texs[b3d->num_texs-1].file[i] = c;
        ++i;
        fread(&c, 1, sizeof(char), fp);
        --remaining;
      }
      b3d->texs[b3d->num_texs-1].file[i] = 0;

      /* read the rest */
      fread(&b3d->texs[b3d->num_texs-1].flags, 1, sizeof(lb3d_tex_t) - FILENAME_MAX, fp);
      remaining -= sizeof(lb3d_tex_t) - FILENAME_MAX;
    }

    /* read next chunk header */
    if (!feof(fp)) fread(&header, 1, sizeof(lb3d_chunk_header_t), fp);
  }

  /* read brush chunk */
  if (!feof(fp) && strncmp(header.tag, "BRUS", 4) == 0)
  {
    remaining = header.length;

    /* read number of texture layers per brush (should be 8 or less) */
    fread(&num_textures, 1, sizeof(int), fp);
    remaining -= sizeof(int);

    while (remaining > 0)
    {
      /* create new brush */
      ++b3d->num_brushes;
      b3d->brushes = (lb3d_brush_t*)realloc(b3d->brushes, b3d->num_brushes * sizeof(lb3d_brush_t));

      /* read brush name */
      i = 0;
      fread(&c, 1, sizeof(char), fp);
      --remaining;
      while (c != 0)
      {
        b3d->brushes[b3d->num_brushes-1].name[i] = c;
        ++i;
        fread(&c, 1, sizeof(char), fp);
        --remaining;
      }
      b3d->brushes[b3d->num_brushes-1].name[i] = 0;

      /* read the rest */
      fread(&b3d->brushes[b3d->num_brushes-1].r, 1, sizeof(lb3d_brush_t) - FILENAME_MAX - 8*sizeof(int) + num_textures*sizeof(int), fp);
      remaining -= sizeof(lb3d_brush_t) - FILENAME_MAX - 8*sizeof(int) + num_textures*sizeof(int);
    }

    /* read next chunk header */
    if (!feof(fp)) fread(&header, 1, sizeof(lb3d_chunk_header_t), fp);
  }

  /* read the rest of chunks */
  while (!feof(fp))
  {
    if (strncmp(header.tag, "NODE", 4) == 0)
    {
      /* node chunk */
      remaining = header.length;

      /* create new node */
      ++b3d->num_nodes;
      b3d->nodes = (lb3d_node_t*)realloc(b3d->nodes, b3d->num_nodes * sizeof(lb3d_node_t));

      /* read node name */
      i = 0;
      fread(&c, 1, sizeof(char), fp);
      --remaining;
      while (c != 0)
      {
        b3d->nodes[b3d->num_nodes-1].name[i] = c;
        ++i;
        fread(&c, 1, sizeof(char), fp);
        --remaining;
      }
      b3d->nodes[b3d->num_nodes-1].name[i] = 0;

      /* read the rest of common data */
      fread(&b3d->nodes[b3d->num_nodes-1].position[0], 1, sizeof(lb3d_node_t) - sizeof(b3d->nodes[b3d->num_nodes-1].name) - sizeof(b3d->nodes[b3d->num_nodes-1].mesh), fp);
      remaining -= sizeof(lb3d_node_t) - sizeof(b3d->nodes[b3d->num_nodes-1].name) - sizeof(b3d->nodes[b3d->num_nodes-1].mesh);

      /* read mesh if present */
      b3d->nodes[b3d->num_nodes-1].mesh = 0;
      if (remaining > 0)
      {
        fread(&header, 1, sizeof(lb3d_chunk_header_t), fp);
        if (strncmp(header.tag, "MESH", 4) == 0)
        {
          int verts_remaining;
          int verts_flags;
          int verts_tex_coord_sets;
          int verts_tex_coord_set_size;
          int verts_vertex_size;

          remaining = header.length;

          /* allocate mesh */
          b3d->nodes[b3d->num_nodes-1].mesh = (lb3d_mesh_t*)malloc(sizeof(lb3d_mesh_t));
          memset(b3d->nodes[b3d->num_nodes-1].mesh, 0, sizeof(lb3d_mesh_t));

          /* read brush id */
          fread(&b3d->nodes[b3d->num_nodes-1].mesh->brush_id, 1, sizeof(int), fp);
          remaining -= sizeof(int);

          /* read vertices chunk */
          /*fread(&header, 1, sizeof(lb3d_chunk_header_t), fp);
          verts_remaining = header.length;
          fread(&verts_flags, 1, sizeof(int), fp);
          fread(&verts_tex_coord_sets, 1, sizeof(int), fp);
          fread(&verts_tex_coord_set_size, 1, sizeof(int), fp);
          while (verts_remaining)
          {
            b3d->nodes[b3d->num_nodes-1].mesh->vertices = (lb3d_vert_t*)realloc(b3d->nodes[b3d->num_nodes-1].mesh->vertices, sizeof(b3d->nodes[b3d->num_nodes-1].mesh->num_vertices));
          }*/
          fseek(fp, remaining, SEEK_CUR);
        } else {
          /* if not a mesh, rewind */
          fseek(fp, -sizeof(lb3d_chunk_header_t), SEEK_CUR);
        }
      }
    } else {
      /* skip non-node chunks */
      fseek(fp, header.length, SEEK_CUR);
    }

    /* read next chunk header */
    if (!feof(fp)) fread(&header, 1, sizeof(lb3d_chunk_header_t), fp);
  }

  //printf("%c%c%c%c\n", header.tag[0], header.tag[1], header.tag[2], header.tag[3]);

  fclose(fp);
  return b3d;
}

void lb3d_free(lb3d_t* b3d)
{
  free(b3d);
}

#ifdef LITE_MD2_USE_GFX

/*
lvert_t* lmd2_getvertices(const lmd2_model_t* mdl, int frame)
{
  int i, j;
  float v[3];
  float n[3];
  float t[2];
  lmd2_frame_t *pframe;
  lmd2_vertex_t *pvert;
  lvert_t* verts;

  /* check if n is in a valid range * /
  if ((frame < 0) || (frame >= mdl->header.num_frames)) return 0;

  /* get frame * /
  pframe = &mdl->frames[frame];

  /* create buffer * /
  verts = (lvert_t*) malloc(sizeof(lvert_t) * mdl->header.num_tris * 3);

  /* get each triangle * /
  for (i = 0; i < mdl->header.num_tris; ++i)
  {
    /* get each vertex * /
    for (j = 0; j < 3; ++j)
    {
      /* get vertex * /
      pvert = &pframe->verts[mdl->triangles[i].vertex[j]];

      /* position * /
      v[0] = (pvert->v[0] * pframe->scale[0]) + pframe->translate[0];
      v[1] = (pvert->v[1] * pframe->scale[1]) + pframe->translate[1];
      v[2] = (pvert->v[2] * pframe->scale[2]) + pframe->translate[2];

      /* normal vector * /
      n[0] = lmd2_normal_table[pvert->normal_index][0];
      n[1] = lmd2_normal_table[pvert->normal_index][1];
      n[2] = lmd2_normal_table[pvert->normal_index][2];

      /* compute texture coordinates * /
      t[0] = (float)mdl->texcoords[mdl->triangles[i].st[j]].s / mdl->header.skinwidth;
      t[1] = (float)mdl->texcoords[mdl->triangles[i].st[j]].t / mdl->header.skinheight;

      /* set vertex * /
      verts[i*3 + j] = lvert(v[0], v[2], v[1], n[0], n[2], n[1], t[0], t[1], 1, 1, 1, 1);
    }
  }

  return verts;
}
*/

#endif /* LITE_B3D_USE_GFX */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LITE_B3D_IMPLEMENTATION */
