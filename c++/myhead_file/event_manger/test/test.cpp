#include "test.hpp"

int main(int argc, char *argv[]) {
    logTest();
    logInfo("按回车退出程序");
    std::thread(pubTest).detach();
    std::thread(subTest).detach();
    getchar();
}
