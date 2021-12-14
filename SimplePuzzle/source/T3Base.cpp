#include <algorithm>

#include "../include/T3Base.h"
#include "../include/utility.h"

bool T3Base::finished(vec const &x, vec const &o) {
  return x.size() + o.size() == 9;
}

int T3Base::eval(vec &x, vec &o) {
  if (win(x)) {
    return WIN;
  }
  if (win(o)) {
    return -WIN;
  }
  return 0;
}

T3Base::Winner T3Base::eval() {
  switch (eval(X, O)) {
    case WIN:
      return winner = Winner::X;
    case -WIN:
      return winner = Winner::O;
    default:
      return winner = finished(X, O) ? Winner::Draw : Winner::NotYet;
  }
}
int T3Base::findBest(bool maximize) {
  int best = maximize ? -INF : INF;
  int move = 0, val;
  for (int i : MAGIC_SQUARE) {
    if (NotIn(i, X) && NotIn(i, O)) {
      if (maximize) {
        val = minmax(i + X, O, 0, !maximize);
        if (val > best) {
          best = val;
          move = i;
        }
      } else {
        val = minmax(X, i + O, 0, !maximize);
        if (val < best) {
          best = val;
          move = i;
        }
      }
    }
  }
  return move;
}

T3Base::Winner T3Base::getLastWinner() const noexcept {
  return winner;
}

bool T3Base::win(vec &player) noexcept {
  if (player.size() < 3) return false;
  std::sort(player.begin(), player.end());
  vec::const_iterator lhs, rhs, cur = player.begin();
  int sum = 0;
  for (unsigned i = 0; i < player.size() - 2; ++i) {
    lhs = cur + 1;
    rhs = player.end() - 1;
    while (lhs < rhs) {
      sum = *cur + *lhs + *rhs;
      if (sum == 15) {
        return true;
      } else if (sum < 15) {
        ++lhs;
      } else {
        --rhs;
      }
    }
    ++cur;
  }
  return false;
}

int T3Base::minmax(vec x, vec o, int depth, bool maximize) {
  int score = eval(x, o);
  if (score == WIN) {
    return score - depth;
  } else if (score == -WIN) {
    return score + depth;
  }
  if (finished(x, o)) {
    return 0;
  }
  int best = maximize ? -INF : INF;
  for (int i : MAGIC_SQUARE) {
    if (NotIn(i, x) && NotIn(i, o)) {
      if (maximize) {
        best = std::max(best, minmax(i + x, o, depth + 1, !maximize));
      } else {
        best = std::min(best, minmax(x, i + o, depth + 1, !maximize));
      }
    }
  }
  return best;
}
void T3Base::clear() {
  Board.clear();
  X.clear();
  O.clear();
  winner = Winner::NotYet;
}