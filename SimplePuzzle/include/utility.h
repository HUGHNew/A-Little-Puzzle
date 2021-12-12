#pragma once
enum class Direction { Up = 1, Down = 2, Left = 4, Right = 8 };
struct Point {
  int x, y;
  Point(int xv, int yv) : x(xv), y(yv) {}
  Point(int v) : Point(v, v) {}
  Point(const Point& p) : x(p.x), y(p.y) {}
  bool operator==(const Point& p) const noexcept;
  Point& operator=(const Point& p);

  /// <summary>
  /// return val in [leftBound,rightBound);
  /// </summary>
  static bool InRange(int val, int leftBound, int rightBound);
  bool InBoundary(const Point& p) const noexcept;
  bool InBoundary(const Point& LTp, const Point& RBp) const noexcept;
};
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
Point GetPoint() noexcept;
Point GetPoint(Point const& corner) noexcept;
Direction GetDirection() noexcept;
}  // namespace Random
int print_d(char const* const _Format, ...);