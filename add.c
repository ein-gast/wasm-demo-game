typedef unsigned char byte;

struct {
  int horizon;
} state;

void init() { state.horizon = 50; }

void input(int code1, int code2) {
  if (state.horizon > 0) {
    state.horizon--;
  }
}

int render(int t, byte r, byte g, byte b, byte *input, int w, int h) {
  static int frame = 0;
  const int bytePerPixel = 4;
  int i, cnt = w * h;
  for (i = 0; i < cnt; i++) {
    if (i % 2 == 0) {
      continue;
    }
    if (i >= cnt - state.horizon * w) {
      input[i * bytePerPixel + 0] = (frame) % 255;
      input[i * bytePerPixel + 1] = 0;
      input[i * bytePerPixel + 2] = 0;
      input[i * bytePerPixel + 3] = 255;
    } else {
      input[i * bytePerPixel + 0] = 0;
      input[i * bytePerPixel + 1] = 0;
      input[i * bytePerPixel + 2] = 0;
      input[i * bytePerPixel + 3] = 0;
    }
  }
  if (frame % 20 == 0 && state.horizon < 50) {
    state.horizon++;
  }
  frame++;
  return frame;
}
