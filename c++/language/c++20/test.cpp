// 目前gcc和clang对moudle支持都不是很好
// 准确来说是c++20只有msvc有较好的支持
// c++20
// import <iostream>;
// c++20以前
#include <iostream>
#include <ranges>

int main() {
  __attribute_maybe_unused__ int x;
  return 0;
}