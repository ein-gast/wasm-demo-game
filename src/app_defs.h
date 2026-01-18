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
} objstate;

// размер пула объектов
#define OBJCNT (50)
#define PROJCNT (50)