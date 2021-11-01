#ifndef __TIC_TAC_TOE__
#define __TIC_TAC_TOE__ 1
#include <iostream>

#include "Tools.hpp"
constexpr int MAGIC_SQUARE[9] = {4, 9, 2, 3, 5, 7, 8, 1, 6};
constexpr int M2Idx[9] = {7, 2, 3, 0, 4, 8, 5, 6, 1};  // Magic to Index
class TicTacToe {
 public:
  int click=-1;
  enum class Winner : int { X, O, Draw, NotYet };
  enum class Match : bool {Computer,Network};
  const static int INF = 1000;
  const static int WIN = 10;
  using vec = std::vector<int>;
  Winner play();
  bool finished(vec const &, vec const &);
  /**
   * @brief 评估当前局势
   */
  int eval(vec &, vec &);

  int findBest(bool maximize);
  /**
   * @brief show current board
   */
  void board();
  Winner getLastWinner()const noexcept;
  TicTacToe(Match m = Match::Computer, bool console = true);
  Match switchMatch(Match m);
  Winner Step(bool first = true);
  Winner Step(int);

 private:
  constexpr static int PollingGapMS = 10;
  vec X, O;  // store magic numbers
  int Board[9];
  Winner winner;
  Match matcher;
  bool ConsoleMode;
  bool win(vec &);
  /**
   * @brief calc the score of
   * @param depth the depth of current level
   * @param maximize whether makes the score higher or not
   * decide which player is searching also
   */
  int minmax(vec x, vec o, int depth, bool maximize);
  void clear();
  int getMatchNext();
  int getSelfNext();
};
#endif  //__TIC_TAC_TOE__