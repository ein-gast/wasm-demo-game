float fRand();
int iRand(int low, int high);

#ifdef _COMPILE_RND_
// hello, wikipedia
const int _a = 1103515245;
const int _c = 12345;
const int _m = 0xFFFF;

float fRand() {
  static unsigned long int last = 1;
  last = (last * 1103515245 + 12345) % _m;
  return (float)last / _m;
}

int iRand(int low, int high) { return low + (int)(high - low) * fRand(); }
#endif