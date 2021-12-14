#ifndef T3BaseMagic
#define T3BaseMagic
constexpr int MAGIC_SQUARE[9] = {4, 9, 2, 3, 5, 7, 8, 1, 6};
constexpr int M2Idx[9] = {7, 2, 3, 0, 4, 8, 5, 6, 1};  // Magic to Index
#endif                                                 // T3Base
#pragma once
#include <vector>

using vec = std::vector<int>;
class T3Base {
 public:
  enum class Winner : int { X, O, Draw, NotYet };
  enum class Match : bool { Computer, Network };
  T3Base():Board(9, ' '), winner(Winner::NotYet), matcher(Match::Computer) {
    X.reserve(9);
    O.reserve(9);
  }
  const static int INF = 1000;
  const static int WIN = 10;
  bool finished(vec const &, vec const &);
  /**
   * @brief 评估当前局势
   */
  int eval(vec &, vec &);
  Winner eval();
  /**
   * @brief find best position for next
   * @param maximize
                 true for X
                false for O
   * @return position : [1,9]
   */
  int findBest(bool maximize);
  /**
   * @brief show current board
   */
  virtual const vec &board() { return Board; }
  Winner getLastWinner() const noexcept;
  void clear();
  virtual int getMatchNext() = 0;
  virtual int getSelfNext() = 0;

 protected:
  constexpr static int PollingGapMS = 10;
  vec X, O, Board;  // store magic numbers
  Winner winner;
  Match matcher;
  bool win(vec &) noexcept;
  /**
   * @brief calc the score of currnt step
   * @param depth the depth of current level
   * @param maximize whether makes the score higher or not
   * decide which player is searching, too
   */
  int minmax(vec x, vec o, int depth, bool maximize);
};