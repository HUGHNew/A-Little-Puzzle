//#define debug 1
#include <algorithm>
#include <cstdarg>
#include "TicTacToe.h"

int printf_d(const char* fmt, ...) {
#ifdef debug
  int _Result;
  va_list _ArgList;
  va_start(_ArgList, fmt);
  _Result = vfprintf(stdout, fmt, _ArgList);
  va_end(_ArgList);
  return _Result;
#else
  return 0;
#endif
}

bool TicTacToe::win(vec & player) {
  if (player.size() < 3) return false;
  std::sort(player.begin(), player.end());
  vec::const_iterator lhs, rhs, cur = player.begin();
  int sum = 0;
  for (unsigned i = 0; i < player.size() - 2; ++i) {
    lhs = cur + i + 1;
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
  }
  return false;
}
bool TicTacToe::finished(vec const& x, vec const& o) {
  return x.size() + o.size() == 9;
}

int TicTacToe::eval(vec & x, vec & o) {
  if (win(x)) {
    return WIN;
  }
  if (win(o)) {
    return -WIN;
  }
  return 0;
}


int TicTacToe::minmax(vec x, vec o, int depth, bool maximize) {
  int score = eval(x,o);
  if (score == WIN) {
    return score - depth;
  } else if (score == -WIN) {
    return score + depth;
  }
  if (finished(x,o)) {
    return 0;
  }
  int best = maximize ? -INF : INF;
  for (int i : MAGIC_SQUARE) {
    if (NotIn(i, {x, o})) {
       if(maximize){
           best=std::max(best,minmax(i+x,o,depth+1,!maximize));
       }else{
           best=std::min(best,minmax(x,i+o,depth+1,!maximize));
       }
    }
  }
  return best;
}

int TicTacToe::findBest(bool maximize) {
  int best = maximize ? -INF : INF;
  int move = 0, val;
  for (int i : MAGIC_SQUARE) {
    if (NotIn(i, {X, O})) {
      if (maximize) {
        val = minmax(i + X,O, 0, !maximize);
        if (val > best) {
          best = val;
          move = i;
        }
      } else {
        val = minmax(X,i + O, 0, !maximize);
        if (val < best) {
          best = val;
          move = i;
        }
      }
    }
  }
  return move;
}
/**
 * @brief show current board
 */
void TicTacToe::board() {
  for (int i = 0; i < 3; ++i) {
    puts("----------");
    for (int j = 0; j < 3; ++j) {
      putchar('|');
      putchar(Board[i * 3 + j]);
      putchar(' ');
    }
    puts("|");
  }
  puts("----------");
}
bool InRange(int e, int lower, int higher) { return e >= lower && e < higher; }
TicTacToe::Winner TicTacToe::play() {
  clear();
  int pos;
  do {
    board();
    // get self input
    pos = getSelfNext();

    Board[pos-1] = 'X';
    X.push_back(MAGIC_SQUARE[pos - 1]);

    // get match next
    pos = getMatchNext();
    printf_d("findbest:%d,position:%d\n", pos, M2Idx[pos - 1]);
    if (pos != 0) {
        Board[M2Idx[pos - 1]] = 'O';
        O.push_back(pos);
    }
    // printf_d("X win:%d,O win:%d,size:%d",win(X),win(O),X.size()+O.size());
  } while (!(win(X) || win(O) || finished(X,O)));
  board();
  switch (eval(X,O)) {
    case WIN:
      return winner = Winner::X;
    case -WIN:
      return winner = Winner::O;
    default:
      return winner = Winner::Draw;
  }
}
void TicTacToe::clear() {
  memset(Board,' ',sizeof(Board));
  X.clear();
  O.clear();
  winner = Winner::NotYet;
}
TicTacToe::Winner TicTacToe::getLastWinner() const noexcept { return winner; }
TicTacToe::TicTacToe(Match m,bool console)
    : matcher(m), winner(Winner::NotYet),ConsoleMode(console) {
  memset(Board, ' ', sizeof(Board));
}
TicTacToe::Match TicTacToe::switchMatch(Match m) {
  Match prev = matcher;
  matcher = m;
  return prev;
}
int TicTacToe::getMatchNext() {
  switch (matcher) { 
    case Match::Computer:
      return findBest(false);
    case Match::Network:
        //todo network com
  }
}
int TicTacToe::getSelfNext() {
  int pos;
  if (ConsoleMode) {
      bool first = true;
      do {
        std::cout << "[1..9]=> ";
        std::cin >> pos;
        if (!first) puts("Invalid Move");
        first = false;
      } while ((!InRange(pos, 1, 10)) || In(MAGIC_SQUARE[pos - 1], {X, O}));
  } else {
      
      // polling model
  // set a flag and check it every TIMER_TIME millisecond
  }
  return pos;
}
TicTacToe::Winner TicTacToe::Step(bool first) {
  if (first) clear();
  //board();
  return Winner::NotYet;
}
TicTacToe::Winner TicTacToe::Step(int position) { return Winner::NotYet; }