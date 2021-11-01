//#include "TicTacToe.h"
////#include "Tools.hpp"
//struct Assert {
//  template <typename _T>
//  static void AreEqual(_T const& lhs, _T const& rhs, const char* err) {
//    if (lhs != rhs) std::cerr << err << std::endl;
//  }
//  static void True(bool cond, const char* err) {
//    if (!cond) std::cerr << err << std::endl;
//  }
//  static void False(bool cond, const char* err) { True(!cond, err); }
//};
//void ContainTest() {
//  VecOne<int> X{1, 2, 4}, Y{5, 6, 3}, Z{7,8,9};
//  for (int i = 1; i < 10; ++i) {
//    Assert::True(In(i, {X, Y,Z}), "should in");
//  }
//  for (int i = 7; i < 10; ++i) {
//    Assert::True(NotIn(i, {X, Y}), "should not in");
//  }
//  for (int i = 7; i < 10; ++i) {
//    Assert::False(In(i, {X, Y}), "should not in");
//  }
//}
//#include"TicTacToe.h"
//int main() {
//  //ContainTest();
//  TicTacToe T3;
//  T3.play();
//  return 0;
//}