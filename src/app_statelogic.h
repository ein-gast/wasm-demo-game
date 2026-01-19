#include "app_defs.h"

struct {
  // screen=window size
  int winW, winH;
  // viewport
  int vpY, vpW, vpH;
  // player
  int plY, plX, plXDir;
  // powerups
  int ammo, shield;

  // scene objects
  objState obj[OBJCNT];
  objState projctl[PROJCNT];
} state;

void o_ering(int i) {
    if (state.obj[i].y < state.vpY - state.winH - PIXSZ) {
        state.obj[i].type = OTYPE_NONE;
        return;
    }
}

void o_ebox(int i) {
    if (state.obj[i].y < state.vpY - state.winH - PIXSZ) {
        state.obj[i].type = OTYPE_NONE;
        return;
    }
}

void o_prjctl_norm(int i) {
  for (int k = 0; k < 3; k++) {
    state.projctl[i].y++;
    if (state.projctl[i].y > state.vpY) {
        state.projctl[i].type = OTYPE_NONE;
        return;
    }
  }
}

void o_prjctl_pwr(int i) {
  for (int k = 0; k < 3; k++) {
    state.projctl[i].y++;
    if (state.projctl[i].y > state.vpY) {
        state.projctl[i].type = OTYPE_NONE;
        return;
    }
  }
}