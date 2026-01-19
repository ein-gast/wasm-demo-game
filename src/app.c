#include "app_defs.h"
#include "app_levels.h"
#include "app_light.h"
#include "app_pixdata.h"
#include "app_statelogic.h"
#define _COMPILE_WALLS_
#include "app_walls.h"
#define _COMPILE_RND_
#include "app_rnd.h"

void putPix(byte *canvas, int bpp, int toX, int toY, const byte pal[256][4],
            const byte pix[PIXSZ][PIXSZ + 1]);
void putProj(byte *canvas, int bpp, int toX, int toY);
void putWall(byte *canvas, int bpp, const walSect *wal);

void init(int w, int h) {
  state.winW = w;
  state.winH = h;
  for (int i = 0; i < OBJCNT; i++) {
    state.obj[i].type = OTYPE_NONE;
  }
  for (int i = 0; i < OBJCNT; i++) {
    state.projctl[i].type = OTYPE_NONE;
  }
  state.ammo = 0;
  state.shield = 0;
  state.plXDir = +1;
  state.plXDir = +1;
  state.vpW = w;
  state.vpH = h;
  state.vpY = h;
  state.plX = w / 2;
  state.plY = state.vpY - h + PIXSZ * 1.5;
  //
  unpakPal(pixShp16x16);
  unpakPal(pixRing16x16);
  level0(state.obj, state.vpW);
}

void input(int code1, int code2) { state.plXDir = -state.plXDir; }

void process(int t) {
  static unsigned int procFrame = 0;
  // viewport
  state.vpY++;
  // state.vpY=100;

  // walls
  const walSect *ws;
  int lastY = 0;
  ws = walFirst();
  while (ws != nullptr) {
    lastY = ws->y;
    if (ws->y + ws->yDist < state.vpY - state.vpH) {
      walTailForward();
      ws = walFirst();
      continue;
    }
    ws = walNext(ws);
  }
  if (lastY < state.vpY) {
    // grow
    walSect newSect;
    createWal(&newSect, state.vpW, PIXSZ * 3);
    pushWal(&newSect, true);
  }

  // player
  switch (state.plXDir) {
  case +1:
    if (state.plX < state.vpW - PIXSZ2) {
      state.plX++;
    }
    break;
  case -1:
    if (state.plX > PIXSZ2) {
      state.plX--;
    }
    break;
  }
  state.plY++;

  // objects
  for (int i = 0; i < OBJCNT; i++) {
    switch (state.obj[i].type) {
    case OTYPE_ERING:
      o_ering(i);
      break;
    case OTYPE_EBOX:
      o_ebox(i);
      break;
    }
  }

  // projectiles
  for (int i = 0; i < PROJCNT; i++) {
    switch (state.projctl[i].type) {
    case OTYPE_BNORM:
      o_prjctl_norm(i);
      break;
    case OTYPE_BPOWER:
      o_prjctl_pwr(i);
      break;
    }
  }

  if (procFrame % 10 == 0) {
    objState s = {.type = OTYPE_BNORM, .x = state.plX, .y = state.plY};
    place(state.projctl, PROJCNT, s);
  }

  procFrame++;
}

