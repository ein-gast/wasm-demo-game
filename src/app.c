#include "app_defs.h"
#include "app_levels.h"
#include "app_light.h"
#include "app_pixdata.h"
#include "app_statelogic.h"
#define _COMPILE_COLLISION_
#include "app_colision.h"
#define _COMPILE_WALLS_
#include "app_walls.h"
#define _COMPILE_RND_
#include "app_rnd.h"

void putPix(byte *canvas, int bpp, int toX, int toY, const byte pal[256][4],
            const byte pix[PIXSZ][PIXSZ + 1]);
void putProj(byte *canvas, int bpp, int toX, int toY);
void putWall(byte *canvas, int bpp, const walSect *wal);
void putFontNumber(byte *canvas, int bpp, int toX, int toY, int number);

// будет помещено в таблицу ипорта из-за -Wl,--inport-undefiled
/* extern */ void extLog(int ptr);

const char app_js[] = {
#ifdef APP_MIN_JS
#embed "app.min.js"
#else
#embed "app.js"
#endif
    , '\0' // null terminator
};

byte sum255(byte a, byte b) {
  int sum = a + b;
  return sum > 255 ? 255 : sum;
}

void init(int vpSize) {
  state.vpS = vpSize;
  for (int i = 0; i < OBJCNT; i++) {
    state.obj[i].type = OTYPE_NONE;
  }
  for (int i = 0; i < PROJCNT; i++) {
    state.projctl[i].type = OTYPE_NONE;
  }
  state.ammo = 0;
  state.shield = 0;
  state.plXDir = +1;
  state.plXDir = +1;
  state.vpY = vpSize;
  state.plX = vpSize / 2;
  state.plY = state.vpY - vpSize + PIXSZ * 2 / 3;
  state.gameover = false;
  state.score = 0;
  state.timer = -1;
  //
  unpakPal(pixFont3x5);
  unpakPal(pixShp16x16);
  //unpakPal(pixRing16x16);
  unpakPal(pixOppo16x16);
  level0(state.obj, state.vpS);
}

void input(int code1, int code2) {
  if (state.gameover && state.timer == 0) {
    init(state.vpS);
  }
  state.plXDir = -state.plXDir;
}

