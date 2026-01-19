#include "app_defs.h"

lightState lights[LITCNT];
int lightPtr = 0;

void litAdd(int x, int y) {
  if (lightPtr > LITCNT) {
    return;
  }
  lights[lightPtr].x = x;
  lights[lightPtr].y = y;
  lights[lightPtr].val = 100;
  lights[lightPtr].r = 255;
  lights[lightPtr].g = 255;
  lights[lightPtr].b = 255;
  lightPtr++;
}

void litReset() { lightPtr = 0; }

col3 litVal(int x, int y) {
  col3 res = {0, 0, 0};
  if (!lightPtr) {
    return res;
  }
  int r = 0, g = 0, b = 0;
  float dist;
  for (int i = 0; i < lightPtr; i++) {
    dist = (x - lights[i].x) * (x - lights[i].x) +
           (y - lights[i].y) * (y - lights[i].y);
    if (dist < 1.0)
      dist = 1.0;
    r += lights[i].r / dist;
    g += lights[i].g / dist;
    b += lights[i].b / dist;
  }
  const float lwr = 2.0;
  res.r = r / lwr;
  res.g = g / lwr;
  res.b = b / lwr;
  return res;
}