int render(int t, byte *input) {
  static unsigned int frame = 0;
  const int bytePerPixel = 4;

  // setup lights
  litReset();
  for (int i = 0; i < PROJCNT; i++) {
    switch (state.projctl[i].type) {
    case OTYPE_BNORM:
      litAdd(state.projctl[i].x - PIXSZ2 + 2, state.projctl[i].y);
      litAdd(state.projctl[i].x + PIXSZ2 - 2, state.projctl[i].y);
      break;
    }
  }

  // clear
  int i = 0, cnt = state.winW * state.winH;
  col3 l = {10, 10, 10};
  for (int y = 0; y < state.winH; y++) {
    for (int x = 0; x < state.winW; x++) {
      i = x + y * state.winW;
      l = litVal(x, state.vpY - y);
      input[i * bytePerPixel + 0] = l.r;
      input[i * bytePerPixel + 1] = l.g;
      input[i * bytePerPixel + 2] = l.b;
      input[i * bytePerPixel + 3] = 255;
    }
  }

  // walls
  int sum = 0;
  const walSect *ws;
  ws = walFirst();
  while (ws != nullptr) {
    if (ws->y > state.vpY) {
      break;
    }
    sum++;
    putWall(input, bytePerPixel, ws);
    ws = walNext(ws);
  }

  // player
  int px = state.plX;
  int py = state.vpY - state.plY;

  putPix(input, bytePerPixel, px, py, pixShp16x16.pal, pixShp16x16.pix);

  // objects
  for (int i = 0; i < OBJCNT; i++) {
    switch (state.obj[i].type) {
    case OTYPE_ERING:
      putPix(input, bytePerPixel, state.obj[i].x, state.vpY - state.obj[i].y,
             pixRing16x16.pal, pixRing16x16.pix);
      break;
    case OTYPE_EBOX:
      break;
    }
  }
  for (int i = 0; i < PROJCNT; i++) {
    switch (state.projctl[i].type) {
    case OTYPE_BNORM:
      putProj(input, bytePerPixel, state.projctl[i].x,
              state.vpY - state.projctl[i].y);
      break;
    case OTYPE_BPOWER:
      break;
    }
  }

  
  frame++;
  return state.vpY;
}

void putPix(byte *canvas, int bpp, int toX, int toY, const byte pal[256][4],
            const byte pix[PIXSZ][PIXSZ + 1]) {
  int ofs;
  toX -= PIXSZ2;
  toY -= PIXSZ2;
  for (int py = 0; py < PIXSZ; py++) {
    if (toY + py < 0 || toY + py >= state.winH) {
      continue;
    }
    for (int px = 0; px < PIXSZ; px++) {
      if (toX + px < 0 || toX + px >= state.winW) {
        continue;
      }
      if (!pal[pix[py][px]][3]) {
        continue;
      }
      ofs = toX + px + (toY + py) * state.winW;
      canvas[ofs * bpp + 0] = pal[pix[py][px]][0];
      canvas[ofs * bpp + 1] = pal[pix[py][px]][1];
      canvas[ofs * bpp + 2] = pal[pix[py][px]][2];
      // canvas[ofs*bpp+3] = pal[pix[py][px]][3];
    }
  }
}

void putProj(byte *canvas, int bpp, int toX, int toY) {
  int x1 = toX - PIXSZ2 + 2, y1 = toY;
  int x2 = toX + PIXSZ2 - 2, y2 = toY;
  int ofs;
  if (x1 >= 0 && x1 < state.winW && y1 >= 0 && y1 < state.winH) {
    ofs = x1 + y1 * state.winW;
    canvas[ofs * bpp + 0] = 0;
    canvas[ofs * bpp + 1] = 255;
    canvas[ofs * bpp + 2] = 0;
  }
  if (x2 >= 0 && x2 < state.winW && y2 >= 0 && y2 < state.winH) {
    ofs = x2 + y2 * state.winW;
    canvas[ofs * bpp + 0] = 0;
    canvas[ofs * bpp + 1] = 255;
    canvas[ofs * bpp + 2] = 0;
  }
}

void putWall(byte *canvas, int bpp, const walSect *wal) {
  int y1 = state.vpY - wal->y - wal->yDist;
  int y2 = y1 + wal->yDist;
  if ((y1 > state.winH && y2 > state.winH) || (y1 < 0 && y2 < 0)) {
    return;
  }

  int ofs, x, y;
  for (y = y1; y < y2; y++) {
    if (y >= state.winH || y < 0) {
      continue;
    }

    for (x = 0; x < state.winW; x++) {
      ofs = x + y * state.winW;
      // canvas[ofs * bpp + 0] = 50;
      // canvas[ofs * bpp + 1] = 50;
      // canvas[ofs * bpp + 2] = 50;

      if (x < wal->xLeft || x > wal->xRight ||
          (x >= wal->mXLeft && x <= wal->mXRight)) {
        canvas[ofs * bpp + 0] = 100;
        canvas[ofs * bpp + 1] = 100;
        canvas[ofs * bpp + 2] = 100;
      }
    }
  }
}
