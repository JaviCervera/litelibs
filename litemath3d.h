/*
LiteMath3D
Public domain 3D math library
Created by Javier San Juan Cervera
No warranty implied. Use as you wish and at your own risk
*/

#ifndef LITE_MATH3D_H
#define LITE_MATH3D_H


typedef struct {
  float m[16];
} lmat4_t;


typedef struct {
  float w, x, y, z;
} lquat_t;


typedef struct {
  float x, y, z;
} lvec3_t;


#ifdef __cplusplus
extern "C" {
#endif


/* misc */


#define lm_deg2rad(A) (A * 0.0174532925f)
#define lm_rad2deg(A) (A * 57.2957795f)


/* vec3 */


lvec3_t lvec3(float x, float y, float z);
lvec3_t lvec3_add(const lvec3_t a, const lvec3_t b);
lvec3_t lvec3_sub(const lvec3_t a, const lvec3_t b);
lvec3_t lvec3_mul(const lvec3_t a, const lvec3_t b);
lvec3_t lvec3_div(const lvec3_t a, const lvec3_t b);
lvec3_t lvec3_addf(const lvec3_t v, float f);
lvec3_t lvec3_subf(const lvec3_t v, float f);
lvec3_t lvec3_mulf(const lvec3_t v, float f);
lvec3_t lvec3_divf(const lvec3_t v, float f);
float lvec3_sqlength(const lvec3_t v);
float lvec3_length(const lvec3_t v);
lvec3_t lvec3_norm(const lvec3_t v);
float lvec3_dot(const lvec3_t a, const lvec3_t b);
lvec3_t lvec3_cross(const lvec3_t a, const lvec3_t b);
lvec3_t lvec3_mix(const lvec3_t a, const lvec3_t b, float t);
lvec3_t lvec3_deg(const lvec3_t radvec);
lvec3_t lvec3_rad(const lvec3_t degvec);


/* quat */


lquat_t lquat(float w, float x, float y, float z);
lquat_t lquat_fromaxis(float angle, lvec3_t axis);
lquat_t lquat_fromeuler(const lvec3_t euler);
lquat_t lquat_add(const lquat_t a, const lquat_t b);
lquat_t lquat_mul(const lquat_t a, const lquat_t b);
lvec3_t lquat_mulvec3(const lquat_t q, const lvec3_t v);
lquat_t lquat_mulf(const lquat_t q, float f);
lquat_t lquat_divf(const lquat_t q, float f);
lquat_t lquat_norm(const lquat_t q);
lquat_t lquat_conj(const lquat_t q);
lquat_t lquat_lerp(const lquat_t a, const lquat_t b, float t);
lquat_t lquat_slerp(const lquat_t a, const lquat_t b, float t);
float lquat_dot(const lquat_t a, const lquat_t b);
float lquat_angle(const lquat_t q);
lvec3_t lquat_axis(const lquat_t q);
lvec3_t lquat_euler(const lquat_t q);


/* mat4 */


lmat4_t lmat4(const float* v);
lmat4_t lmat4_identity();
lmat4_t lmat4_mul(const lmat4_t a, const lmat4_t b);
lvec3_t lmat4_mulvec3(const lmat4_t m, const lvec3_t v, float w);
lmat4_t lmat4_translate(const lmat4_t m, const lvec3_t v);
lmat4_t lmat4_rotate(const lmat4_t m, float angle, const lvec3_t axis);
lmat4_t lmat4_scale(const lmat4_t m, const lvec3_t v);
lmat4_t lmat4_trans(const lmat4_t m);
lmat4_t lmat4_inv(const lmat4_t m);
lmat4_t lmat4_ortholh(float left, float right, float bottom, float top, float near, float far);
lmat4_t lmat4_frustumlh(float left, float right, float bottom, float top, float near, float far);
lmat4_t lmat4_perspectivelh(float fovy, float aspect, float near, float far);
lmat4_t lmat4_lookatlh(const lvec3_t center, const lvec3_t eye, const lvec3_t up);
lmat4_t lmat4_orthorh(float left, float right, float bottom, float top, float near, float far);
lmat4_t lmat4_frustumrh(float left, float right, float bottom, float top, float near, float far);
lmat4_t lmat4_perspectiverh(float fovy, float aspect, float near, float far);
lmat4_t lmat4_lookatrh(const lvec3_t center, const lvec3_t eye, const lvec3_t up);
lmat4_t lmat4_transform(const lvec3_t position, const lquat_t rotation, const lvec3_t scale);
lmat4_t lmat4_billboard(const lmat4_t view, const lvec3_t pos, float spin, float width, float height, int upfront);


#ifdef __cplusplus
}
#endif


