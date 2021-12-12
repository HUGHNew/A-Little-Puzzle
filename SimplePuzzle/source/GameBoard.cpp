#include <conio.h>
#include <graphics.h>


#include "../include/GameBoard.h"

namespace Random {
int Ascii2Pos(char ch) {
  if (Point::InRange(ch, 'A', 'Z' + 1)) {
    return ch - 'A' + 10;
  } else {
    return ch - '0';
  }
}
Point GetPoint() noexcept {
  return Point(GetRandom(0, getwidth()), GetRandom(0, getheight()));
}
SPoint GetSPoint(Point const& corner) noexcept {
  return SPoint(GetRandom(0, corner.x), GetRandom(0, corner.y),
      GetRandomUppercase());
}
SPoint GetSPoint(char ch,bool sym_normal=false) noexcept { 
    return SPoint(GetPoint(), 
        sym_normal ? GetRandomAscii(Ascii2Pos(ch))
        : GetRandomUppercase());
}
}

#pragma region EasyX relative
void outtextxy(const SPoint& point) {
  outtextxy(point.x, point.y, point.symbol);
}
void outtextxy(const SPoint& point,char ch) {
  outtextxy(point.x, point.y, ch?ch:point.symbol);
}
static RECT GetFitRect(int size,int seq=0) {
  int wdelta = getwidth() / size;
  int right = wdelta * (seq + 1), height = getheight();
  int top = height/3;
  // left,top,right,bottom
  return {wdelta * seq, top, right, height - top};
}
#pragma endregion

