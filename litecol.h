/*
LiteCol
Public domain collision library
Created by Javier San Juan Cervera
No warranty implied. Use as you wish and at your own risk
*/

#ifndef LITE_COL_H
#define LITE_COL_H

#ifdef __cplusplus
extern "C" {
#endif


int lcol_boxbox(float minx1, float miny1, float minz1, float maxx1, float maxy1, float maxz1, float minx2, float miny2, float minz2, float maxx2, float maxy2, float maxz2);
int lcol_boxsphere(float minx1, float miny1, float minz1, float maxx1, float maxy1, float maxz1, float sx, float sy, float sz, float sqrad);
int lcol_spheresphere(float sx1, float sy1, float sz1, float sqrad1, float sx2, float sy2, float sz2, float sqrad2);
int lcol_pointinbox(float x, float y, float z, float minx, float miny, float minz, float maxx, float maxy, float maxz);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LITE_COL_H */




/* IMPLEMENTATION */




#ifdef LITE_COL_IMPLEMENTATION

#define _min(A, B)       (A < B ? A : B)
#define _max(A, B)       (A > B ? A : B)
#define _clamp(A, B, C)  _min(_max(A, B), C)
#define _inrange(V, A, B) (V > A && V < B)

#ifdef __cplusplus
extern "C" {
#endif


int lcol_boxbox(float minx1, float miny1, float minz1, float maxx1, float maxy1, float maxz1, float minx2, float miny2, float minz2, float maxx2, float maxy2, float maxz2) {
  if (lcol_pointinbox(minx1, miny1, minz1, minx2, miny2, minz2, maxx2, maxy2, maxz2)
      || lcol_pointinbox(maxx1, maxy1, maxz1, minx2, miny2, minz2, maxx2, maxy2, maxz2)
      || lcol_pointinbox(minx2, miny2, minz2, minx1, miny1, minz1, maxx1, maxy1, maxz1)
      || lcol_pointinbox(maxx2, maxy2, maxz2, minx1, miny1, minz1, maxx1, maxy1, maxz1)) {
    return 1;
  } else {
    return 0;
  }
}


int lcol_boxsphere(float minx, float miny, float minz, float maxx, float maxy, float maxz, float sx, float sy, float sz, float sqrad) {
  /* check if sphere center is in box */
  if (lcol_pointinbox(sx, sy, sz, minx, miny, minz, maxx, maxy, maxz)) {
    return 1;
  } else {
    float vx, vy, vz;
    float sqlength;

    /* find closest point on the surface of the box */
    vx = _clamp(sx, minx, maxx);
    vy = _clamp(sy, miny, maxy);
    vz = _clamp(sz, minz, maxz);

    /* subtract sphere center to get director vector */
    vx -= sx;
    vy -= sy;
    vz -= sz;

    /* get squared length */
    sqlength = vx*vx + vy*vy + vz*vz;

    return sqlength < sqrad;
  }
}


int lcol_spheresphere(float sx1, float sy1, float sz1, float sqrad1, float sx2, float sy2, float sz2, float sqrad2) {
  float vx, vy, vz;
  float sqlength;

  /* subtract sphere center to get director vector */
  vx = sx1 - sx2;
  vy = sy1 - sy2;
  vz = sz1 - sz2;

  /* get squared length */
  sqlength = vx*vx + vy*vy + vz*vz;

  return sqlength < sqrad1 + sqrad2;
}


int lcol_pointinbox(float x, float y, float z, float minx, float miny, float minz, float maxx, float maxy, float maxz) {
  return _inrange(x, minx, maxx) && _inrange(y, miny, maxy) && _inrange(z, minz, maxz);
}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LITE_COL_IMPLEMENTATION */
