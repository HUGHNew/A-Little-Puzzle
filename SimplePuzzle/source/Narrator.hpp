#pragma once
#include <graphics.h>

static class Narrator {
  constexpr static const wchar_t* AsideWords[] = {
      _T("也许一次，也许千万遍\n看见，抑或看不见"),
      _T("很久之后，它或许会浮现\n不过，一瞬之间"),
      _T("只是一瞬，抑或是永恒"),
      _T("有所限制 必有所突破"),
      _T("战胜我 拿到答案"),
      _T("你或许可以走点捷径？(按)"),  //网络对战
      _T("取胜之道 从未有之"),
      _T("你应该知道了胜算如何"),
      _T("无意之举"),
      _T("欢迎来到迷你解谜，在这里，所见即所得"),
      _T("按\n"),  //选项界面
      _T("前面的区域 以后再来探索吧"),
      _T("看来 你很懂哦"),
      _T("运气不错 可惜没结果"),
      _T("归焉 待办")};
  static LOGFONT* AsideFont;
  #pragma region PrintWords
  static void PrintWords(RECT* r, UINT idx) {
    drawtext(AsideWords[idx], r, DT_CENTER | DT_VCENTER);
  }
  static void PrintWords(int x, int y, int width, int height, UINT idx) {
    RECT r = {x, y, x + width, y + height};
    PrintWords(&r, idx);
  }
  static RECT getFitRect() {
    int width = getwidth(), height = getheight();
    int y = height / 3;
    return {0, y, width, height - y};
  }
  static void PrintWords(UINT idx) {
    RECT r = getFitRect();
    PrintWords(&r, idx);
  }
  #pragma endregion
 public:
  enum PuzzleAside {
    CometSlip, CometStart, CometInternal,
    SnakeHiss,
    TStart, TFailed3, TGame1, TGame5, TGame10,
    Welcome, Select,
    LockALL, LockRest, LockComet, Unlock
  };
  /**
  * @brief show the narrator' words
  */
  static void showContent(int idx, int msec, int fontheight = 50) {
    cleardevice();
    LOGFONT font;
    gettextstyle(&font);
    font.lfHeight = fontheight;
    font.lfQuality = ANTIALIASED_QUALITY;
    _tcscpy_s(font.lfFaceName, _T("黑体"));
    settextstyle(&font);
    PrintWords(idx);
    Sleep(msec);
    cleardevice();
  }
  static constexpr int DefaultTimeGap_msec = 2000;
  static void Say(PuzzleAside idx, int msec = DefaultTimeGap_msec) {
    cleardevice();
    showContent((int)idx, msec);
    cleardevice();
  }
  static int SetFontHeight(int height) {
    int old_height = 0;
    if (AsideFont == NULL) {
      gettextstyle(AsideFont);
    } else {
      old_height = AsideFont->lfHeight;
    }
    AsideFont->lfHeight = height;
    return old_height;
  }
};