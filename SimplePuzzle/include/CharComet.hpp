#pragma once
#include <functional>

#include "utility.h"

struct SPoint:Point {
  char symbol;
  SPoint(int xv, int yv,char s) : Point(xv,yv),symbol(s) {}
  SPoint(int v,char s) : Point(v, v),symbol(s) {}
  SPoint(const Point& p,char s) : SPoint(p.x, p.y, s) {}
  SPoint(const SPoint& p) : SPoint(p.x,p.y,p.symbol) {}
};
class CharComet {
  using iter_fn = std::function<SPoint&(SPoint&)>;
  int TTL;
  SPoint origin;
  iter_fn move;

 public:
  using iter_type = iter_fn;
  CharComet(const SPoint& o, int TimeToLive, iter_fn const& fn)
      : origin(o), TTL(TimeToLive), move(fn) {}
  const SPoint& step() {
    --TTL;
    return move(origin);
  }
  int getTTL() const noexcept { return TTL; }
  const SPoint& getPoint() const noexcept { return origin; }
  bool dead() const noexcept { return TTL <= 0; }
  bool liveLonger(int TTL) const noexcept { return this->TTL > TTL; }
  bool operator<(const CharComet& rhs) const noexcept {
    return this->TTL < rhs.TTL;
  }
  bool operator>(const CharComet& rhs) const noexcept {
    return this->TTL > rhs.TTL;
  }
  bool operator==(const CharComet& rhs) const noexcept {
    return this->TTL == rhs.TTL;
  }
};
