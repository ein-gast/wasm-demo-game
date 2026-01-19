#include "app_defs.h"
#include "app_walls.h"

int place(objState *list, int listSize, objState newObj) {
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
void level0(objState *list, int levelWidth) {
  for (int i = 0; i < 10; i++) {
    list[i].type = OTYPE_ERING;
    list[i].x = (1 + i) * PIXSZ;
    list[i].y = (1 + i) * 2 * PIXSZ + 200;
  }
  walSect newSect;
  createWal(&newSect, levelWidth, PIXSZ * 3);
  newSect.y = levelWidth;
  newSect.xRight = levelWidth - newSect.xLeft;
  pushWal(&newSect, false);
  for (int i = 0; i < 18; i++) {
    createWal(&newSect, levelWidth, PIXSZ * 3);
    pushWal(&newSect, true);
  }
}