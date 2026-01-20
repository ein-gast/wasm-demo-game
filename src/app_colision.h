#include "app_defs.h"
#include <stdbool.h>

void colReset();

void colWalAdd(const walSect *obj, int index);
void colObjAdd(const objState *obj, int index);
void colPlayrAdd(int x, int y);

int collWithWall(int x, int y, int w, int h);
int collWithObj(int x, int y);
bool collWithPlayer(int x, int y);

#ifdef _COMPILE_COLLISION_
struct {
  const walSect *ptr;
  int index;
} colWal[WALCNT];

struct {
  const objState *ptr;
  int index;
} colObj[OBJCNT];

struct {
  int x;
  int y;
} colPlayr;

int colWalHead;
int colObjHead;

void colReset() {
  colWalHead = 0;
  colObjHead = 0;
}

void colWalAdd(const walSect *obj, int index) {
  colWal[colWalHead].ptr = obj;
  colWal[colWalHead].index = index;
  colWalHead = (colWalHead + 1) % WALCNT;
}

void colObjAdd(const objState *obj, int index) {
  colObj[colObjHead].ptr = obj;
  colObj[colObjHead].index = index;
  colObjHead = (colObjHead + 1) % OBJCNT;
}

void colPlayrAdd(int x, int y) {
  colPlayr.x = x;
  colPlayr.y = y;
}

int collWithWall(int x, int y, int w, int h) {
  const walSect *wal;
  for (int i = 0; i < colWalHead; i++) {
    wal = colWal[i].ptr;

    if (y + h / 2 < wal->y || y - h / 2 > wal->y + wal->yDist) {
      continue;
    }

    if (x - w / 2 < wal->xLeft || x + w / 2 > wal->xRight ||
        (x + w / 2 >= wal->mXLeft && x - w / 2 <= wal->mXRight)) {
      return colWal[i].index;
    }
  }
  return -1;
}

int collWithObj(int x, int y) {
  long unsigned int dist2;
  for (int i = 0; i < colObjHead; i++) {
    dist2 = (x - colObj[i].ptr->x) * (x - colObj[i].ptr->x) +
            (y - colObj[i].ptr->y) * (y - colObj[i].ptr->y);
    if (dist2 < PIXSZ2 * PIXSZ2) {
      return colObj[i].index;
    }
  }
  return -1;
}

bool collWithPlayer(int x, int y) {
  long unsigned int dist2 =
      (x - colPlayr.x) * (x - colPlayr.x) + (y - colPlayr.y) * (y - colPlayr.y);
  return dist2 < PIXSZ2 * PIXSZ2;
}
#endif