#pragma region digit
static const wchar_t* DIGIT_ASCII[] = {
    _T(R"(
d88888D d88888b d8888b.  .d88b.  
YP  d8' 88'     88  `8D .8P  Y8. 
   d8'  88ooooo 88oobY' 88    88 
  d8'   88~~~~~ 88`8b   88    88 
 d8' db 88.     88 `88. `8b  d8' 
d88888P Y88888P 88   YD  `Y88P'  
)"),
    _T(R"(
 .d88b.  d8b   db d88888b 
.8P  Y8. 888o  88 88'     
88    88 88V8o 88 88ooooo 
88    88 88 V8o88 88~~~~~ 
`8b  d8' 88  V888 88.     
 `Y88P'  VP   V8P Y88888P 
)"),
    _T(R"(
d888888b db   d8b   db  .d88b.  
`~~88~~' 88   I8I   88 .8P  Y8. 
   88    88   I8I   88 88    88 
   88    Y8   I8I   88 88    88 
   88    `8b d8'8b d8' `8b  d8' 
   YP     `8b8' `8d8'   `Y88P'  
)"),
    _T(R"(
d888888b db   db d8888b. d88888b d88888b 
`~~88~~' 88   88 88  `8D 88'     88'     
   88    88ooo88 88oobY' 88ooooo 88ooooo 
   88    88~~~88 88`8b   88~~~~~ 88~~~~~ 
   88    88   88 88 `88. 88.     88.     
   YP    YP   YP 88   YD Y88888P Y88888P 
)"),
    _T(R"(
d88888b  .d88b.  db    db d8888b. 
88'     .8P  Y8. 88    88 88  `8D 
88ooo   88    88 88    88 88oobY' 
88~~~   88    88 88    88 88`8b   
88      `8b  d8' 88b  d88 88 `88. 
YP       `Y88P'  ~Y8888P' 88   YD 
)"),
    _T(R"(
d88888b d888888b db    db d88888b 
88'       `88'   88    88 88'     
88ooo      88    Y8    8P 88ooooo 
88~~~      88    `8b  d8' 88~~~~~ 
88        .88.    `8bd8'  88.     
YP      Y888888P    YP    Y88888P 
)"),
    _T(R"(
.d8888. d888888b db    db 
88'  YP   `88'   `8b  d8' 
`8bo.      88     `8bd8'  
  `Y8b.    88     .dPYb.  
db   8D   .88.   .8P  Y8. 
`8888Y' Y888888P YP    YP 
)"),
    _T(R"(
.d8888. d88888b db    db d88888b d8b   db 
88'  YP 88'     88    88 88'     888o  88 
`8bo.   88ooooo Y8    8P 88ooooo 88V8o 88 
  `Y8b. 88~~~~~ `8b  d8' 88~~~~~ 88 V8o88 
db   8D 88.      `8bd8'  88.     88  V888 
`8888Y' Y88888P    YP    Y88888P VP   V8P 
)"),
    _T(R"(
d88888b d888888b  d888b  db   db d888888b 
88'       `88'   88' Y8b 88   88 `~~88~~' 
88ooooo    88    88      88ooo88    88    
88~~~~~    88    88  ooo 88~~~88    88    
88.       .88.   88. ~8~ 88   88    88    
Y88888P Y888888P  Y888P  YP   YP    YP   
)"),
    _T(R"(
d8b   db d888888b d8b   db d88888b 
888o  88   `88'   888o  88 88'     
88V8o 88    88    88V8o 88 88ooooo 
88 V8o88    88    88 V8o88 88~~~~~ 
88  V888   .88.   88  V888 88.     
VP   V8P Y888888P VP   V8P Y88888P 
)"),
};
#pragma endregion

GameBoard::GameBoard() noexcept : CometsAnswer(Random::GetRandomAscii()) {
  CurrentChoice = 1;
  Progress = 0;
}

void GameBoard::run() noexcept {
  int option = StartMenu(),rel = 0;
  while (true) {
    option = StartMenu();
    switch (option) {
    case -1:
        return;
    case 0:
        switch (CurrentChoice) {
        case 1:
            if (Comets()) rel = 0x1;
            break;
        case 2:
            if (Snake()) rel = 0x10;
            break;
        case 4:
            if (TicTacToe()) rel = 0x100;
            break;
        }
        Progress |= rel;
        break;
    case 1:
        TheEnd();
        break;
    }
  }
}
void flush_key_msg(int count = 2) {
  while (--count) {
    getmessage(EM_KEY);
  }
}
    // todo key event control and Answer TIME
bool GameBoard::Comets() noexcept {
  FlushAll();
  /**
   * 2-bit control flag
   * 0x1 clear flag
   * 0x2 show answer flag
   * 0x4 ESC flag
   * 0x8 ans flag
  */
  int controlFlag = 0;
  constexpr int Clear = 1;
  constexpr int Answer = 2;
  constexpr int ESC = 4;
  constexpr int Finish = 8;
  /**
   * answer gap - 5s
   * random gap - 100ms
  */
  constexpr int Barrier = 500;
  constexpr int Correct = 10;
  int counter = Barrier;
  constexpr int SleepUnit = 10; // ms

  settextstyle(16, 8, _T("Courier"));  // 设置字体

  // 设置颜色
  settextcolor(GREEN);
  setlinecolor(BLACK);

  int height = getheight();
  RECT r = {0, 0, getwidth(), height};
  while (true) {
    for (int i = 0; i < Random::GetRandom(10); i++) {
      comets.emplace_back(CharComet(
          Random::GetSPoint(CometsAnswer,true),
          Random::GetRandom(height >> 4, height >> 1),
          [](SPoint& p) -> SPoint& { p.y += 1; return p; })
      );
    }
    #pragma region Draw Point
    char ch = controlFlag & Answer ? CometsAnswer : 0;
    BeginBatchDraw();
    for (auto&& it = comets.begin(); it != comets.end(); ++it) {
      if (it->dead()) {
        it = comets.erase(it);
        controlFlag |= Clear;
      } else {
        outtextxy(it->getPoint(),ch);
        it->step();
      }
    }
    FlushBatchDraw();
    #pragma endregion
    --counter;
    if (counter == 0) { // state transfer
      controlFlag ^= Answer;
      counter = controlFlag & Answer ? Correct : Barrier;
    }
    Sleep(SleepUnit);

    if (controlFlag & Clear) {
      cleardevice();
      --controlFlag; // 1 or 3
    }
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
      controlFlag |= ESC;
    }
    if (GetAsyncKeyState(CometsAnswer) & 0x8000) {
      controlFlag |= Finish;
    }
    if (controlFlag >= ESC) {
      flush_key_msg();
      // narrator's work
      return controlFlag >= Finish;
    }
  }
}
// todo weird snake
bool GameBoard::Snake() noexcept { return false; }
// todo 
bool GameBoard::TicTacToe() noexcept { return false; }
// todo 
void GameBoard::TheEnd() noexcept {}

int GameBoard::StartMenu() noexcept {
  InitStartMenu();
  int ch;
  while (true) {
    switch (ch = GetStartMenuOptions(CometsAnswer)) {
      case Left:
        CurrentChoice = CursorMove(true);
        break;
      case Right:
        CurrentChoice = CursorMove(false);
        break;
      case 0:
      case 1:
      case -1:
        return ch;
    }
  }
}

void GameBoard::InitStartMenu() const noexcept { DrawStartMenu(0); }

void GameBoard::DrawStartMenu(int idx) const noexcept {
  static const wchar_t* SM[] = {_T("流"), _T("蛇"), _T("棋")};
  FlushAll();
  RECT text_rects[] = {GetFitRect(3, 0), GetFitRect(3, 1), GetFitRect(3, 2)};
  int rect_height = text_rects[0].bottom - text_rects[0].top;
  int rect_width  = text_rects[0].right - text_rects[0].left;
  static LOGFONT normal,old;
  gettextstyle(&normal);
  old = normal;
  normal.lfHeight = rect_height;
  normal.lfWidth = rect_width>>1;
  normal.lfQuality = ANTIALIASED_QUALITY; // 抗锯齿
  COLORREF old_color=gettextcolor();

  settextstyle(&normal);

  BeginBatchDraw();
  settextcolor(WHITE);
  for (int i = 0; i < 3; ++i) {
    if (i == idx) {
      settextcolor(RED);
      drawtext(SM[i], text_rects + i, DT_CENTER | DT_VCENTER);
      settextcolor(WHITE);
    } else {
      drawtext(SM[i], text_rects + i, DT_CENTER | DT_VCENTER);
    }
  }
  FlushBatchDraw();
}

int GameBoard::CursorMove(bool left) noexcept { 
  static int init = 0;
  if (left) {
    if (init != 0) {
      --init;
      DrawStartMenu(init);
    }
  } else {
    if (init != 2) {
      ++init;
      DrawStartMenu(init);
    }
  }
  return 1 << init;
}

int GameBoard::GetStartMenuOptions(int comet) noexcept {
  const int AnswerSet[]={comet,'9','5'};
  constexpr int mask=0x8000;
  int proc=0,current;
  do{
    current=GetStartMenuRawBlockInput();
    if(current==AnswerSet[proc]){
      ++proc;
    }else{
      proc=0;
      switch (current) { 
        case Left:
          return Left;
        case Right:
          return Right;
        case -1:
          return -1;
        case 128:
          return 0;
      }
    }
  }while(proc!=3);
  return 1;
}

int GameBoard::GetStartMenuRawBlockInput(bool press) noexcept {
  constexpr int mask=0x8000;
  ExMessage msg = getmessage(EM_KEY); // blocked API for key msg
  while (msg.message != (press ? WM_KEYDOWN : WM_KEYUP)) {
    msg = getmessage(EM_KEY);
  }
  if (Point::InRange(msg.vkcode, 'a', 'z')) {
    return msg.vkcode - 'a' + 'A';
  } else if (Point::InRange(msg.vkcode, 'A', 'Z') ||
             Point::InRange(msg.vkcode, '0', '9')) {
    return msg.vkcode;
  }
  switch (msg.vkcode) {
    case VK_LEFT: return Left; // left shift
    case VK_RIGHT: return Right; // right shift
    case VK_RETURN: return 128; // push choice
    case VK_ESCAPE: return -1; // exit
    default: return 10; // no idea about it
  }
}

void GameBoard::FlushAll() noexcept {
  cleardevice();
  FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // Windows Console API
}
