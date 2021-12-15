#define DEBUG

#include <graphics.h>
#include <random>

#include "../include/utility.h"

#pragma region Point Imp
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

constexpr static double Sqrt3 = 1.732;
bool In(Point const& p, Rect const& region) {
  return (p.x >= region.origin.x && p.x < region.origin.x + region.width) &&
         (p.y >= region.origin.y && p.y < region.origin.y + region.height);
}

bool Point::In() const noexcept { return x >= 0 && y >= 0; }
bool Point::In(Rect const& region) const noexcept {
  return ::In(*this, region);
}

#pragma endregion

#pragma region Rect Imp
Rect::Rect(int x, int y, int wid, int heig)
    : origin{x, y}, width(wid), height(heig) {}
int Rect::block(Point const& p, int wb, int hb) {
  if (p.In(*this)) {
    int x = p.x - origin.x, y = p.y - origin.y;
    int xblk = width / wb, yblk = height / hb;
    if (x % xblk == 0 || y % yblk == 0) {
      return -1;
    } else {
      return y / yblk * wb + x / xblk;
    }
  } else
    return -2;
}
Point Rect::RegionCenterPoint() const noexcept {
  return Point{origin.x + width / 2, origin.y + height / 2};
}

Rect const& Rect::getRegion(int num, int hdiv, int vdiv) const noexcept {
  int h = num % hdiv, v = num / hdiv;
  int width = this->width / hdiv, height = this->height / vdiv;
  int ox = origin.x + width * h, oy = origin.y + height * v;
  return Rect(ox, oy, width, height);
}

Point Rect::RegionCenterPoint(Rect const& r) noexcept {
  return r.RegionCenterPoint();
}

#pragma endregion

Square::Square(int x, int y, int width) : Rect{x, y, width, width} {}
int Square::block(Point const& p, int blk) { return Rect::block(p, blk, blk); }


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
      normal_dist(random_engine, NDT(pos, pos >> 3 ? pos >> 3:1))
      : 
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

#pragma region EasyX T3 Draw
/**
 * @brief Draw Cross Line;
 * Two lines span half of the Y-axis angle
 * default as 40
 * @param x : the cross center X value
 * @param y : the cross center Y value
 * @param len : the length of each line
 */
void DrawCross(int x, int y) { DrawCross(x, y, getheight() / 4); }
void DrawCircle(int x, int y) { DrawCircle(x, y, getheight() / 6); }
void DrawCross(int x, int y, int len) {
  constexpr static unsigned long DColor = 0x34409D;
  setlinecolor(DColor);
  double Xdelta = len / 4, Ydelta = len / 4 * Sqrt3;
  line(x + Xdelta, y - Ydelta, x - Xdelta,
       y + Ydelta);  // draw slash
  line(x + Xdelta, y + Ydelta, x - Xdelta,
       y - Ydelta);  // draw backslash
}
void DrawCircle(int x, int y, int len) {
  constexpr static unsigned long DColor = 0x39AC3B;
  setlinecolor(DColor);
  circle(x, y, len);
}
#pragma endregion
int inTicTacToeRegion(Point cursor, Rect T3, int xblock, int yblock) {
  return T3.block(cursor, xblock, yblock);
}
int inTicTacToeRegion(Point cursor, Rect T3, int blocks) {
  return T3.block(cursor, blocks, blocks);
}


int print_d(char const* const _Format, ...){
#ifdef DEBUG
    int _Result;
    va_list _ArgList;
    __crt_va_start(_ArgList, _Format);
    _Result = _vfprintf_l(stdout, _Format, NULL, _ArgList);
    __crt_va_end(_ArgList);
    return _Result;
#else
  return 0;
#endif
}
