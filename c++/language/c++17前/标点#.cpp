// #----------------------------
// #------------------标点符号<#>|
// #----------------------------

/// @brief 内容详细请看c++官方文档:
// ?标点符号----https://zh.cppreference.com/w/cpp/language/punctuators

//* #的作用
//* 1.引入预处理指
//* include预处理的作用是把指定路径下文件内容复制到此文件下
//* define预处理的作用是用后面的内容替换前面的内容

#include <iostream>
#define X 10

//* define 也可以有参数
#define OUT(arg) std::cout << arg
//* ##展开此处参数带""展开(可嵌套)
#define PRINT(...) OUT(#__VA_ARGS__)

int main() {
  int x{1};
  OUT("OUT宏输出:");
  OUT(x);
  OUT("\n");
  PRINT("PRINT宏输出:" 1, 2, "test" x\n);
  return 0;
}
