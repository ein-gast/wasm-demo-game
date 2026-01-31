#include "app_defs.h"
#include "app_levels.h"
#ifdef WITH_LIGHT
#include "app_light.h"
#endif
// #include "app_gfx.h"
#include "app_pixdata.h"
#include "app_statelogic.h"
#define _COMPILE_COLLISION_
#include "app_colision.h"
#define _COMPILE_WALLS_
#include "app_walls.h"
#define _COMPILE_RND_
#include "app_rnd.h"

#define init() I()
#define keyInput() K()
#define process() P()
#define renderFrame(input) R(input)

void putBitmap(byte *canvas, int toX, int toY, bool vMirror,
               const byte pix[][PIXSZ2 + 1]);
void putProj(byte *canvas, int toX, int toY);
void putWall(byte *canvas, const walSect *wal);
void putFontNumber(byte *canvas, int toX, int toY, int number);

// будет помещено в таблицу ипорта из-за -Wl,--inport-undefiled
// /* extern */ void extLog(int ptr);

const char js[] = {
#ifdef APP_MIN_JS
#embed "boot.min.js"
#else
#embed "boot.js"
#endif
    , '\0' // null terminator
};

const col4 colBgGamePlay = {10, 10, 10, 255};
const col4 colBgGameOver = {120, 10, 10, 255};
const col4 colWall = {100, 100, 100, 255};
const col4 colProjectile = {0, 255, 0, 255};

#ifdef WITH_LIGHT
byte sum255(byte a, byte b) {
  int sum = a + b;
  return sum > 255 ? 255 : sum;
}
#endif

void drawPixel4(byte *canvas, int toX, int toY, const col4 *col) {
  if (col->v[3] != 0 && toX > 0 && toY > 0 && toX <= CANVASZS &&
      toY <= CANVASZS) {
    int ofs = toX + toY * CANVASZS;
    ofs *= BPP;
    canvas[ofs + 0] = col->v[0];
    canvas[ofs + 1] = col->v[1];
    canvas[ofs + 2] = col->v[2];
    canvas[ofs + 3] = 255;
  }
}

/* I() = init() */
void I() {
  for (int i = 0; i < OBJCNT; i++) {
    state.obj[i].type = OTYPE_NONE;
  }
  for (int i = 0; i < PROJCNT; i++) {
    state.projctl[i].type = OTYPE_NONE;
  }

  state.plXDir = +1;
  // state.plXDir = 0;
  state.vpY = CANVASZS;
  state.plX = CANVASZS / 2;
  state.plY = state.vpY - CANVASZS + PIXSZ * 2 / 3;
  state.gameover = false;
  state.score = 0;
  state.timer = -1;
  //
  unpakPal(pixFont3x5);
  // unpakPal(pixVFont3x5);
  unpakPal(pixShp16x16);
  // unpakPal(pixRing16x16);
  unpakPal(pixOppo16x16);
  level0(state.obj, CANVASZS);
}

/* K() = keyInput() */
void K(/*int code1, int code2*/) {
  if (state.gameover && state.timer == 0) {
    init();
  }
  state.plXDir = -state.plXDir;
}

