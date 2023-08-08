// std
#include <chrono>
#include <thread>
// log
#include "log/log.hpp"
// pub
#include "event_manger/publisher.hpp"
// sub
#include "event_manger/subscriber.hpp"

int main(int argc, char *argv[]) {
    // 当前使用者姓名,日志文件路径,默认等级
    logSystemInit("洛叶白", "../../../../log", KINFO);
    logInfo("日志系统初始化完毕");
    // auto x = LogSystem::getInstance();

    return 0;
}
