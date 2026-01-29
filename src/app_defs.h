#pragma once

typedef unsigned char byte;

// типы объектов
const byte OTYPE_NONE = 0, //
    OTYPE_ERING = 1,       // противник №1
    //OTYPE_EBOX = 2,        // противник №2
    //OTYPE_PWPN = 101,      // буст оружия
    //OTYPE_PSHLD = 102,     // буст защиты
    OTYPE_BNORM = 201     // снаряд нормальный
    //OTYPE_BPOWER = 202    // снаряд усиленный
;

// размер растра
#define PIXSZ (16)
#define PIXSZ2 (PIXSZ / 2)

// очки
#define SCORE_DISTANCE 1
#define SCORE_ENEMY 50

//
typedef struct {
  byte type;
  int x, y;
  // int light_val;
} objState;

typedef struct {
  byte r, g, b;
} col3;

typedef struct {
  byte r, g, b, a;
} col4;

//typedef col4 pal256[256];
typedef byte pal256[256][4];

typedef struct {
  int x, y;
  int val;
  byte r, g, b;
} lightState;

/*
               mXRigth
  xLeft   mxLeft  |    xRight
      V      V    V     V
  ----|      |----|     |----
*/
typedef struct {
  int y;
  int yDist;
  int xLeft;
  int xRight;
  int mXLeft;
  int mXRight;
  int index;
} walSect;

// размеры пулов объектов
#define PROJCNT (20)         // "выстрелы"
#define LITCNT (PROJCNT * 2) // источники света
#define WALCNT (20)          // секции стен
#define OBJCNT (WALCNT * 20) // противники и поверапы
