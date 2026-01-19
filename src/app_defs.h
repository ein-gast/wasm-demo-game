#pragma once

typedef unsigned char byte;

// типы объектов
const byte OTYPE_NONE = 0, OTYPE_ERING = 1, OTYPE_EBOX = 2, OTYPE_PWPN = 101,
           OTYPE_PSHLD = 102, OTYPE_BNORM = 201, OTYPE_BPOWER = 202;

// размер растра
#define PIXSZ (16)
#define PIXSZ2 (PIXSZ / 2)

//
typedef struct {
  byte type;
  int x, y;
  // int light_val;
} objState;

typedef struct {byte r, g, b;} col3;

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
#define OBJCNT (20) // противники и поверапы
#define PROJCNT (20) // "выстрелы"
#define LITCNT (20) // источники света
#define WALCNT (20) // секции стен