#pragma once
#include <graphics.h>

namespace Words {
constexpr static const wchar_t* ESC = _T("终于放弃了？");
constexpr static const wchar_t* ANS = _T("你悟了"); // answer
    namespace Comets {
    constexpr static const wchar_t* Hint =
        _T("按下你所看见\n出现次数最多的符号");
    constexpr static const wchar_t* Start =
        _T("很久之后，它或许会浮现\n不过，一瞬之间");
    constexpr static const wchar_t* Internal = _T("只是一瞬，抑或是永恒");
    }  // namespace Comets
    namespace Snake {
    constexpr static const wchar_t * Hiss= _T("有所限制 必有所突破");
    }
    namespace T3 {
    constexpr static const wchar_t* Start = _T("战胜我 拿到答案");
    constexpr static const wchar_t* Game1 = _T("取胜之道 从未有之");
    constexpr static const wchar_t* Game3 = _T("你应该知道了胜算如何");
    constexpr static const wchar_t* Game5 = _T("无意之举");
    constexpr static const wchar_t* ResDraw = _T("平局");
    constexpr static const wchar_t* ResLose = _T("你输了？");
    constexpr static const wchar_t* Restart = _T("按ESC退出;点击重开");
    }  // namespace T3
    namespace End {
    constexpr static const wchar_t* LockAll = _T("前面的区域 以后再来探索吧");
    constexpr static const wchar_t* LockRest = _T("看来 你很懂哦");
    constexpr static const wchar_t* LockComet = _T("运气不错 可惜没结果");
    constexpr static const wchar_t* Unlock = _T("归焉 待办");
    constexpr static const wchar_t* Others = _T("未 曾 设 想");
    }  // namespace End
    constexpr static const wchar_t* AllWords[] = {
        Comets::Hint, Comets::Start, Comets::Internal,
        Snake::Hiss,
        T3::Start,T3::Game1,T3::Game3,T3::Game5,T3::ResDraw,T3::ResLose,T3::Restart,
        End::LockAll, End::LockRest, End::LockComet,End::Unlock,End::Others,
        ESC,ANS
    };
}
class Narrator {
  static LOGFONT* AsideFont;
  #pragma region PrintWords
  static void PrintWords(RECT* r, UINT idx) {
    drawtext(Words::AllWords[idx], r, DT_CENTER | DT_VCENTER);
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
    CometHint, CometStart, CometInternal,
    SnakeHiss,
    TStart, TGame1, TGame3, TGame5,GameDraw,GameLose,GameRestart,
    LockALL, LockRest, LockComet, Unlock, LockOthers,
    ESC,ANS
  };
  /**
  * @brief show the narrator' words
  */
  static void showContent(int idx, int msec,bool flush ,int fontheight = 50) {
    cleardevice();
    LOGFONT font;
    gettextstyle(&font);
    font.lfHeight = fontheight;
    font.lfWidth = fontheight >> 1;
    font.lfQuality = ANTIALIASED_QUALITY;
    //_tcscpy_s(font.lfFaceName, _T("黑体"));
    settextstyle(&font);
    PrintWords(idx);
    if (flush) {FlushBatchDraw();}
    Sleep(msec);
    cleardevice();
  }
  static constexpr int DefaultTimeGap_msec = 2000;
  static void Say(PuzzleAside idx, bool flush = false,
                  int msec = DefaultTimeGap_msec) {
    //BeginBatchDraw();
    showContent((int)idx, msec,flush);
    //FlushBatchDraw();
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