#endif /* LITE_MATH3D_H */




/* IMPLEMENTATION */




#ifdef LITE_MATH3D_IMPLEMENTATION

#include <math.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* vec3 */

lvec3_t lvec3(float x, float y, float z) {
  lvec3_t v;

  v.x = x;
  v.y = y;
  v.z = z;
  return v;
}


lvec3_t lvec3_add(const lvec3_t a, const lvec3_t b) {
  return lvec3(a.x+b.x, a.y+b.y, a.z+b.z);
}


lvec3_t lvec3_sub(const lvec3_t a, const lvec3_t b) {
  return lvec3(a.x-b.x, a.y-b.y, a.z-b.z);
}


lvec3_t lvec3_mul(const lvec3_t a, const lvec3_t b) {
  return lvec3(a.x*b.x, a.y*b.y, a.z*b.z);
}


lvec3_t lvec3_div(const lvec3_t a, const lvec3_t b) {
  return lvec3(a.x/b.x, a.y/b.y, a.z/b.z);
}


lvec3_t lvec3_sumf(const lvec3_t v, float f) {
  return lvec3(v.x+f, v.y+f, v.z+f);
}


lvec3_t lvec3_subf(const lvec3_t v, float f) {
  return lvec3(v.x-f, v.y-f, v.z-f);
}


lvec3_t lvec3_mulf(const lvec3_t v, float f) {
  return lvec3(v.x*f, v.y*f, v.z*f);
}


lvec3_t lvec3_divf(const lvec3_t v, float f) {
  return lvec3_mulf(v, 1/f);
}


float lvec3_sqlength(const lvec3_t v) {
  return lvec3_dot(v, v);
}


float lvec3_length(const lvec3_t v) {
  return (float)sqrt(lvec3_sqlength(v));
}


lvec3_t lvec3_norm(const lvec3_t v) {
  float length, invlength;

  length = lvec3_length(v);
  invlength = length > 0 ? (1 / length) : 0;
  return lvec3_mulf(v, invlength);
}


float lvec3_dot(const lvec3_t a, const lvec3_t b) {
  return a.x*b.x + a.y*b.y + a.z*b.z;
}


lvec3_t lvec3_cross(const lvec3_t a, const lvec3_t b) {
  return lvec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}


lvec3_t lvec3_mix(const lvec3_t a, const lvec3_t b, float t) {
  return lvec3_add(a, lvec3_mulf(lvec3_sub(b, a), t));
}


lvec3_t lvec3_deg(const lvec3_t radvec) {
  return lvec3(lm_rad2deg(radvec.x), lm_rad2deg(radvec.y), lm_rad2deg(radvec.z));
}


lvec3_t lvec3_rad(const lvec3_t degvec) {
  return lvec3(lm_deg2rad(degvec.x), lm_deg2rad(degvec.y), lm_deg2rad(degvec.z));
}


/* quat */


lquat_t lquat(float w, float x, float y, float z) {
  lquat_t q;

  q.w = w;
  q.x = x;
  q.y = y;
  q.z = z;
  return q;
}


lquat_t lquat_fromaxis(float angle, const lvec3_t axis) {
  lvec3_t v;
  float s, w, x, y, z;

  angle *= 0.5f;
  v = lvec3_mulf(lvec3_norm(axis), sin(angle));
  return lquat(cos(angle), v.x, v.y, v.z);
}


