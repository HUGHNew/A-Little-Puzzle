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
  outtextxy(point.x, point.y, ch);
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

// todo key event control and Answer TIME
bool GameBoard::Comets() noexcept {
  cleardevice();
  bool clearFlag = false;

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
    BeginBatchDraw();
    for (auto&& it = comets.begin(); it != comets.end(); ++it) {
      if (it->dead()) {
        it = comets.erase(it);
        clearFlag = true;
      } else {
        outtextxy(it->getPoint());
        it->step();
      }
    }
    FlushBatchDraw();
    // outtextxy(comets.top().getPoint());

    Sleep(10);  // 延时
    if (clearFlag) {
      cleardevice();
      clearFlag = false;
    }
    if (_kbhit()) break;  // 按任意键退出
  }
  return false;
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
      case 'a':
        CurrentChoice = CursorMove(true);
        break;
      case 'd':
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
  RECT text_rects[] = {GetFitRect(3, 0), GetFitRect(3, 1), GetFitRect(3, 2)};
  int rect_height = text_rects[0].bottom - text_rects[0].top;
  static LOGFONT normal,old;
  gettextstyle(&normal);
  old = normal;
  normal.lfHeight = rect_height;
  normal.lfQuality = ANTIALIASED_QUALITY; // 抗锯齿
  COLORREF old_color=gettextcolor();

  settextstyle(&normal);

  BeginBatchDraw();
  for (int i = 0; i < 3; ++i) {
    if (i == idx) {
      settextcolor(RED);
      drawtext(SM[i], text_rects + i, DT_CENTER | DT_VCENTER);
      settextcolor(old_color);
    } else {
      drawtext(SM[i], text_rects + i, DT_CENTER | DT_VCENTER);
    }
  }
  FlushBatchDraw();
  //settextstyle(&normal);
}

int GameBoard::CursorMove(bool left) noexcept { 
  static int init = 0;
  static bool move = false;
  if (left) {
    if (init != 0) {
      if (move) {
        move = false;
        --init;
        DrawStartMenu(init);
      } else {
        move = true;
      }
    }
  } else {
    if (init != 2) {
      if (move) {
        move = false;
        ++init;
        DrawStartMenu(init);
      } else {
        move = true;
      }
    }
  }
  return 1 << init;
}

int GameBoard::GetStartMenuOptions(int comet) noexcept {
  const int AnswerSet[]={comet,9,5};
  constexpr int mask=0x8000;
  int proc=0,current;
  do{
    current=GetStartMenuRawBlockInput();
    if(current==AnswerSet[proc]){
      ++proc;
    }else{
      proc=0;
      switch (current) { 
        case 'a':
          return 'a';
        case 'd':
          return 'd';
        case -1:
          return -1;
        case 128:
          return 0;
      }
    }
  }while(proc!=3);
  return 1;
}

int GameBoard::GetStartMenuRawBlockInput() noexcept {
  constexpr int mask=0x8000;
  ExMessage msg = getmessage(EM_KEY); // blocked API for key msg
  switch (msg.vkcode) {
    case '0':return 0;
    case '1':return 1;
    case '2':return 2;
    case '3':return 3;
    case '4':return 4;
    case '5':return 5;
    case '6':return 6;
    case '7':return 7;
    case '8':return 8;
    case '9':return 9;
    case VK_LEFT: return 'a'; // left shift
    case VK_RIGHT: return 'd'; // right shift
    case VK_RETURN: return 128; // push choice
    case VK_ESCAPE: return -1; // exit
    default: return 10; // no idea about it
  }
}
