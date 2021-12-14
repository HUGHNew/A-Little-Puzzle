#include <graphics.h>

#include "../include/utility.h"
#include "../include/TicTacToe.h"

int TicTacToe::getMatchNext() {
  switch (matcher) {
    case Match::Computer:
      return findBest(false);
    case Match::Network:
      // deprecated
      return 0;
  }
}

int TicTacToe::getSelfNext() {
  static Rect screen(0, 0, getwidth(), getheight());
  int pos;
  do {
    ExMessage msg = getmessage(EM_MOUSE | EM_KEY);
    if (msg.message == WM_KEYUP) {
    }
    while ((msg.message != WM_LBUTTONUP && msg.message != WM_RBUTTONUP) ||
           msg.message == WM_KEYUP) {
      msg = getmessage(EM_MOUSE | EM_KEY);
    }
    switch (msg.message) {
      case WM_KEYUP:
        if (msg.vkcode == VK_ESCAPE)
          throw "";
        else if (msg.vkcode == '5')
          throw 0;
    }
    pos = screen.block(Point{msg.x, msg.y}, 3, 3);
  } while (!MoveCheck(pos + 1));
  return pos + 1;
}

void TicTacToe::DrawGameBoard() {
  int height = getheight(), width = getwidth();
  int hdis = height / 3, wdis = width / 3;
  // BeginBatchDraw();
  // horizonal lines
  line(0, hdis, width, hdis);
  line(0, hdis * 2, width, hdis * 2);
  // vertical lines
  line(wdis, 0, wdis, height);
  line(wdis * 2, 0, wdis * 2, height);
  // EndBatchDraw();
}

TicTacToe::Winner TicTacToe::play() {
  static Rect screen(0, 0, getwidth(), getheight());
  bool start = true;
  Winner rel = Winner::NotYet;
  int pos;
  DrawGameBoard();
  do {
    // get self input [1,9]
    pos = getSelfNext();

    //Board[pos - 1] = 'X';
    //X.push_back(MAGIC_SQUARE[pos - 1]);
    //Point center = screen.getRegion(pos - 1).RegionCenterPoint();
    //DrawCross(center.x, center.y);
    GetInputAndDraw(pos);
    pos = getMatchNext();  // magic number in [1,9]
    if (pos != 0) {
      //int midx = M2Idx[pos - 1];
      //lastOppo = midx;
      //Board[midx] = 'O';
      //O.push_back(pos);  // directly store the value
      //center = screen.getRegion(midx).RegionCenterPoint();
      //DrawCircle(center.x, center.y);
      GetInputAndDraw(pos, false);
    }
  } while (!(win(X) || win(O) || finished(X, O)));
  switch (eval(X, O)) {
    case WIN:
      return winner = Winner::X;
    case -WIN:
      return winner = Winner::O;
    default:
      return winner = Winner::Draw;
  }
}

bool TicTacToe::MoveCheck(int position) const noexcept {
  int magic = MAGIC_SQUARE[position - 1];
  return !((!(position > 0 && position < 10)) ||
           (In(magic, X) && In(magic, O)));
}

void TicTacToe::GetInputAndDraw(int pos, bool XStep) noexcept {
  static Rect screen(0, 0, getwidth(), getheight());
  if (XStep) {
    Board[pos - 1] = 'X';
    X.push_back(MAGIC_SQUARE[pos - 1]);
    --pos;
  } else {
    pos = M2Idx[pos - 1];
    lastOppo = pos;
    Board[pos] = 'O';
    O.push_back(pos);  // directly store the value
  }
  Point center = screen.getRegion(pos).RegionCenterPoint();
  if(XStep) {
    DrawCross(center.x, center.y);
  } else {
    DrawCircle(center.x, center.y);
  }
}

int TicTacToe::getLastOpponentsMove() const noexcept { return lastOppo; }

TicTacToe::TicTacToe() : lastOppo(-1) {}