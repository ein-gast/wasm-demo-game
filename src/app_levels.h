#include "app_defs.h"

int place(objstate *list, int listSize, objstate newObj) {
  for (int i = 0; i < listSize; i++) {
    if (list[i].type != OTYPE_NONE) {
      continue;
    }
    list[i] = newObj;
    return i;
  }
  return -1;
}

// расстановка объектов на старате
void level0(objstate *list) {
  for (int i = 0; i < 10; i++) {
    list[i].type = OTYPE_ERING;
    list[i].x = (1 + i) * PIXSZ;
    list[i].y = (1 + i) * 2 * PIXSZ + 200;
  }
}