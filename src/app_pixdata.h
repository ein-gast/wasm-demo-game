#include "app_defs.h"

pal256 globalPal;

#define unpakPal(raster)                                                       \
  {                                                                            \
    unpakPalArray(raster.mapSize, raster.palMap, globalPal);                   \
  }

struct {
  //pal256 pal;
  const int mapSize;
  const byte palMap[3][5];
  const byte pix[PIXSZ][PIXSZ + 1];
} pixShp16x16 = {
    .mapSize = 3,
    .palMap =
        {// code -> r,g,b,a
         {' ', 0x4B, 0x5B, 0xAB, 0xFF},
         {'.', 0xFF, 0xFF, 0xFF, 0xFF},
         {'X', 0, 0, 0, 0}},
    .pix = {"XXXXXXXXXXXXXXXX", "XXXXXXXXXXXXXXXX", "XXXXXXXXXXXXXXXX",
            "XXXXXXXXXXXXXXXX", "XXXXXXXXXXXXXXXX", "XXXXXXXXXXXXXXXX",
            "XXXXXXXXXXXXXXXX", "XXXXXXXXXXXXXXXX", "XX.XXXX  XXXX.XX",
            "XX..XXX  XXX..XX", "XX. XX    XX .XX", "XX..XX    XX..XX",
            "XX. X      X .XX", "XX..X      X..XX", "XX.    XX    .XX",
            "XX.XXXXXXXXXX.XX"}};

struct {
  //pal256 pal;
  const int mapSize;
  const byte palMap[3][5];
  const byte pix[PIXSZ][PIXSZ + 1];
} pixOppo16x16 = {
    .mapSize = 3,
    .palMap =
        {// code -> r,g,b,a  #9F21C0
         {'_', 0x9F, 0x21, 0xC0, 0xFF},
         {'.', 0xFF, 0xFF, 0xFF, 0xFF},
         {'X', 0, 0, 0, 0}},
    .pix = {
"XXXXXXXXXXXXXXXX",
"XXXXX_____XXXXXX",
"XXX_________XXXX",
"XX_____._____XXX",
"X______.______XX",
"X____.....____XX",
"____.__.__.____X",
"____._____.____X",
"__....___....__X",
"____._____.____X",
"____.__.__.____X",
"X____.....____XX",
"X______.______XX",
"XX_____._____XXX",
"XXX_________XXXX",
"XXXXX_____XXXXXX"
}

};

/*
struct {
  //byte pal[256][4];
  const int mapSize;
  const byte palMap[2][5];
  const byte pix[PIXSZ][PIXSZ + 1];
} pixRing16x16 = {
    .mapSize = 2,
    .palMap =
        {// code -> r,g,b,a
         {' ', 0xF2, 0xA6, 0x5E, 0xFF},
         {'X', 0, 0, 0, 0}},
    .pix = {"XXXXXX    XXXXXX", "XXXX        XXXX", "XXX   XXXX   XXX",
            "XX   XXXXXX   XX", "XX  XXXXXXXX  XX", "X  XXXXXXXXXX  X",
            "X  XXXXXXXXXX  X", "  XXXXXXXXXXXX  ", "  XXXXXXXXXXXX  ",
            "  XXXXXXXXXXXX  ", "  XXXXXXXXXXXX  ", "X  XXXXXXXXXX  X",
            "X   XXXXXXXX   X", "XX    XXXX    XX", "XXXX        XXXX",
            "XXXXXX    XXXXXX"}};
*/

struct {
  //pal256 pal;
  const int mapSize;
  const byte palMap[2][5];
  const byte pix[5][30 + 1];
} pixFont3x5 = {
    .mapSize = 2,
    .palMap = {{'.', 0xFF, 0xFF, 0xFF, 0xFF}, {'X', 0, 0, 0, 0x00}},
    .pix = {"...X.........X................", ".X.XX.XX.XX..X..XX.XXXX..X..X.",
            ".X.XX....X...........X.X......", ".X.XX..XXXX.XX.XX..X..XX.X.XX.",
            "...XX.......XX........XX......"}};

/*
struct {
  //pal256 pal;
  const int mapSize;
  const byte palMap[2][5];
  const byte pix[50][3+1];
} pixVFont3x5 = {
    .mapSize = 2,
    .palMap = {{' ', 0xFF, 0xFF, 0xFF, 0xFF}, {'X', 0, 0, 0, 0x00}},
    .pix = {"XXX", "X X", "X X", "X X", "XXX", " XX", "  X", "  X", "  X",
            "  X", "XXX", "  X", "XXX", "X  ", "XXX", "XXX", "  X", " XX",
            "  X", "XXX", "X X", "X X", "XXX", "  X", "  X", "XXX", "X  ",
            "XXX", "  X", "XXX", "XXX", "X  ", "XXX", "X X", "XXX", "XXX",
            "  X", " X ", "X  ", "X  ", "XXX", "X X", "XXX", "X X", "XXX",
            "XXX", "X X", "XXX", "  X", "XXX"}};
*/

// распаковывает палитру изображения
void unpakPalArray(int mapSize, const byte palMap[][5], pal256 pal) {
  for (int i = 0; i < mapSize; i++) {
    pal[palMap[i][0]][0] = palMap[i][1];
    pal[palMap[i][0]][1] = palMap[i][2];
    pal[palMap[i][0]][2] = palMap[i][3];
    pal[palMap[i][0]][3] = palMap[i][4];
  }
}

// распаковывает палитру изображения
