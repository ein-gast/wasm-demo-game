#include "app_defs.h"
// objstate obj[OBJCNT];
// objstate projectl[PROJCNT];

struct {
  // screen=window size
  int win_w, win_h;
  // viewport
  int vp_y, vp_w, vp_h;
  // player
  int pl_y, pl_x, pl_xdir;
  // powerups
  int ammo, shield;

  // scene objects
  objstate obj[OBJCNT];
  objstate projctl[PROJCNT];
} state;

void o_ering(int i) {}

void o_ebox(int i) {}

void o_prjctl_norm(int i) {
  for (int k = 0; k < 3; k++) {
    state.projctl[i].y++;
    if (state.projctl[i].y > state.vp_y) {
        state.projctl[i].type = OTYPE_NONE;
        return;
    }
  }
}

void o_prjctl_pwr(int i) {
  for (int k = 0; k < 3; k++) {
    state.projctl[i].y++;
    if (state.projctl[i].y > state.vp_y) {
        state.projctl[i].type = OTYPE_NONE;
        return;
    }
  }
}