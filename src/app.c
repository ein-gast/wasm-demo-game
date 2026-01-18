#include "app_defs.h"
#include "app_levels.h"
#include "app_pixdata.h"
#include "app_statelogic.h"

void putPix(byte *canvas, int bpp, int toX, int toY, const byte pal[256][4],
            const byte pix[PIXSZ][PIXSZ + 1]);
void putProj(byte *canvas, int bpp, int toX, int toY);

void init(int w, int h) {
  state.win_w = w;
  state.win_h = h;
  for (int i = 0; i < OBJCNT; i++) {
    state.obj[i].type = OTYPE_NONE;
  }
  for (int i = 0; i < OBJCNT; i++) {
    state.projctl[i].type = OTYPE_NONE;
  }
  state.ammo = 0;
  state.shield = 0;
  state.pl_xdir = +1;
  state.pl_xdir = +1;
  state.vp_w = w;
  state.vp_h = h;
  state.vp_y = h;
  state.pl_x = w / 2;
  state.pl_y = state.vp_y - h + PIXSZ * 1.5;
  //
  unpakPal(pixShp16x16);
  unpakPal(pixRing16x16);
  level0(state.obj);
}

void input(int code1, int code2) { state.pl_xdir = -state.pl_xdir; }

void process(int t) {
  static unsigned int procFrame = 0;

  switch (state.pl_xdir) {
  case +1:
    if (state.pl_x < state.vp_w - PIXSZ2) {
      state.pl_x++;
    }
    break;
  case -1:
    if (state.pl_x > PIXSZ2) {
      state.pl_x--;
    }
    break;
  }

  for (int i = 0; i < OBJCNT; i++) {
    switch (state.obj[i].type) {
    case OTYPE_ERING:
      break;
    case OTYPE_EBOX:
      break;
    }
  }

  state.pl_y++;
  state.vp_y++;

  if (procFrame % 20 == 0) {
    objstate s = {.type = OTYPE_BNORM, .x = state.pl_x, .y = state.pl_y};
    place(state.projctl, PROJCNT, s);
  }
  procFrame++;
}

int render(int t, byte *input) {
  static unsigned int frame = 0;
  const int bytePerPixel = 4;

  // clear
  int i, cnt = state.win_w * state.win_h;
  for (i = 0; i < cnt; i++) {
    input[i * bytePerPixel + 0] = 10;
    input[i * bytePerPixel + 1] = 10;
    input[i * bytePerPixel + 2] = 10;
    input[i * bytePerPixel + 3] = 255;
  }

  // player
  int px = state.pl_x;
  int py = state.vp_y - state.pl_y;

  // putPix(input, w, h, bytePerPixel, px, py - PIXSZ * 2, pixRing16x16.pal,
  //        pixRing16x16.pix);
  putPix(input, bytePerPixel, px, py, pixShp16x16.pal, pixShp16x16.pix);

  // objects
  for (int i = 0; i < OBJCNT; i++) {
    switch (state.obj[i].type) {
    case OTYPE_ERING:
      o_ering(i);
      putPix(input, bytePerPixel, state.obj[i].x, state.vp_y - state.obj[i].y,
             pixRing16x16.pal, pixRing16x16.pix);
      break;
    case OTYPE_EBOX:
      o_ebox(i);
      break;
    }
  }
  int sum = 0;
  for (int i = 0; i < PROJCNT; i++) {
    switch (state.projctl[i].type) {
    case OTYPE_BNORM:
      o_prjctl_norm(i);
      putProj(input, bytePerPixel, state.projctl[i].x,
              state.vp_y - state.projctl[i].y);
      sum++;
      break;
    case OTYPE_BPOWER:
      o_prjctl_pwr(i);
      break;
    }
  }

  frame++;
  return sum;
}

void putPix(byte *canvas, int bpp, int toX, int toY, const byte pal[256][4],
            const byte pix[PIXSZ][PIXSZ + 1]) {
  int ofs;
  toX -= PIXSZ2;
  toY -= PIXSZ2;
  for (int py = 0; py < PIXSZ; py++) {
    if (toY + py < 0 || toY + py >= state.win_h) {
      continue;
    }
    for (int px = 0; px < PIXSZ; px++) {
      if (toX + px < 0 || toX + px >= state.win_w) {
        continue;
      }
      if (!pal[pix[py][px]][3]) {
        continue;
      }
      ofs = toX + px + (toY + py) * state.win_w;
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
  if (x1 >= 0 && x1 < state.win_w && y1 >= 0 && y1 < state.win_h) {
    ofs = x1 + y1 * state.win_w;
    canvas[ofs * bpp + 0] = 0;
    canvas[ofs * bpp + 1] = 255;
    canvas[ofs * bpp + 2] = 0;
  }
  if (x2 >= 0 && x2 < state.win_w && y2 >= 0 && y2 < state.win_h) {
    ofs = x2 + y2 * state.win_w;
    canvas[ofs * bpp + 0] = 0;
    canvas[ofs * bpp + 1] = 255;
    canvas[ofs * bpp + 2] = 0;
  }
}
