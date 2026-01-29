#include "app_defs.h"
#include "app_rnd.h"
#include "app_walls.h"

/*
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
*/

objState *placePtr(objState *list, int listSize) {
  for (int i = 0; i < listSize; i++) {
    if (list[i].type != OTYPE_NONE) {
      continue;
    }
    return list + i;
  }
  return nullptr;
}

void createSection(objState *list, int levelWidth) {
  walSect newSect;
  objState *newObj;
  createWal(&newSect, levelWidth, PIXSZ * 3);
  int y = pushWal(&newSect, true);
  y += newSect.yDist / 2;
  if (newSect.mXLeft > 0) {
    if (nullptr != (newObj = placePtr(list, OBJCNT))) {
      newObj->type = OTYPE_ERING;
      newObj->x = iRand(newSect.xLeft + PIXSZ, newSect.mXLeft - PIXSZ);
      newObj->y = y;
    }
    if (nullptr != (newObj = placePtr(list, OBJCNT))) {
      newObj->type = OTYPE_ERING;
      newObj->x = iRand(newSect.mXRight + PIXSZ, newSect.xRight - PIXSZ);
      newObj->y = y;
    }
  } else {
    if (nullptr != (newObj = placePtr(list, OBJCNT))) {
      newObj->type = OTYPE_ERING;
      newObj->x = iRand(newSect.xLeft + PIXSZ, newSect.xRight - PIXSZ);
      newObj->y = y;
    }
    // if (nullptr != (newObj = placePtr(list, OBJCNT))) {
    //   newObj->type = OTYPE_ERING;
    //   newObj->x = newSect.xRight - PIXSZ;
    //   newObj->y = y;
    // }
  }
  //}
}

// расстановка объектов на старате
void level0(objState *list, int levelWidth) {
  objState *newObj;
  int i;
  /*
  for (i = 0; i < 10; i++) {
    if (nullptr != (newObj = placePtr(list, OBJCNT))) {
      newObj->type = OTYPE_ERING;
      newObj->x = (1 + i) * PIXSZ;
      newObj->y = (1 + i) * 2 * PIXSZ + 200;
    }
  }
  */
  walSect newSect;
  createWal(&newSect, levelWidth, PIXSZ * 3);
  newSect.y = levelWidth;
  newSect.xRight = levelWidth - newSect.xLeft;
  pushWal(&newSect, false);
  //
  for (i = 0; i < 18; i++) {
    createSection(list, levelWidth);
  }
}
