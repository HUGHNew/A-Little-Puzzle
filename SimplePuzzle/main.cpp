#include <graphics.h>

#include "include/GameBoard.h"
#include "source/Narrator.hpp"

int main() {
  initgraph(960, 640);
  GameBoard().
      run()
      //Comets()
      //Snake()
      ;
  closegraph();
  return 0;
}