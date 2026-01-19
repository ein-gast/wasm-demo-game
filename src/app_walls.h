#include "app_defs.h"
#include "app_rnd.h"
#include <stdbool.h>

void createWal(walSect *newSect, int levelWidth, int minspaceWidth);
void pushWal(const walSect *newSect, bool recomputeY);
const walSect *walFirst();
const walSect *walNext(const walSect *curr);
void walReset();
bool walIsEmpty();
int walHeadForward();
int walTailForward();

// #define _COMPILE_WALLS_
#ifdef _COMPILE_WALLS_
// кольцевой буфер стенок
walSect walls[WALCNT];
int walHead = 0;
int walTail = 0;

void createWal(walSect *newSect, int levelWidth, int minspaceWidth) {
  newSect->y = 0;
  newSect->yDist = iRand(20, levelWidth);
  newSect->xLeft = iRand(1, levelWidth *1 / 5);
  newSect->xRight = iRand(levelWidth * 4 / 5, levelWidth - 1);

  int maxSideCorridorWidth = (newSect->xRight - newSect->xLeft) / 2;
  if (maxSideCorridorWidth > minspaceWidth && fRand() > 0.7) {
    newSect->mXLeft =
        newSect->xLeft + iRand(minspaceWidth, maxSideCorridorWidth);
    newSect->mXRight =
        newSect->xRight - iRand(minspaceWidth, maxSideCorridorWidth);
  } else {
    newSect->mXLeft = -1;
    newSect->mXRight = -1;
  }
}

void pushWal(const walSect *newSect, bool recomputeY) {
  walls[walHead] = *newSect;
  walls[walHead].index = walHead;
  if (recomputeY) {
    int newY = 0;
    if (!walIsEmpty()) {
      int oldHead = (WALCNT + walHead - 1) % WALCNT;
      newY = walls[oldHead].y + walls[oldHead].yDist;
    }
    walls[walHead].y = newY;
  }
  walHeadForward();
}

const walSect *walFirst() {
  if (walIsEmpty()) {
    return nullptr;
  }
  return walls + walTail;
}

const walSect *walNext(const walSect *curr) {
  if (curr == nullptr) {
    return nullptr;
  }
  int i = (curr->index + 1) % WALCNT;
  if (i == walHead) {
    return nullptr;
  }
  return walls + i;
}

int walHeadForward() {
  walHead = (walHead + 1) % WALCNT;
  if (walHead == walTail) {
    walTail = (walTail + 1) % WALCNT;
  }
  return walHead;
}

int walTailForward() {
  if (walHead == walTail) {
    return walTail;
  }
  walTail = (walTail + 1) % WALCNT;
  return walTail;
}

void inline walReset() { walTail = walHead = 0; }
bool inline walIsEmpty() { return walTail == walHead; }
#endif