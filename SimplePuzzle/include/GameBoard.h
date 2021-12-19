#pragma once

#include <list>
#include "CharComet.hpp"
#include "TicTacToe.h"
#include "Snake.hpp"
using CometCollection = std::list<CharComet>;
class GameBoard {
  CometCollection comets;
  TicTacToe T3;
  SnakeGameEasyX snake;
  int CometsAnswer, CurrentChoice, Progress;
  bool GoToEnd;
 public:
  static constexpr int Left = 37;
  static constexpr int Right = 39;
  GameBoard()noexcept;
  void run()noexcept;
  bool Comets() noexcept;
  bool Snake() noexcept;
  bool TicTacToe() noexcept;
  void TheEnd() noexcept;
  /**
   * @brief The whole game start menu for game selection
   * @return current choice
   * -1 : exit
   *  0 : enter selected game
   *  1 : step hidden stage
  */
  int StartMenu() noexcept;
  /**
   * @param idx Game options
  */
  void DrawStartMenu(int idx) const noexcept;
  /**
   * @return the game choice 1<<n stands for game n
  */
  int CursorMove(bool left) noexcept;
  /**
   * @brief choose the game or enter the hidden stage
   * @return 'a'|'d' -- left|right shift
   * -1 exit
   * 0 choose current game
   * 1 enter the hidden stage
  */
  static int GetStartMenuOptions(int) noexcept;
  /**
   * @return [-1,10] 'a' 'd' 128
   *  -1 : esc
   * 0-9 : num
   *  10 : don't care
   * a|d : shift
   * 128 : enter
  */
  static int GetStartMenuRawBlockInput(bool press=true) noexcept;
  static void FlushAll() noexcept;
  struct Comments {
    static const wchar_t* Title;
    static const wchar_t* TitleDone;
    static const wchar_t* CometComment;
    static const wchar_t* SnakeComment;
    static const wchar_t* T3Comment;
    static const wchar_t* CommentsList[3];
  };
};