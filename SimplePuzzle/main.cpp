#include <graphics.h>

#include <iostream>

#include "include/GameBoard.h"
#include "source/Narrator.hpp"

void uni_test() {
  int *value = new int[10];
  for (int i = 0; i < 10; ++i) {
    value[i]=0;
  }
  for (int i = 0; i < 1e5; ++i) {
    ++value[Random::GetRandom(1e3) % 0xa];
  }
  for (int i = 0; i < 10; ++i) {
    std::cout << value[i] << " | ";
  }
}

int main() {
  initgraph(960, 640);
  GameBoard().
      //Comets()
      run()
      //Snake()
      ;
  closegraph();
  return 0;
}