typedef unsigned char byte;

int add(int first, int second) { return first + second; }

int fillBuff(int t, byte r, byte g, byte b, byte *input, int w, int h) {
  static int frame = 0;
  const int bytePerPixel = 4;
  int i, cnt = w * h;
  for (i = 0; i < cnt; i++) {
    if(i%2 == 0 ) {continue;}
    input[i * bytePerPixel + 0] = (frame) % 255;
    input[i * bytePerPixel + 1] = 0;
    input[i * bytePerPixel + 2] = 0;
    input[i * bytePerPixel + 3] = 255;
  }
  return frame++;
}
