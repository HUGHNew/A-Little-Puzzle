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
using SnakeCell = Square<16>;
class snake {
  using SnakeBody = std::list<SnakeCell>;
  SnakeBody body;
  Direction forward;
  int body_limit, eaten, delta;

 public:
  snake(SnakeCell origin, Direction d, int move_delta = 16, int limit = 9)
      : body(), forward(d), body_limit(limit), eaten(0), delta(move_delta) {
    body.emplace_back(origin);
  }
  /// <summary>
  /// get the body length of current direction
  /// </summary>
  int getDirectionSteps() const noexcept {
    int value = 0, count = -1;
    if ((int)forward < 3) {  // vertical
      value = body.begin()->origin.y;
      for (auto&& it : body) {
        if (it.origin.y == value) {
          ++count;
        } else {
          return count;
        }
      }
    } else {  // horizontal
      value = body.begin()->origin.x;
      for (auto&& it : body) {
        if (it.origin.x == value) {
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
    body.push_front(SnakeCell(body.begin()->origin.x + xoff,
                              body.begin()->origin.y + yoff));
    body.pop_back();
    return true;
  }
  bool step() { return step(forward); }
  /// <summary>
  /// grow if not reach the body limit
  /// </summary>
  snake& grow() {
    SnakeCell it = body.back();
    step();
    if (body.size() < body_limit) {
      body.emplace_back(it);
    }
    ++eaten;
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
        if (!it.origin.InBoundary(corner)) return true;
      }
    } else {
      return !body.begin()->origin.InBoundary(corner);
    }
    return false;
  }
  Direction getDirection() const noexcept { return forward; }
  int getEatenCount() const noexcept { return eaten; }
  int getLimit() const noexcept { return body_limit; }
  void flush(Point const& corner) noexcept {
    body.clear();
    body.emplace_back(SnakeCell(Random::GetPoint(corner)));
    eaten = 0;
  }
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
  SnakeCell food;
  const Point Boundary;

 public:
  SnakeGameBase(int w = Width, int h = Height, int move = 16, int limit = 9)
      : role(Random::GetPoint(Point(w, h)), Random::GetDirection(), move,
             limit),
        Boundary(w, h),
        food(Random::GetPoint(w, h)) {}
  constexpr static const char* SnakeSymbol = "123456789";
  constexpr static const char* SnakeDirect = "^v<>";
  constexpr static int Width = 640;
  constexpr static int Height = 480;
  constexpr static int Second = 1000;
  constexpr static int LowSpeedFrame =
      Second / 8;  // the time gap between every two frames
  constexpr static int MiddleSpeedFrame = Second / 24;
  constexpr static int HighSpeedFrame = Second / 60;
  int getSleepTime() const noexcept {
    int process = role.getEatenCount();
    if (process < role.getLimit() >> 1)
      return LowSpeedFrame;
    else if (process < role.getLimit())
      return MiddleSpeedFrame;
    else
      return HighSpeedFrame;
  }

  bool play() {
    initialization();
    int rel = 2;
    do {
      switch (gameStart()) {
        case 0:
          rel = 1;
          break;
        case -1:
          rel = 0;
          break;
        default:
          break;
      }
      flush();
    } while (rel == 2);
    dispose();
    return rel == 1;
  }
  virtual void flush() {
    role.flush(Boundary);
    food.origin = Random::GetPoint(Boundary);
  }
  /**
   * @return
   * -2 : die
   * -1 : ESC/exit
   *  0 : answer
   */
  virtual int gameStart() = 0;
  virtual bool eat() noexcept {
    if (role.getSnake().begin()->collision(food)) {
      food.origin = Random::GetPoint(Boundary);
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
  LOGFONT SnakeFont;
  int cellSize;

 public:
  SnakeGameEasyX(int w = Width, int h = Height, int move = 16,
                 bool selfhold = true, int blk = 16)
      : SnakeGameBase(w, h, move), selfHoldGraph(selfhold), cellSize(blk) {
    gettextstyle(&SnakeFont);
    SnakeFont.lfHeight = cellSize << 1;
    SnakeFont.lfWidth = 0;
    SnakeFont.lfWeight = FW_MEDIUM;
    SnakeFont.lfQuality = ANTIALIASED_QUALITY;
  }
  virtual void initialization() {
    if (selfHoldGraph) {
      initgraph(Width, Height);
    }
    setbkmode(TRANSPARENT);
    settextstyle(&SnakeFont);
  }
  virtual void dispose() {
    if (selfHoldGraph) {
      closegraph();
    }
  }
  virtual void action() {
    cleardevice();
    BeginBatchDraw();
    //std::cout << "food:" << food.origin.x << "," << food.origin.y
    //          << " head:" << role.getSnake().begin()->origin.x << ","
    //          << role.getSnake().begin()->origin.y << std::endl;
    if (eat()) {
      role.grow();
    }
    setfillcolor(RED);
    displayCell(food, false);
    const auto& body = role.getSnake();
    auto head = body.begin();
    setfillcolor(Random::GetColor());
    displayCell(*head, SnakeDirect[getBitPos((int)role.getDirection())], true);

    ++head;
    if (body.size() != 1) {
      for (unsigned idx = 0; idx < body.size() - 1; ++idx, ++head) {
        setfillcolor(Random::GetColor());
        displayCell(*head, true);
      }
      setfillcolor(BLACK);
      displayCell(*body.rbegin(), SnakeSymbol[body.size() - 1], true);
    }

    FlushBatchDraw();
    Sleep(getSleepTime());
  }
  /**
   * @return
   * -2 : die
   * -1 : ESC/exit
   *  0 : answer
   */
  virtual int gameStart() override {
    int input = 1, len = 0;
    do {
      input = getInput();
      if (Point::InRange(input, 1, 9)) {
        role.step(Direction(input));
      } else if (input == 9) {
        role.step();
      } else {
        return input;
      }
      if (role.suicide() || role.crossBoundary(Boundary)) {
        return input = -2;
      }
      len = role.getDirectionSteps();
      for (int i = 0; i < len; ++i) {
        role.step();
        if (role.suicide() || role.crossBoundary(Boundary)) {
          return input = -2;
        }
      }
      action();
    } while (input > 0);
  }
  void fillrectangle(Rect const& rect) {
    ::fillrectangle(rect.origin.x, rect.origin.y, rect.origin.x + rect.width,
                    rect.origin.y + rect.height);
  }
  void displayCell(const SnakeCell& p, TCHAR ch, bool cell) {
    if (cell) {
      fillrectangle(p);
      outtextxy(p.origin.x, p.origin.y, ch);
    } else {
      fillcircle(p.origin.x, p.origin.y, 7);
    }
  }
  void displayCell(const SnakeCell& p, bool cell) {
    if (cell) {
      fillrectangle(p);
    } else {
      fillcircle(p.origin.x, p.origin.y, 7);
    }
  }
};
