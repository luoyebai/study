/**
 * @file test.cpp
 * @author luoyebai (2112216825@qq.com)
 * @brief 测试
 * @version 0.1
 * @date 2023-08-11
 *
 * @copyright Copyright (c) 2023
 *
 */

// 关闭日志颜色
// #define CLOSE_COLOR

#include "test.hpp"
// 时间
// using namespace std::literals;
// sleep(1s);

int main(int argc, char *argv[]) {
    logSystemInit("洛叶白", "../../../../log/", KUNKNOW);
    logTest();
    // 异步测试
    // exit(0);
    log_i("按回车退出程序");
    log_i("测试");
    log_i("再次测试");
    std::thread(pubTest).detach();
    std::thread(subTest).detach();
    getchar();
}