lquat_t lquat_fromeuler(const lvec3_t euler) {
  float halfx, halfy, halfz;
  float sinx, siny, sinz;
  float cosx, cosy, cosz;
  float w, x, y, z;

  halfx = euler.x * 0.5f;
  halfy = euler.y * 0.5f;
  halfz = euler.z * 0.5f;
  sinx = sin(halfx);
  siny = sin(halfy);
  sinz = sin(halfz);
  cosx = cos(halfx);
  cosy = cos(halfy);
  cosz = cos(halfz);

  w = cosx * cosy * cosz + sinx * siny * sinz;
  x = sinx * cosy * cosz - cosx * siny * sinz;
  y = cosx * siny * cosz + sinx * cosy * sinz;
  z = cosx * cosy * sinz - sinx * siny * cosz;
  return lquat(w, x, y, z);
}


lquat_t lquat_add(const lquat_t a, const lquat_t b) {
  return lquat(a.w+b.w, a.x+b.x, a.y+b.y, a.z+b.z);
}


lquat_t lquat_mul(const lquat_t a, const lquat_t b) {
  return lquat(
          a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z,
          a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
          a.w*b.y + a.y*b.w + a.z*b.x - a.x*b.z,
          a.w*b.z + a.z*b.w + a.x*b.y - a.y*b.x);
}


lvec3_t lquat_mulvec3(const lquat_t q, const lvec3_t v) {
  lquat_t vq, rq;
  rq = lquat_mul(lquat_mul(q, lquat(0, v.x, v.y, v.z)), lquat_conj(q));
  return lvec3(rq.x, rq.y, rq.z);
}


lquat_t lquat_mulf(const lquat_t q, float f) {
  return lquat(q.w*f, q.x*f, q.y*f, q.z*f);
}


lquat_t lquat_divf(const lquat_t q, float f) {
  return lquat_mulf(q, 1/f);
}


lquat_t lquat_norm(const lquat_t q) {
  float mag2;
  lquat_t outq;

  outq = q;
  mag2 = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
  if (mag2 > 0.00001f  &&  fabs(mag2 - 1.0f) > 0.00001f) {
    float invmag;
    invmag = 1.0f / (float)sqrt(mag2);
    outq.w *= invmag;
    outq.x *= invmag;
    outq.y *= invmag;
    outq.z *= invmag;
  }
  return outq;
}


lquat_t lquat_conj(const lquat_t q) {
  return lquat(q.w, -q.x, -q.y, -q.z);
}


lquat_t lquat_lerp(const lquat_t a, const lquat_t b, float t) {
  return lquat_norm(lquat_add(lquat_mulf(a, 1-t), lquat_mulf(b, t)));
}


lquat_t lquat_slerp(const lquat_t a, const lquat_t b, float t) {
  lquat_t q;
  float dot;
  
  dot = lquat_dot(a, b);
  if (dot < 0) {
    dot *= -1;
    q = lquat_mulf(b, -1);
  } else {
    q = b;
  }
  if (dot < 0.95f) {
    float angle;

    angle = acos(dot);
    return lquat_add(lquat_mulf(a, sin(angle*(1-t))), lquat_divf(lquat_mulf(q, sin(angle*t)), sin(angle)));
  } else {
    return lquat_lerp(a, q, t);
  }
}


float lquat_dot(const lquat_t a, const lquat_t b) {
  return a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;
}


float lquat_angle(const lquat_t q) {
  return acos(q.w) * 2.0f;
}


lvec3_t lquat_axis(const lquat_t q) {
  float scale, invscale;

  scale = (float)sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
  invscale = (scale > 0) ? (1 / scale) : 0;
  return lvec3(q.x*invscale, q.y*invscale, q.z*invscale);
}


lvec3_t lquat_euler(const lquat_t q) {
  float rx, ry, rz;

  rx = atan2(2 * (q.y*q.z + q.w*q.x), q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z);
  ry = asin(-2 * (q.x*q.z - q.w*q.y));
  rz = atan2(2 * (q.x*q.y + q.w*q.z), q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z);
  return lvec3(rx, ry, rz);
}


/* mat4 */


