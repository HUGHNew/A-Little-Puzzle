#pragma once
#include <graphics.h>

static class Narrator {
  constexpr static const wchar_t* AsideWords[] = {
      _T("Ҳ��һ�Σ�Ҳ��ǧ���\n�������ֻ򿴲���"),
      _T("�ܾ�֮��������ḡ��\n������һ˲֮��"),
      _T("ֻ��һ˲���ֻ�������"),
      _T("�������� ������ͻ��"),
      _T("սʤ�� �õ���"),
      _T("���������ߵ�ݾ���(��)"),  //�����ս
      _T("ȡʤ֮�� ��δ��֮"),
      _T("��Ӧ��֪����ʤ�����"),
      _T("����֮��"),
      _T("��ӭ����������գ����������������"),
      _T("��\n"),  //ѡ�����
      _T("ǰ������� �Ժ�����̽����"),
      _T("���� ��ܶ�Ŷ"),
      _T("�������� ��ϧû���"),
      _T("���� ����")};
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
    _tcscpy_s(font.lfFaceName, _T("����"));
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