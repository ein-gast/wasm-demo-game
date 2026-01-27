#include "app_colision.h"
#include "app_defs.h"
#include <stdbool.h>

struct {
  // viewport size and pos
  int vpY, vpS;
  // player
  int plY, plX, plXDir;
  // powerups
  int ammo, shield;
  // game over flag
  bool gameover;
  int timer;
  // score
  int score;

  // scene objects
  objState obj[OBJCNT];
  objState projctl[PROJCNT];
} state;

void do_gameover() {
  state.timer = 60 * 2;
  state.gameover = true;
}

void o_player() {
  int col = collWithObj(state.plX, state.plY);
  if (col >= 0) {
    state.obj[col].type = OTYPE_NONE;
    do_gameover();
    return;
  }
  col = collWithWall(state.plX, state.plY, PIXSZ, PIXSZ2);
  if (col >= 0) {
    do_gameover();
    return;
  }
}

void o_ering(int i) {
  if (state.obj[i].y < state.vpY - state.vpS - PIXSZ) {
    state.obj[i].type = OTYPE_NONE;
    return;
  }
}

void o_ebox(int i) {
  if (state.obj[i].y < state.vpY - state.vpS - PIXSZ) {
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

  int col = collWithObj(state.projctl[i].x, state.projctl[i].y);
  if (col >= 0) {
    state.obj[col].type = OTYPE_NONE;
    state.projctl[i].type = OTYPE_NONE;
    state.score += 5;
    return;
  }

  col = collWithWall(state.projctl[i].x, state.projctl[i].y, PIXSZ, PIXSZ2 / 2);
  if (col >= 0) {
    state.projctl[i].type = OTYPE_NONE;
    return;
  }
}

// void o_prjctl_pwr(int i) {
//   for (int k = 0; k < 3; k++) {
//     state.projctl[i].y++;
//     if (state.projctl[i].y > state.vpY) {
//         state.projctl[i].type = OTYPE_NONE;
//         return;
//     }
//   }
// }