lmat4_t lmat4(const float* v) {
  lmat4_t mat;

  memcpy(&mat, v, sizeof(mat.m));
  return mat;
}


lmat4_t lmat4_identity() {
  lmat4_t mat;

  memset(&mat, 0, sizeof(mat.m));
  mat.m[0] = mat.m[5] = mat.m[10] = mat.m[15] = 1;
  return mat;
}


lmat4_t lmat4_mul(const lmat4_t a, const lmat4_t b) {
  int i;
  lmat4_t mat;

  for (i = 0; i < 4; i++) {
    float a0, a1, a2, a3;

    a0 = a.m[i];
    a1 = a.m[i+4];
    a2 = a.m[i+8];
    a3 = a.m[i+12];
    mat.m[i] = a0*b.m[0] + a1*b.m[1] + a2*b.m[2] + a3*b.m[3];
    mat.m[i+4] = a0*b.m[4] + a1*b.m[5] + a2*b.m[6] + a3*b.m[7];
    mat.m[i+8] = a0*b.m[8] + a1*b.m[9] + a2*b.m[10] + a3*b.m[11];
    mat.m[i+12] = a0*b.m[12] + a1*b.m[13] + a2*b.m[14] + a3*b.m[15];
  }
  return mat;
}


lvec3_t lmat4_mulvec3(const lmat4_t m, const lvec3_t v, float w) {
  lmat4_t mat;

  mat = lmat4_identity();
  mat.m[12] = v.x;
  mat.m[13] = v.y;
  mat.m[14] = v.z;
  mat.m[15] = w;
  mat = lmat4_mul(m, mat);
  return lvec3(mat.m[12], mat.m[13], mat.m[14]);
}


lmat4_t lmat4_translate(const lmat4_t m, const lvec3_t v) {
  lmat4_t mat;

  mat = lmat4_identity();
  mat.m[12] = v.x;
  mat.m[13] = v.y;
  mat.m[14] = v.z;
  return lmat4_mul(m, mat);
}


lmat4_t lmat4_rotate(const lmat4_t m, float angle, const lvec3_t axis) {
  float c, s;
  float xx, xy, xz, yy, yz, zz;
  lmat4_t mat;

  c = cos(angle);
  s = sin(angle);
  xx = axis.x * axis.x;
  xy = axis.x * axis.y;
  xz = axis.x * axis.z;
  yy = axis.y * axis.y;
  yz = axis.y * axis.z;
  zz = axis.z * axis.z;
  mat = lmat4_identity();
  mat.m[0] = xx * (1 - c) + c;
  mat.m[1] = xy * (1 - c) + axis.z * s;
  mat.m[2] = xz * (1 - c) - axis.y * s;
  mat.m[4] = xy * (1 - c) - axis.z * s;
  mat.m[5] = yy * (1 - c) + c;
  mat.m[6] = yz * (1 - c) + axis.x * s;
  mat.m[8] = xz * (1 - c) + axis.y * s;
  mat.m[9] = yz * (1 - c) - axis.x * s;
  mat.m[10] = zz * (1 - c) + c;
  return lmat4_mul(m, mat);
}


lmat4_t lmat4_scale(const lmat4_t m, const lvec3_t v) {
  lmat4_t mat;
  
  mat = lmat4_identity();
  mat.m[0] = v.x;
  mat.m[5] = v.y;
  mat.m[10] = v.z;
  return lmat4_mul(m, mat);
}


lmat4_t lmat4_trans(const lmat4_t m) {
  int r, c;
  lmat4_t mat;

  for (r = 0; r < 4; r++) {
    for (c = 0; c < 4; c++) {
      mat.m[c*4 + r] = m.m[r*4 + c];
    }
  }
  return mat;
}


