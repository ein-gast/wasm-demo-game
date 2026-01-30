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
