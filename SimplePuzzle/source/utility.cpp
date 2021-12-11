#include <random>

#include "../include/utility.h"

bool Point::operator==(const Point& p) const noexcept {
  return x == p.x && y == p.y;
}
Point& Point::operator=(const Point& p) {
  x = p.x;
  y = p.y;
  return *this;
}
bool Point::InRange(int val, int leftBound, int rightBound) {
  return val >= leftBound && val < rightBound;
}
bool Point::InBoundary(const Point& p) const noexcept {
  return Point::InBoundary({0, 0}, p);
}
bool Point::InBoundary(const Point& LTp, const Point& RBp) const noexcept {
  return InRange(x, LTp.x, RBp.x) && InRange(y, LTp.y, RBp.y);
}

namespace Random {
static std::random_device seed;
static std::default_random_engine random_engine(seed());
static std::uniform_int_distribution<> distribution;
static std::normal_distribution<> normal_dist;
using NDT=std::normal_distribution<>::param_type;
char GetRandomUppercase(bool normal) noexcept { return GetRandom('A', 'Z',normal); }
char GetRandomLowercase(bool normal) noexcept { return GetRandom('a', 'z',normal); }
int GetRandom(int max, bool normal) noexcept { return GetRandom(0, max, normal); }
int GetRandom(int min, int max, bool normal) noexcept {
  return normal ? normal_dist(random_engine,
                              NDT((max + min) >> 1, (max - min) >> 2))
                : distribution(random_engine, max - min) + min;
}
char GetRandomAscii(int pos,bool normal) noexcept {
  int tmp = normal? 
      normal_dist(random_engine, NDT(pos,pos>>3)) : 
      distribution(random_engine, 36);
  if (tmp > 9) {
    return tmp - 10 + 'A';
  } else {
    return tmp + '0';
  }
}
char GetRandomAscii() noexcept { return GetRandomAscii(0,false); }
char GetRandomAscii(int pos) noexcept { return GetRandomAscii(pos,true); }
Point GetPoint(Point const& corner) noexcept {
  return Point(GetRandom(0, corner.x), GetRandom(0, corner.y));
}
Direction GetDirection() noexcept {
  return Direction(1 << distribution(random_engine, 4));
}
}  // namespace Random