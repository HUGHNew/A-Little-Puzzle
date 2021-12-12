﻿#include <conio.h>
#include <graphics.h>


#include "../include/GameBoard.h"
#include "Narrator.hpp"

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
static RECT GetCommentRect(RECT const&base) { 
    int height = getheight();
  return {0, base.bottom, getwidth(), height - ((height - base.bottom)>>1)};
}
static RECT GetTitleRect(RECT const& base) {
  int height = getheight();
  return {0, (height - base.bottom) >> 1, getwidth(), base.bottom};
}
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
  ExMessage msg;
  if (count == -1) {
    while (peekmessage(&msg, EM_KEY));
  } else {
      while (--count) {
        getmessage(EM_KEY);
      }
  }
}
    // todo key event control and Answer TIME
bool GameBoard::Comets() noexcept {
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
   * random gap - 200ms
  */
  constexpr int Barrier = 500;
  constexpr int Correct = 20;
  int counter = Barrier;
  constexpr int SleepUnit = 10; // ms

  //Narrator::Say(Narrator::CometHint);
  //Narrator::Say(Narrator::CometStart);
  FlushAll();

  settextstyle(16, 8, _T("Courier"));  // 设置字体

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
      if (controlFlag & Answer) {
        counter = Correct;
        Narrator::Say(Narrator::CometInternal,true);
        FlushAll();
      } else {
        counter = Barrier;
      }
      settextstyle(16, 8, _T("Courier"));
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
      if (controlFlag >= Finish) {
        Narrator::Say(Narrator::ANS,true);
      } else {
        Narrator::Say(Narrator::ESC,true);
      }
      //flush_key_msg();
      EndBatchDraw();
      FlushAll();
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
#pragma region constant 
const wchar_t* GameBoard::Comments::Title =
    _T("这里是只需要键盘的小解谜，每个游戏都会自己的答案\n按方向键切换游戏;按回车键进入;")
    _T("按ESC退出");
const wchar_t* GameBoard::Comments::CometComment =
    _T("寻找字符流里面的奥秘");
const wchar_t* GameBoard::Comments::SnakeComment =
    _T("探索贪吃蛇的异常");
const wchar_t* GameBoard::Comments::T3Comment = _T("发现井字棋的胜率");
const wchar_t* GameBoard::Comments::CommentsList[3] = {CometComment,
                                                       SnakeComment, T3Comment};
#pragma endregion
void GameBoard::DrawStartMenu(int idx) const noexcept {
  static const wchar_t* SM[] = {_T("流"), _T("蛇"), _T("棋")};
  FlushAll();
  RECT text_rects[] = {GetFitRect(3, 0), GetFitRect(3, 1), GetFitRect(3, 2)};
  RECT title_rect = GetTitleRect(text_rects[0]),
       comment_rect = GetCommentRect(text_rects[0]);
  int rect_height = text_rects[0].bottom - text_rects[0].top;
  int rect_width  = text_rects[0].right - text_rects[0].left;
  static LOGFONT normal,comment;
  gettextstyle(&normal);
  comment = normal;
  normal.lfHeight = rect_height;
  normal.lfWidth = rect_width>>1;
  normal.lfQuality = ANTIALIASED_QUALITY; // 抗锯齿
  comment.lfHeight = 15;
  comment.lfWidth = 10;
  comment.lfQuality = ANTIALIASED_QUALITY;  // 抗锯齿
  COLORREF old_color=gettextcolor();


  BeginBatchDraw();
  settextstyle(&comment);
  drawtext(Comments::Title, &title_rect, DT_CENTER | DT_VCENTER);
  drawtext(Comments::CommentsList[idx], &comment_rect, DT_CENTER | DT_VCENTER);
  settextstyle(&normal);
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
  EndBatchDraw();
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
  flush_key_msg(-1);
  cleardevice();
  FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // Windows Console API
}