lmat4_t lmat4_inv(const lmat4_t m) {
  int i;
  float det, invdet;
  lmat4_t inv;

  inv.m[ 0] =  m.m[5] * m.m[10] * m.m[15] - m.m[5] * m.m[11] * m.m[14] - m.m[9] * m.m[6] * m.m[15] + m.m[9] * m.m[7] * m.m[14] + m.m[13] * m.m[6] * m.m[11] - m.m[13] * m.m[7] * m.m[10];
  inv.m[ 4] = -m.m[4] * m.m[10] * m.m[15] + m.m[4] * m.m[11] * m.m[14] + m.m[8] * m.m[6] * m.m[15] - m.m[8] * m.m[7] * m.m[14] - m.m[12] * m.m[6] * m.m[11] + m.m[12] * m.m[7] * m.m[10];
  inv.m[ 8] =  m.m[4] * m.m[ 9] * m.m[15] - m.m[4] * m.m[11] * m.m[13] - m.m[8] * m.m[5] * m.m[15] + m.m[8] * m.m[7] * m.m[13] + m.m[12] * m.m[5] * m.m[11] - m.m[12] * m.m[7] * m.m[ 9];
  inv.m[12] = -m.m[4] * m.m[ 9] * m.m[14] + m.m[4] * m.m[10] * m.m[13] + m.m[8] * m.m[5] * m.m[14] - m.m[8] * m.m[6] * m.m[13] - m.m[12] * m.m[5] * m.m[10] + m.m[12] * m.m[6] * m.m[ 9];
  inv.m[ 1] = -m.m[1] * m.m[10] * m.m[15] + m.m[1] * m.m[11] * m.m[14] + m.m[9] * m.m[2] * m.m[15] - m.m[9] * m.m[3] * m.m[14] - m.m[13] * m.m[2] * m.m[11] + m.m[13] * m.m[3] * m.m[10];
  inv.m[ 5] =  m.m[0] * m.m[10] * m.m[15] - m.m[0] * m.m[11] * m.m[14] - m.m[8] * m.m[2] * m.m[15] + m.m[8] * m.m[3] * m.m[14] + m.m[12] * m.m[2] * m.m[11] - m.m[12] * m.m[3] * m.m[10];
  inv.m[ 9] = -m.m[0] * m.m[ 9] * m.m[15] + m.m[0] * m.m[11] * m.m[13] + m.m[8] * m.m[1] * m.m[15] - m.m[8] * m.m[3] * m.m[13] - m.m[12] * m.m[1] * m.m[11] + m.m[12] * m.m[3] * m.m[ 9];
  inv.m[13] =  m.m[0] * m.m[ 9] * m.m[14] - m.m[0] * m.m[10] * m.m[13] - m.m[8] * m.m[1] * m.m[14] + m.m[8] * m.m[2] * m.m[13] + m.m[12] * m.m[1] * m.m[10] - m.m[12] * m.m[2] * m.m[ 9];
  inv.m[ 2] =  m.m[1] * m.m[ 6] * m.m[15] - m.m[1] * m.m[ 7] * m.m[14] - m.m[5] * m.m[2] * m.m[15] + m.m[5] * m.m[3] * m.m[14] + m.m[13] * m.m[2] * m.m[ 7] - m.m[13] * m.m[3] * m.m[ 6];
  inv.m[ 6] = -m.m[0] * m.m[ 6] * m.m[15] + m.m[0] * m.m[ 7] * m.m[14] + m.m[4] * m.m[2] * m.m[15] - m.m[4] * m.m[3] * m.m[14] - m.m[12] * m.m[2] * m.m[ 7] + m.m[12] * m.m[3] * m.m[ 6];
  inv.m[10] =  m.m[0] * m.m[ 5] * m.m[15] - m.m[0] * m.m[ 7] * m.m[13] - m.m[4] * m.m[1] * m.m[15] + m.m[4] * m.m[3] * m.m[13] + m.m[12] * m.m[1] * m.m[ 7] - m.m[12] * m.m[3] * m.m[ 5];
  inv.m[14] = -m.m[0] * m.m[ 5] * m.m[14] + m.m[0] * m.m[ 6] * m.m[13] + m.m[4] * m.m[1] * m.m[14] - m.m[4] * m.m[2] * m.m[13] - m.m[12] * m.m[1] * m.m[ 6] + m.m[12] * m.m[2] * m.m[ 5];
  inv.m[ 3] = -m.m[1] * m.m[ 6] * m.m[11] + m.m[1] * m.m[ 7] * m.m[10] + m.m[5] * m.m[2] * m.m[11] - m.m[5] * m.m[3] * m.m[10] - m.m[ 9] * m.m[2] * m.m[ 7] + m.m[ 9] * m.m[3] * m.m[ 6];
  inv.m[ 7] =  m.m[0] * m.m[ 6] * m.m[11] - m.m[0] * m.m[ 7] * m.m[10] - m.m[4] * m.m[2] * m.m[11] + m.m[4] * m.m[3] * m.m[10] + m.m[ 8] * m.m[2] * m.m[ 7] - m.m[ 8] * m.m[3] * m.m[ 6];
  inv.m[11] = -m.m[0] * m.m[ 5] * m.m[11] + m.m[0] * m.m[ 7] * m.m[ 9] + m.m[4] * m.m[1] * m.m[11] - m.m[4] * m.m[3] * m.m[ 9] - m.m[ 8] * m.m[1] * m.m[ 7] + m.m[ 8] * m.m[3] * m.m[ 5];
  inv.m[15] =  m.m[0] * m.m[ 5] * m.m[10] - m.m[0] * m.m[ 6] * m.m[ 9] - m.m[4] * m.m[1] * m.m[10] + m.m[4] * m.m[2] * m.m[ 9] + m.m[ 8] * m.m[1] * m.m[ 6] - m.m[ 8] * m.m[2] * m.m[ 5];
  det = m.m[0] * inv.m[0] + m.m[1] * inv.m[4] + m.m[2] * inv.m[8] + m.m[3] * inv.m[12];
  if (fabs(det) <= 0.00001f) return lmat4(m.m);
  invdet = 1.0f / det;
  for (i = 0; i < 16; i++) {
    inv.m[i] *= invdet;
  }
  return inv;
}


