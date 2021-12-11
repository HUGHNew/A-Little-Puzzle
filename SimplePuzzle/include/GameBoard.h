#pragma once

#include <list>
#include "CharComet.hpp"
using CometCollection = std::list<CharComet>;
class GameBoard {
  CometCollection comets;
  int CometsAnswer, CurrentChoice, Progress;
 public:
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
  void InitStartMenu()const noexcept;
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
  static int GetStartMenuRawBlockInput() noexcept;
};
