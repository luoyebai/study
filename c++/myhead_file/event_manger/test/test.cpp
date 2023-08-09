// 关闭日志颜色
// #define CLOSE_COLOR
// 日志不显示在那个函数下
#define NOT_SHOW_FUN

#include "test.hpp"

int main(int argc, char *argv[]) {
    logTest();
    // 异步测试
    // exit(0);
    logInfo("按回车退出程序");
    std::thread(pubTest).detach();
    std::thread(subTest).detach();
    getchar();
}