lmat4_t lmat4_ortholh(float left, float right, float bottom, float top, float near_, float far_) {
  float a, b, c, tx, ty, tz;
  lmat4_t mat;

  a = 2.0f / (right - left);
  b = 2.0f / (top - bottom);
  c = 2.0f / (far_ - near_);
  tx = -(right+left) / (right-left);
  ty = -(top+bottom) / (top-bottom);
  tz = -(far_+near_) / (far_-near_);
  mat = lmat4_identity();
  mat.m[0] = a;
  mat.m[5] = b;
  mat.m[10] = c;
  mat.m[12] = tx;
  mat.m[13] = ty;
  mat.m[14] = tz;
  return mat;
}


lmat4_t lmat4_frustumlh(float left, float right, float bottom, float top, float near_, float far_) {
  lmat4_t mat;
  
  mat = lmat4_identity();
  mat.m[0]  = 2 * near_ / (right - left);
  mat.m[5]  = 2 * near_ / (top - bottom);
  mat.m[8]  = -(right + left) / (right - left);
  mat.m[9]  = -(top + bottom) / (top - bottom);
  mat.m[10] = (far_ + near_) / (far_ - near_);
  mat.m[11] = 1;
  mat.m[14] = -(2 * far_ * near_) / (far_ - near_);
  mat.m[15] = 0;
  return mat;
}


lmat4_t lmat4_perspectivelh(float fovy, float aspect, float near_, float far_) {
  float tangent, width, height;

  tangent = (float)tan(fovy/2);
  height = near_ * tangent;
  width = height * aspect;
  return lmat4_frustumlh(-width, width, -height, height, near_, far_);
}


lmat4_t lmat4_lookatlh(const lvec3_t center, const lvec3_t eye, const lvec3_t up) {
  lmat4_t mat;
  lvec3_t x, y, z;

  z = lvec3_norm(lvec3_sub(eye, center));
  x = lvec3_cross(up, z);
  y = lvec3_cross(z, x);
  x = lvec3_norm(x);
  y = lvec3_norm(y);
  mat = lmat4_identity();
  mat.m[0] = x.x;
  mat.m[1] = y.x;
  mat.m[2] = z.x;
  mat.m[4] = x.y;
  mat.m[5] = y.y;
  mat.m[6] = z.y;
  mat.m[8] = x.z;
  mat.m[9] = y.z;
  mat.m[10] = z.z;
  return lmat4_translate(mat, lvec3_mulf(center, -1));
}


