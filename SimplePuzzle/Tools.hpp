#ifndef _TOOLS_
#define _TOOLS_ 1
#include<vector>
#pragma region In
template <typename T>
using VecOne = std::vector<T, std::allocator<T>>;
template <class InputIterator, class T, class BinaryOperation>
T foldl(InputIterator first, InputIterator last, T init,
        BinaryOperation binary_op) {
  if (first == last) return init;
  for (InputIterator ptr = first; ptr < last; ++ptr) {
    init = binary_op(init, *ptr);
  }
  return init;
}

static bool In(int e, VecOne<int> const& container) {
  for (auto const& it : container) {
    if (it == e) {
      return true;
    }
  }
  return false;
}

static bool In(int e, std::initializer_list<VecOne<int>> containers) {
  return foldl(containers.begin(), containers.end(), false,
               [=](bool lhs, VecOne<int> const& rhs) -> bool {
                 return lhs || In(e, rhs);
               });
}
static bool NotIn(int e, VecOne<int> const& container) { return !In(e, container); }
static bool NotIn(int e, std::initializer_list<VecOne<int>> containers) {
  return !In(e, containers);
}
#pragma endregion
#pragma region Operators
static VecOne<int> operator+(int e, VecOne<int> const& raw) {
  VecOne<int> vec_new(raw);
  vec_new.push_back(e);
  return vec_new;
}
static VecOne<int> operator+(VecOne<int> const& raw, int e) { return e + raw; }
#pragma endregion
#endif  //_TOOLS_