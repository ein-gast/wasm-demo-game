#include "app_defs.h"

void drawPixel(byte *canvas, int canvasSZ, int toX, int toY, col3 color) {
  int ofs = toX + toY * canvasSZ;
  ofs *= BPP;
  canvas[ofs + 0] = color.r;
  canvas[ofs + 1] = color.g;
  canvas[ofs + 2] = color.b;
  canvas[ofs + 3] = 255;
}

void drawBox(byte *canvas, int canvasSZ, int toX, int toY, int W, int H,
             col3 color) {
  W /= 2;
  H /= 2;
  int leftTopX = max(0, toX - W), leftTopY = max(0, toY - W),
      rightBotX = min(canvasSZ, toX), rightBotY = min(canvasSZ, toY);
  int px, py;

  for (py = leftTopY; py < rightBotY; py++) {
    for (px = leftTopX; px < rightBotX; px++) {
      drawPixel(canvas, canvasSZ, px, py, color);
    }
  }
}
