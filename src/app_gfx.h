#include "app_defs.h"

void drawPixel(byte *canvas, int canvasSZ, int toX, int toY, col4 color) {
  if (color.a != 0 && toX > 0 && toY > 0 && toX <= canvasSZ &&
      toY <= canvasSZ) {
    int ofs = toX + toY * canvasSZ;
    ofs *= BPP;
    canvas[ofs + 0] = color.r;
    canvas[ofs + 1] = color.g;
    canvas[ofs + 2] = color.b;
    //canvas[ofs + 3] = 255;
  }
}

void drawBox(byte *canvas, int canvasSZ, int toX, int toY, int W, int H,
             col4 color) {
  W /= 2;
  H /= 2;
  int px, py;

  px = toX - W;
  py = toY - H;
  int leftTopX = max(0, px); // ?
  int leftTopY = max(0, py); // ?

  px = toX + W;
  py = toY + H;
  int rightBotX = min(canvasSZ, px); // ?
  int rightBotY = min(canvasSZ, py); // ?

  for (py = leftTopY; py < rightBotY; py++) {
    for (px = leftTopX; px < rightBotX; px++) {
      drawPixel(canvas, canvasSZ, px, py, color);
    }
  }
}

void drawTriangle(byte *canvas, int canvasSZ, int toX, int toY, int W, int dir,
                  col4 color) {
  W /= 2;
  int px, py;

  px = toX - W;
  int leftTopX = max(0, px);
  px = toX + W;
  int rightBotX = min(canvasSZ, px);

  int leftTopY = toY;
  int rightBotY = toY + dir * W;

  for (py = leftTopY; py != rightBotY; py += dir) {
    for (px = leftTopX; px < rightBotX; px++) {
      drawPixel(canvas, canvasSZ, px, py, color);
    }
    leftTopX++;
    rightBotX--;
  }
}

void drawXOppo(byte *canvas, int canvasSZ, int toX, int toY) {
  const col4 color1 = {0x9F, 0x21, 0xC0, 0xFF};
  const col4 color2 = {0xFF, 0xFF, 0xFF, 0xFF};
  drawTriangle(canvas, canvasSZ, toX, toY + 1, PIXSZ, +1, color1);
  drawTriangle(canvas, canvasSZ, toX, toY, PIXSZ, -1, color1);
  drawBox(canvas, canvasSZ, toX, toY + 1, PIXSZ - 3, PIXSZ - 3, color1);

  drawBox(canvas, canvasSZ, toX, toY + 1, PIXSZ - 7, PIXSZ - 7, color2);
  drawBox(canvas, canvasSZ, toX, toY + 1, PIXSZ - 9, PIXSZ - 9, color1);

  drawBox(canvas, canvasSZ, toX - 4, toY + 1, PIXSZ2 / 2, 2, color2);
  drawBox(canvas, canvasSZ, toX + 4, toY + 1, PIXSZ2 / 2, 2, color2);

  drawBox(canvas, canvasSZ, toX, toY - 3, 2, PIXSZ2 / 2, color2);
  drawBox(canvas, canvasSZ, toX, toY + 5, 2, PIXSZ2 / 2, color2);
}

void drawXPlayer(byte *canvas, int canvasSZ, int toX, int toY) {
  const col4 color1 = {0x4B, 0x5B, 0xAB, 0xFF};
  const col4 color2 = {0xFF, 0xFF, 0xFF, 0xFF};

  drawTriangle(canvas, canvasSZ, toX, toY + 1, PIXSZ-3, +1, color1);
  drawTriangle(canvas, canvasSZ, toX, toY, PIXSZ-3, -1, color1);

  drawTriangle(canvas, canvasSZ, toX, toY + 2, 6, -1, color2);

  drawBox(canvas, canvasSZ, toX -6, toY+1, 2, PIXSZ-4, color2);
  drawBox(canvas, canvasSZ, toX +6, toY+1, 2, PIXSZ-4, color2);
}