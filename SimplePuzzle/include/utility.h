#include <vector>
#pragma once
enum class Direction { Up = 1, Down = 2, Left = 4, Right = 8 };
struct Rect;
struct Point {
  int x, y;
  Point(int xv, int yv) : x(xv), y(yv) {}
  Point(int v) : Point(v, v) {}
  Point(const Point& p) : x(p.x), y(p.y) {}
  bool operator==(const Point& p) const noexcept;
  Point& operator=(const Point& p);
  bool In() const noexcept;
  bool In(Rect const& region) const noexcept;
  /// <summary>
  /// return val in [leftBound,rightBound);
  /// </summary>
  static bool InRange(int val, int leftBound, int rightBound);
  bool InBoundary(const Point& p) const noexcept;
  bool InBoundary(const Point& LTp, const Point& RBp) const noexcept;
};
struct Rect {
  Point origin;
  int width, height;
  Rect(int x, int y, int wid, int h);
  Rect(Point const& p, int wid, int heig);
  int block(Point const& p, int wb, int hb);
  Point RegionCenterPoint() const noexcept;
  bool collision(Rect const& other) const noexcept;
  int coverage(Rect const& other) const noexcept;
  int area() const noexcept { return width * height; }
  Rect const& getRegion(int num, int hdiv = 3, int vdiv = 3) const noexcept;
  bool operator==(Rect const&) const noexcept;
  static Point RegionCenterPoint(Rect const&) noexcept;
};

template <int Size = 16>
struct Square : Rect {
  Square(int x, int y) : Rect(x, y, Size, Size) {}
  Square(Point const& p) : Square(p.x, p.y) {}
  int block(Point const& p, int blk) const { return Rect::block(p, blk, blk); }
};
int inTicTacToeRegion(Point cursor, Rect T3, int xblock, int yblock);
int inTicTacToeRegion(Point cursor, Rect T3, int blocks);
namespace Random {
char GetRandomUppercase(bool normal = false) noexcept;
char GetRandomLowercase(bool normal = false) noexcept;
int GetRandom(int max, bool normal = false) noexcept;
int GetRandom(int min, int max, bool normal = false) noexcept;
char GetRandomAscii() noexcept;
/**
 * @brief normal distribution
 */
char GetRandomAscii(int pos) noexcept;
Point GetPoint(int x, int y) noexcept;
Point GetPoint(Point const& corner) noexcept;
Direction GetDirection() noexcept;
int GetColor() noexcept;
}  // namespace Random

#pragma region In
template <typename T>
using VecOne = std::vector<T, std::allocator<T>>;
template <class InputIterator, class T, class BinaryOperation>
T foldl(InputIterator first, InputIterator last, T init,
        BinaryOperation binary_op) {
  if (first == last) return init;
  for (InputIterator ptr = first; ptr < last; ++ptr) {
    init = binary_op(init, *ptr);
  }
  return init;
}

static bool In(int e, VecOne<int> const& container) {
  for (auto const& it : container) {
    if (it == e) {
      return true;
    }
  }
  return false;
}

static bool In(int e, std::initializer_list<VecOne<int>> containers) {
  return foldl(containers.begin(), containers.end(), false,
               [=](bool lhs, VecOne<int> const& rhs) -> bool {
                 return lhs || In(e, rhs);
               });
}
static bool NotIn(int e, VecOne<int> const& container) {
  return !In(e, container);
}
static bool NotIn(int e, std::initializer_list<VecOne<int>> containers) {
  return !In(e, containers);
}
#pragma endregion
#pragma region Operators
static VecOne<int> operator+(int e, VecOne<int> const& raw) {
  VecOne<int> vec_new(raw);
  vec_new.push_back(e);
  return vec_new;
}
static VecOne<int> operator+(VecOne<int> const& raw, int e) { return e + raw; }
#pragma endregion
#pragma region EasyX Draw
/**
 * @brief Draw Cross Line;
 * Two lines span half of the Y-axis angle
 * default as 40
 * @param x : the cross center X value
 * @param y : the cross center Y value
 * @param len : the length of each line
 */
void DrawCross(int x, int y);
void DrawCircle(int x, int y);
void DrawCross(int x, int y, int len);
void DrawCircle(int x, int y, int len);
#pragma endregion
#pragma region debug
int print_d(char const* const _Format, ...);
#pragma endregion