/* P() = process() */
void P() {
  static unsigned int procFrame = 0;

  // timer
  if (state.gameover && state.timer > 0) {
    state.timer--;
  }

  // viewport
  state.vpY++;

  colReset();

  // walls
  const walSect *ws;
  int lastY = 0;
  ws = walFirst();
  while (ws != nullptr) {
    lastY = ws->y;
    if (ws->y + ws->yDist < state.vpY - CANVASZS) {
      walTailForward();
      ws = walFirst();
      continue;
    }
    colWalAdd(ws, 1);
    ws = walNext(ws);
  }
  if (lastY < state.vpY) {
    // grow up
    createSection(state.obj, CANVASZS);
  }

  // player
  if (!state.gameover) {
    if (state.vpY % CANVASZS == 0) {
      state.score += SCORE_DISTANCE;
    }
    switch (state.plXDir) {
    case +1:
      if (state.plX < CANVASZS - PIXSZ2) {
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
      objState *s;
      if (nullptr != (s = placePtr(state.projctl, PROJCNT))) {
        s->type = OTYPE_BNORM;
        s->x = state.plX;
        s->y = state.plY;
      }
    }
  }

  // objects
  for (int i = 0; i < OBJCNT; i++) {
    if (state.obj[i].type == OTYPE_ERING) {
      o_ering(i);
      colObjAdd(state.obj + i, i);
    }
    /*
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
    */
  }

  // projectiles
  for (int i = 0; i < PROJCNT; i++) {
    if (state.projctl[i].type == OTYPE_BNORM) {
      o_prjctl_norm(i);
    }
    /*
    switch (state.projctl[i].type) {
    case OTYPE_BNORM:
      o_prjctl_norm(i);
      break;
      // case OTYPE_BPOWER:
      //   o_prjctl_pwr(i);
      //   break;
    }
    */
  }

  procFrame++;
  // extLog(procFrame);
}

/* R(input) = renderFrame(input) */
void R(byte *input) {
  static unsigned int frame = 0;

#ifdef WITH_LIGHT
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
#endif

  int i = 0; //, cnt = state.vpS * state.vpS;
  const col4 *clearColor = &colBgGamePlay;
  if (state.gameover) {
    clearColor = &colBgGameOver;
  }

  // clear - optimize?
  for (int y = 0; y < CANVASZS; y++) {
    for (int x = 0; x < CANVASZS; x++) {
      i = (x + y * CANVASZS) * BPP;
      drawPixel4(input, x, y, clearColor);
    }
  }

  // walls + clear
  int sum = 0;
  const walSect *ws;
  ws = walFirst();
  while (ws != nullptr) {
    if (ws->y > state.vpY) {
      break;
    }
    sum++;
    putWall(input, ws);
    ws = walNext(ws);
  }

#ifdef WITH_LIGHT
  // apply light+shade
  for (int y = 0; y < state.vpS; y++) {
    for (int x = 0; x < state.vpS; x++) {
      i = (x + y * state.vpS) * BPP;
      clearColor = litVal(x, state.vpY - y); // var reuse
      input[i + 0] = sum255(clearColor.r, input[i + 0]);
      input[i + 1] = sum255(clearColor.g, input[i + 1]);
      input[i + 2] = sum255(clearColor.b, input[i + 2]);
    }
  }
#endif

  // player
  if (!state.gameover) {
    int px = state.plX;
    int py = state.vpY - state.plY;
    putBitmap(input, px, py, true, pixShp16x16.pix);
    // drawXPlayer(input, CANVASZS, px, py);
  }

  // objectsstate.vpS / 2, state.vpS / 2
  for (int i = 0; i < OBJCNT; i++) {
    switch (state.obj[i].type) {
    case OTYPE_ERING:
      putBitmap(input, state.obj[i].x, state.vpY - state.obj[i].y, true,
                pixOppo16x16.pix);
      // drawXOppo(input, CANVASZS, state.obj[i].x, state.vpY -
      // state.obj[i].y);
      break;
      // case OTYPE_EBOX:
      //   break;
    }
  }
  for (int i = 0; i < PROJCNT; i++) {
    switch (state.projctl[i].type) {
    case OTYPE_BNORM:
      putProj(input, state.projctl[i].x, state.vpY - state.projctl[i].y);
      break;
      // case OTYPE_BPOWER:
      //   break;
    }
  }

  putFontNumber(input, CANVASZS - 10, 5, state.score);

  frame++;
}

void putBitmap(byte *canvas, int toX, int toY, bool vMirror,
               const byte pix[][PIXSZ2 + 1]) {
  int fx, fy;
  col4 *col;
  toX -= PIXSZ2;
  toY -= PIXSZ2;
  for (int py = 0; py < PIXSZ; py++) {
    if (vMirror) {
      fy = py < PIXSZ2 ? py : PIXSZ - py - 1;
    } else {
      fy = py;
    }
    for (int px = 0; px < PIXSZ; px++) {
      fx = px < PIXSZ2 ? px : PIXSZ - px - 1;
      col = globalPal + pix[fy][fx];
      drawPixel4(canvas, toX + px, toY + py, col);
    }
  }
}

void putFontNumber(byte *canvas, int toX, int toY, int number) {
  // auto pal = pixFont3x5.pal;
  auto pix = pixFont3x5.pix;
  int digitX, rangeX = 0;
  col4 *col;
  do {
    digitX = (number % 10) * 3;

    for (int py = 0; py < 5; py++) {
      for (int px = 0; px < 3; px++) {
        col = globalPal + pix[py][digitX + px];
        drawPixel4(canvas, toX - rangeX + px, toY + py, col);
      }
    }

    number = number / 10;
    rangeX += 3 + 1;
  } while (number > 0);
}

void putProj(byte *canvas, int toVpX, int toVpY) {
  int x1 = toVpX - PIXSZ2 + 2, y1 = toVpY;
  int x2 = toVpX + PIXSZ2 - 2, y2 = toVpY;

  drawPixel4(canvas, x1, y1, &colProjectile);
  drawPixel4(canvas, x2, y2, &colProjectile);
}

void putWall(byte *canvas, const walSect *wal) {
  int y1 = state.vpY - wal->y - wal->yDist;
  int y2 = y1 + wal->yDist;
  if ((y1 > CANVASZS && y2 > CANVASZS) || (y1 < 0 && y2 < 0)) {
    return;
  }

  int x, y;
  for (y = y1; y < y2; y++) {
    if (y >= CANVASZS || y < 0) {
      continue;
    }

    for (x = 0; x < CANVASZS; x++) {
      if (x < wal->xLeft || x > wal->xRight ||
          (x >= wal->mXLeft && x <= wal->mXRight)) {
        drawPixel4(canvas, x, y, &colWall);
      }
    }
  }
}
