#ifndef PAIR_HPP
#define PAIR_HPP

class Pair {
  int x;
  int y;

public:
  Pair(int px, int py){x = px; y = py;};
  int getX() {return x;};
  int getY() {return y;};
};

#endif
