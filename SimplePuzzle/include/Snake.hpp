#pragma once
#include <graphics.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <list>

#include "utility.h"

static bool InLine(Direction lhs, Direction rhs) {
  if (lhs == rhs) return true;
  int lv = (int)lhs, rv = (int)rhs;
  int mn = lv > rv ? lv : rv, mx = lv + rv - mn;  // min and max
  if (mx - mn * 2 == 0 && mn != 2) {
    return true;
  }
  return false;
}
class snake {
  using SnakeBody = std::list<Point>;
  SnakeBody body;
  Direction forward;
  int body_limit, eaten, delta;

 public:
  snake(Point origin, Direction d, int limit = 9, int move_delta = 1)
      : body(), forward(d), body_limit(limit), eaten(0), delta(move_delta) {
    body.push_back(origin);
  }
  /// <summary>
  /// get the body length of current direction
  /// </summary>
  int getDirectionSteps() const noexcept {
    int value = 0, count = -1;
    if ((int)forward < 3) {  // vertical
      value = body.begin()->y;
      for (auto&& it : body) {
        if (it.y == value) {
          ++count;
        } else {
          return count;
        }
      }
    } else {  // horizontal
      value = body.begin()->x;
      for (auto&& it : body) {
        if (it.x == value) {
          ++count;
        } else {
          return count;
        }
      }
    }
    return count;
  }
  /// <summary>
  /// step with old or new direction
  /// </summary>
  /// <param name="d">move with new direction</param>
  /// <return>change the direction or not</return>
  bool step(Direction d) {
    int xoff = 0, yoff = 0;
    Direction move = d;
    if (InLine(d, forward)) {
      move = forward;
    } else {
      forward = d;
    }
    switch (move) {
      case Direction::Up:
        yoff = -delta;
        break;
      case Direction::Down:
        yoff = delta;
        break;
      case Direction::Left:
        xoff = -delta;
        break;
      case Direction::Right:
        xoff = delta;
        break;
    }
    body.push_front(Point(body.begin()->x + xoff, body.begin()->y + yoff));
    body.pop_back();
    return true;
  }
  bool step() { return step(forward); }
  /// <summary>
  /// grow if not reach the body limit
  /// </summary>
  snake& grow() {
    if (body.size() < body_limit) {
      body.push_back(body.back());
    }
    step();
    return *this;
  }
  const SnakeBody& getSnake() const noexcept { return body; }
  /// <summary>
  /// whether the snake ate itself
  /// </summary>
  bool suicide() const noexcept {
    auto&& head = *body.begin();
    int cnt = 0;
    for (auto&& it : body) {
      if (head == it) ++cnt;
    }
    return cnt != 1;  // it's safe if the head doesn't cover anyone
  }
  /// <summary>
  /// check if the snake cross the boundary
  /// </summary>
  /// <param name="corner">The lower right corner of the rectangle
  /// boundary</param> <param name="checkall">check head only if false, and
  /// check the whole body else</param>
  bool crossBoundary(const Point& corner, bool checkall = false) {
    if (checkall) {
      for (auto&& it : body) {
        if (!it.InBoundary(corner)) return true;
      }
    } else {
      return !body.begin()->InBoundary(corner);
    }
    return false;
  }
  Direction getDirection() const noexcept { return forward; }
  int getEatenCount() const noexcept { return eaten; }
  int getLimit() const noexcept { return body_limit; }
};

/// <summary>
/// get the first bit position from the LSB
/// </summary>
/// <returns>
/// -1 : num==0
/// </returns>
static int getBitPos(int num) {
  int p = 0;
  while (true) {
    if (num == 0)
      return -1;
    else if (num & 0x1)
      return p;
    else {
      ++p;
      num >>= 1;
    }
  }
}
class SnakeGameBase {
 protected:
  snake role;
  Point food;
  const Point Boundary;

