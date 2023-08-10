// 关闭日志颜色
// #define CLOSE_COLOR

#include "test.hpp"

using namespace std::literals;

int main(int argc, char *argv[]) {
    logTest();
    // 异步试
    // exit(0);
    logi("按回车退出程序");
    logi("123333");
    logi("7777777777777777777777777");
    std::thread(pubTest).detach();
    std::thread(subTest).detach();
    getchar();
    // sleep(1s);
}