lmat4_t lmat4_orthorh(float left, float right, float bottom, float top, float near_, float far_) {
  float a, b, c, tx, ty, tz;
  lmat4_t mat;

  a = 2.0f / (right - left);
  b = 2.0f / (top - bottom);
  c = -2.0f / (far_ - near_);
  tx = -(right+left) / (right-left);
  ty = -(top+bottom) / (top-bottom);
  tz = -(far_+near_) / (far_-near_);
  mat = lmat4_identity();
  mat.m[0] = a;
  mat.m[5] = b;
  mat.m[10] = c;
  mat.m[12] = tx;
  mat.m[13] = ty;
  mat.m[14] = tz;
  return mat;
}


lmat4_t lmat4_frustumrh(float left, float right, float bottom, float top, float near_, float far_) {
  lmat4_t mat;
  
  mat = lmat4_identity();
  mat.m[0]  = 2 * near_ / (right - left);
  mat.m[5]  = 2 * near_ / (top - bottom);
  mat.m[8]  = (right + left) / (right - left);
  mat.m[9]  = (top + bottom) / (top - bottom);
  mat.m[10] = -(far_ + near_) / (far_ - near_);
  mat.m[11] = -1;
  mat.m[14] = -(2 * far_ * near_) / (far_ - near_);
  mat.m[15] = 0;
  return mat;
}


lmat4_t lmat4_perspectiverh(float fovy, float aspect, float near_, float far_) {
  float tangent, width, height;

  tangent = (float)tan(fovy/2);
  height = near_ * tangent;
  width = height * aspect;
  return lmat4_frustumrh(-width, width, -height, height, near_, far_);
}


lmat4_t lmat4_lookatrh(const lvec3_t center, const lvec3_t eye, const lvec3_t up) {
  lmat4_t mat;
  lvec3_t x, y, z;

  z = lvec3_norm(lvec3_sub(center, eye));
  x = lvec3_cross(up, z);
  y = lvec3_cross(z, x);
  x = lvec3_norm(x);
  y = lvec3_norm(y);
  mat = lmat4_identity();
  mat.m[0] = x.x;
  mat.m[1] = y.x;
  mat.m[2] = z.x;
  mat.m[4] = x.y;
  mat.m[5] = y.y;
  mat.m[6] = z.y;
  mat.m[8] = x.z;
  mat.m[9] = y.z;
  mat.m[10] = z.z;
  return lmat4_translate(mat, lvec3_mulf(center, -1));
}


lmat4_t lmat4_transform(const lvec3_t position, const lquat_t rotation, const lvec3_t scale) {
  lvec3_t axis;
  lmat4_t mat;

  axis = lquat_axis(rotation);
  mat = lmat4_identity();
  mat = lmat4_translate(mat, position);
  mat = lmat4_rotate(mat, lquat_angle(rotation), axis);
  mat = lmat4_scale(mat, scale);
  return mat;
}


lmat4_t lmat4_billboard(const lmat4_t view, const lvec3_t pos, float spin, float width, float height, int upfront) {
  lmat4_t mat;
  mat = lmat4_trans(view);
  mat.m[3] = 0;
  mat.m[7] = 0;
  mat.m[11] = 0;
  mat.m[12] = pos.x;
  mat.m[13] = pos.y;
  mat.m[14] = pos.z;
  mat.m[15] = 1;
  if (upfront) {
    mat.m[4] = 0;
    mat.m[5] = 1;
    mat.m[6] = 0;
  }
  mat = lmat4_rotate(mat, spin, lvec3(0, 0, 1));
  mat = lmat4_scale(mat, lvec3(width, height, 1));
  return mat;
}


#ifdef __cplusplus
}
#endif

#endif /* LITE_MATH3D_IMPLEMENTATION */
