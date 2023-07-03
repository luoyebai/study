// #-----------------------------
// #------------------标点符号<{}>|
// #-----------------------------

/// @brief 内容详细请看c++官方文档:
// ?标点符号----https://zh.cppreference.com/w/cpp/language/punctuators

//* 6.语言链接中分隔声明
extern "C++" {
//* 5.命名空间定义中分割空间体
namespace name {
//* 3.分割复合语句 如lambda(c++11起)表达式
[[maybe_unused]] auto L = [] {
  //* 4.列表初始化(c++11起)
  [[maybe_unused]] int x[1]{1};
};

//* 1.class/struct或union定义中分隔成员说明
class C : decltype(L) {
  struct S : decltype(L) {
    union U {
      //* 2.enum中分隔枚举列表
      enum E {};
    };
  };
};
} // namespace name
}