 public:
  SnakeGameBase(int w = Width, int h = Height, int limit = 9)
      : role(GetSnakeOrigin(), Random::GetDirection(), limit),
        Boundary(w, h),
        food(0, 0) {
    food = Random::GetPoint(Boundary);
  }
  constexpr static const char* SnakeSymbol = "123456789";
  constexpr static const char* SnakeDirect = "^v<>";
  constexpr static int Width = 640;
  constexpr static int Height = 480;
  constexpr static int Second = 1000;
  constexpr static int LowSpeedFrame =
      Second / 24;  // the time gap between every two frames
  constexpr static int MiddleSpeedFrame = Second / 60;
  constexpr static int HighSpeedFrame = Second / 120;
  int getSleepTime() const noexcept {
    int process = role.getEatenCount();
    if (process < role.getLimit() >> 1)
      return LowSpeedFrame;
    else if (process < role.getLimit() + 1)
      return MiddleSpeedFrame;
    else
      return HighSpeedFrame;
  }
  virtual int play() {
    int input = 1, len = 0;
    initialization();
    do {
      input = getInput();
      if (Point::InRange(input, 1, 9)) {
        role.step(Direction(input));
      } else
        role.step();
      if (role.suicide() || role.crossBoundary(Boundary)) {
        input = -2;
        break;
      }
      len = role.getDirectionSteps();
      for (int i = 0; i < len; ++i) {
        role.step();
        if (role.suicide() || role.crossBoundary(Boundary)) {
          input = -2;
          break;
        }
      }
      action();
    } while (input > 0);
    dispose();
    return input;
  }
  virtual bool eat() noexcept {
    if (role.getSnake().begin()->operator==(food)) {
      food = Random::GetPoint(Boundary);
      return true;
    }
    return false;
  }
  virtual void initialization() = 0;
  virtual void dispose() = 0;
  virtual void action() = 0;
  /// <summary>
  /// get input key
  /// -1 : ESC
  /// 0  : answer
  /// 1  : Up
  /// 2  : Down
  /// 4  : Left
  /// 8  : Right
  /// 9  : no input
  /// </summary>
  /// <returns></returns>
  virtual int getInput() {
    if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('w') & 0x8000))
      return (int)Direction::Up;
    if ((GetAsyncKeyState(VK_DOWN) & 0x8000) ||
        (GetAsyncKeyState('s') & 0x8000))
      return (int)Direction::Down;
    if ((GetAsyncKeyState(VK_LEFT) & 0x8000) ||
        (GetAsyncKeyState('a') & 0x8000))
      return (int)Direction::Left;
    if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) ||
        (GetAsyncKeyState('d') & 0x8000))
      return (int)Direction::Right;
    if (GetAsyncKeyState('9') & 0x8000) return 0;
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) return -1;
    return 9;
  }

 protected:
  virtual Point&& GetSnakeOrigin() const noexcept {
    return Random::GetPoint(Point(Width, Height));
  }
};

class SnakeGameEasyX : public SnakeGameBase {
  bool selfHoldGraph;
 public:
  SnakeGameEasyX(bool selfhold=false) : selfHoldGraph(selfhold) {}
  virtual void initialization() {
    if (selfHoldGraph) {
      initgraph(Width, Height);
    }
  }
  virtual void dispose() {
    if (selfHoldGraph) {
      closegraph();
    }
  }
  virtual void action() {
    cleardevice();
    BeginBatchDraw();
    std::cout << "food:" << food.x << "," << food.y
              << "head:" << role.getSnake().begin()->x << ","
              << role.getSnake().begin()->y << std::endl;
    // outtextxy(food.x, food.y, _T("Here is food"));
    if (eat()) {
      role.grow();
    }
    displayCell(food, '*');

    const auto& body = role.getSnake();
    auto head = body.begin();
    displayCell(*head, SnakeDirect[getBitPos((int)role.getDirection())]);

    ++head;
    int idx = 0;
    for (; head != body.end(); ++head) {
      displayCell(*head, SnakeSymbol[idx++]);
    }

    FlushBatchDraw();
    Sleep(getSleepTime());
  }
  void displayCell(const Point& p, TCHAR ch) { outtextxy(p.x, p.y, ch); }
};