void process(int t) {
  static unsigned int procFrame = 0;
  // timer
  if (state.gameover && state.timer > 0) {
    state.timer--;
  }
  // viewport
  state.vpY++;
  // state.vpY=100;

  colReset();

  // walls
  const walSect *ws;
  int lastY = 0;
  ws = walFirst();
  while (ws != nullptr) {
    lastY = ws->y;
    if (ws->y + ws->yDist < state.vpY - state.vpS) {
      walTailForward();
      ws = walFirst();
      continue;
    }
    colWalAdd(ws, 1);
    ws = walNext(ws);
  }
  if (lastY < state.vpY) {
    // grow
    walSect newSect;
    createWal(&newSect, state.vpS, PIXSZ * 3);
    pushWal(&newSect, true);
  }

  // player
  if (!state.gameover) {
    if (state.vpY % state.vpS == 0) {
      state.score++;
    }
    switch (state.plXDir) {
    case +1:
      if (state.plX < state.vpS - PIXSZ2) {
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

    o_player();

    if (procFrame % 10 == 0) {
      objState s = {.type = OTYPE_BNORM, .x = state.plX, .y = state.plY};
      place(state.projctl, PROJCNT, s);
    }
  }

  // objects
  for (int i = 0; i < OBJCNT; i++) {
    switch (state.obj[i].type) {
    case OTYPE_ERING:
      o_ering(i);
      colObjAdd(state.obj + i, i);
      break;
      // case OTYPE_EBOX:
      //   o_ebox(i);
      //   colWalAdd(ws, 1);
      //   break;
    }
  }

  // projectiles
  for (int i = 0; i < PROJCNT; i++) {
    switch (state.projctl[i].type) {
    case OTYPE_BNORM:
      o_prjctl_norm(i);
      break;
      // case OTYPE_BPOWER:
      //   o_prjctl_pwr(i);
      //   break;
    }
  }

  procFrame++;
  // extLog(procFrame);
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
  int i = 0; //, cnt = state.vpS * state.vpS;
  col3 l = {10, 10, 10};
  if (state.gameover) {
    l.r = 120;
  }

  for (int y = 0; y < state.vpS; y++) {
    for (int x = 0; x < state.vpS; x++) {
      i = (x + y * state.vpS) * bytePerPixel;
      // l = litVal(x, state.vpY - y);
      input[i + 0] = l.r;
      input[i + 1] = l.g;
      input[i + 2] = l.b;
      input[i + 3] = 255;
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

  // apply light+shade
  for (int y = 0; y < state.vpS; y++) {
    for (int x = 0; x < state.vpS; x++) {
      i = (x + y * state.vpS) * bytePerPixel;
      l = litVal(x, state.vpY - y);
      input[i + 0] = sum255(l.r, input[i + 0]);
      input[i + 1] = sum255(l.g, input[i + 1]);
      input[i + 2] = sum255(l.b, input[i + 2]);
    }
  }

  // player
  if (!state.gameover) {
    int px = state.plX;
    int py = state.vpY - state.plY;
    putPix(input, bytePerPixel, px, py, pixShp16x16.pal, pixShp16x16.pix);
  }

  // objects
  for (int i = 0; i < OBJCNT; i++) {
    switch (state.obj[i].type) {
    case OTYPE_ERING:
      putPix(input, bytePerPixel, state.obj[i].x, state.vpY - state.obj[i].y,
             pixOppo16x16.pal, pixOppo16x16.pix);
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

  putFontNumber(input, bytePerPixel, state.vpS - 10, 5, state.score);

  frame++;
  return state.vpY;
}

void putPix(byte *canvas, int bpp, int toX, int toY, const byte pal[256][4],
            const byte pix[PIXSZ][PIXSZ + 1]) {
  int ofs;
  toX -= PIXSZ2;
  toY -= PIXSZ2;
  for (int py = 0; py < PIXSZ; py++) {
    if (toY + py < 0 || toY + py >= state.vpS) {
      continue;
    }
    for (int px = 0; px < PIXSZ; px++) {
      if (toX + px < 0 || toX + px >= state.vpS) {
        continue;
      }
      if (!pal[pix[py][px]][3]) {
        continue;
      }
      ofs = toX + px + (toY + py) * state.vpS;
      ofs *= bpp;
      canvas[ofs + 0] = pal[pix[py][px]][0];
      canvas[ofs + 1] = pal[pix[py][px]][1];
      canvas[ofs + 2] = pal[pix[py][px]][2];
      // canvas[ofs*bpp+3] = pal[pix[py][px]][3];
    }
  }
}

void putProj(byte *canvas, int bpp, int toX, int toY) {
  int x1 = toX - PIXSZ2 + 2, y1 = toY;
  int x2 = toX + PIXSZ2 - 2, y2 = toY;
  int ofs;
  if (x1 >= 0 && x1 < state.vpS && y1 >= 0 && y1 < state.vpS) {
    ofs = x1 + y1 * state.vpS;
    ofs *= bpp;
    canvas[ofs + 0] = 0;
    canvas[ofs + 1] = 255;
    canvas[ofs + 2] = 0;
  }
  if (x2 >= 0 && x2 < state.vpS && y2 >= 0 && y2 < state.vpS) {
    ofs = x2 + y2 * state.vpS;
    ofs *= bpp;
    canvas[ofs + 0] = 0;
    canvas[ofs + 1] = 255;
    canvas[ofs + 2] = 0;
  }
}

void putWall(byte *canvas, int bpp, const walSect *wal) {
  int y1 = state.vpY - wal->y - wal->yDist;
  int y2 = y1 + wal->yDist;
  if ((y1 > state.vpS && y2 > state.vpS) || (y1 < 0 && y2 < 0)) {
    return;
  }

  int ofs, x, y;
  for (y = y1; y < y2; y++) {
    if (y >= state.vpS || y < 0) {
      continue;
    }

    for (x = 0; x < state.vpS; x++) {
      ofs = x + y * state.vpS;
      ofs *= bpp;
      // canvas[ofs * bpp + 0] = 50;
      // canvas[ofs * bpp + 1] = 50;
      // canvas[ofs * bpp + 2] = 50;

      if (x < wal->xLeft || x > wal->xRight ||
          (x >= wal->mXLeft && x <= wal->mXRight)) {
        canvas[ofs + 0] = 100;
        canvas[ofs + 1] = 100;
        canvas[ofs + 2] = 100;
      }
    }
  }
}

void putFontNumber(byte *canvas, int bpp, int toX, int toY, int number) {
  auto pal = pixFont3x5.pal;
  auto pix = pixFont3x5.pix;
  int ofs, digitX, rangeX = 0;

  do {
    digitX = (number % 10) * 3;

    for (int py = 0; py < 5; py++) {
      if (toY + py < 0 || toY + py >= state.vpS) {
        continue;
      }
      for (int px = 0; px < 3; px++) {
        if (toX + px < 0 || toX + px >= state.vpS) {
          continue;
        }
        if (!pal[pix[py][digitX + px]][3]) {
          continue;
        }
        ofs = toX - rangeX + px + (toY + py) * state.vpS;
        ofs *= bpp;
        canvas[ofs + 0] = pal[pix[py][digitX + px]][0];
        canvas[ofs + 1] = pal[pix[py][digitX + px]][1];
        canvas[ofs + 2] = pal[pix[py][digitX + px]][2];
      }
    }

    number = number / 10;
    rangeX += 3 + 1;
  } while (number > 0);
}