#include "app_defs.h"

pal256 globalPal;

#define unpakPal(raster)                                                       \
  {                                                                            \
    unpakPalArray(raster.mapSize, raster.palMap, globalPal);                   \
  }

struct {
  // pal256 pal;
  const int mapSize;
  const byte palMap[3][5];
  const byte pix[PIXSZ2][PIXSZ2 + 1];
} pixShp16x16 = {.mapSize = 3,
                 .palMap =
                     {// code -> r,g,b,a
                      {' ', 0x4B, 0x5B, 0xAB, 0xFF},
                      {'.', 0xFF, 0xFF, 0xFF, 0xFF},
                      {'A', 0, 0, 0, 0}},
                 .pix = {
                    "AAAAAAAA",
                    "A.AAAAA ",
                    "A..AAAA ",
                    "A. AAA  ",
                    "A..AAA  ",
                    "A. AA   ",
                    "A..AA   ",
                    "A.     .",
                 }};

struct {
  // pal256 pal;
  const int mapSize;
  const byte palMap[3][5];
  const byte pix[PIXSZ2][PIXSZ2 + 1];
} pixOppo16x16 = {.mapSize = 3,
                  .palMap =
                      {// code -> r,g,b,a  #9F21C0
                       {'_', 0x9F, 0x21, 0xC0, 0xFF},
                       {'.', 0xFF, 0xFF, 0xFF, 0xFF},
                       {'A', 0, 0, 0, 0}},
                  .pix = {
                  "AAAAA___", 
                  "AAA_____", 
                  "AA_____.", 
                  "A______.",
                  "A____...", 
                  "____.__.", 
                  "____.___", 
                  "__....__",
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
  // pal256 pal;
  const int mapSize;
  const byte palMap[2][5];
  const byte pix[5][30 + 1];
} pixFont3x5 = {
    .mapSize = 2,
    .palMap = {{'.', 0xFF, 0xFF, 0xFF, 0xFF}, {'A', 0, 0, 0, 0x00}},
    .pix = {"...A.........A................", ".A.AA.AA.AA..A..AA.AAAA..A..A.",
            ".A.AA....A...........A.A......", ".A.AA..AAAA.AA.AA..A..AA.A.AA.",
            "...AA.......AA........AA......"}};

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
  int colorIndex;
  for (int i = 0; i < mapSize; i++) {
    colorIndex = palMap[i][0];
    pal[colorIndex][0] = palMap[i][1];
    pal[colorIndex][1] = palMap[i][2];
    pal[colorIndex][2] = palMap[i][3];
    pal[colorIndex][3] = palMap[i][4];
  }
}

// распаковывает палитру изображения
