#pragma once
#include "T3Base.h"
class TicTacToe : public T3Base {
 public:
  int getLastOpponentsMove() const noexcept;
  TicTacToe();
  int getMatchNext();
  /**
   *  @return  position in range[1,9]
   */
  int getSelfNext();
  static void DrawGameBoard();
  Winner play();

 private:
  int lastOppo;  // [1,9]
  /**
   * @param position : [1,9]
   */
  bool MoveCheck(int position) const noexcept;
  void GetInputAndDraw(int pos, bool XStep = true)noexcept;
};
