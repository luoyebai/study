/// @brief //#内容详细请看c++官方文档
#include <functional>
// # {}标点符号
//* 1.class/struct或union定义中分隔成员说明
//* 2.enum中分隔枚举列表
//* 3.分割复合语句 如lambda表达式
//* 4.列表初始化
//* 5.命名空间定义中分割空间体
//* 6.语言链接中分隔声明

//*6
extern "C++" {
//*5
namespace name {
//*3
auto L = std::function<void(void)>([] {
    //*4
    int x[1]{1};
});

//*1
class C : decltype(L) {
    struct S : decltype(L) {
        union U {
            //*2
            enum E {};
        };
    };
};
} // namespace name
}
