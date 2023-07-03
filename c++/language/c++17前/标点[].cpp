// #-----------------------------
// #------------------标点符号<[]>|
// #-----------------------------

/// @brief 内容详细请看c++官方文档:
// ?标点符号----https://zh.cppreference.com/w/cpp/language/punctuators

struct X {
  //* 4.delete表达式
  void operator delete[](void *ptr) { return; }
};

int main() {
  //* 1.下标运算符号
  //* 2.声明或类型标识符号
  int x[1]{1};
  x[0] = 0;
  //* 3.new[]运算符
  auto y = new int[1];
  //* 5.lambda表达式中分隔捕获
  //* 注:结构化绑定无法用lambda直接捕获
  auto F = []() {};
  //* 4.delete表达式
  //* 防止多次detele
  if (y != nullptr)
    delete[] y;
  return 0;
}