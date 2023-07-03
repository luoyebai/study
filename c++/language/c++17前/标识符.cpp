// #-----------------------------
// #------------------main和初始化|
// #-----------------------------

/**
 *@brief 内容详细请看c++官方文档:
 * ?main 主函数----https://zh.cppreference.com/w/cpp/language/main_function
 * ?标识符----https://zh.cppreference.com/w/cpp/language/identifiers
 * 这里是程序的入口main函数,除了全局的初始化外,这里是代码第一个执行的地方
 * @param argc // *输入参数个数
 * @param argv // *具体参数字符串---argv[0]为程序名
 * @return int // *程序返回值
 */
int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
    //*关键词只有在属性记号中可做他用
    //*如:[[maybe_unused]]---告诉编译器可能不使用该变量
    // [[maybe_unused]]
    int x{0};
    [[maybe_unused]] int a[1]{0};
    //*这里的decltype(x)代替了类型声明符
    [[maybe_unused]] decltype(x) y1{0};
    //! decltype((x))y2 = 0;    //非常量引用只能绑定左值
    [[maybe_unused]] decltype((x)) y2{y1}; //*成立 y2为可变左值引用
    [[maybe_unused]] decltype(+(x)) y3{0}; //*成立 y3等同y1
    //! decltype(a) y4 = {0} ;  //类型为int类型1长数组只能赋值为空
    [[maybe_unused]] decltype(a) y4{0}; //*成立 y4为int类型1长数组
    [[maybe_unused]] decltype(+a) y5{nullptr}; //*成立 y5为int类型指针
    [[maybe_unused]] decltype(*a) y6{y1};      //*成立 y6为int引用同(a[0])
    [[maybe_unused]] decltype(+*a) y7(0);      //*成立 y7为int类型
    return 0;
}
