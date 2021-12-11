

#include <graphics.h>

#include <iostream>

#include "source/Narrator.hpp"
#include "include/GameBoard.h"


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
  //uni_test();
  // 初始化图形模式
  initgraph(640, 480);
  GameBoard().run();
  closegraph();
  return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
