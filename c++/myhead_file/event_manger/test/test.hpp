#ifndef INCLUDE_TEST_HPP
#define INCLUDE_TEST_HPP

// std
#include <string>
// include
#include "event_manger/publisher.hpp"
#include "event_manger/subscriber.hpp"
#include "log/log.hpp"

void logTest() {
    logSystemInit("洛叶白", "../../../../log/", KDEBUG);
    logDebug("日志系统初始化完毕");
    logUnknow("这是未知信息");
    logDebug("这是调试信息");
    logInfo("这是正常信息");
    logWarn("这是警告信息");
    logError("这是错误信息");
    logFatal("这是失败信息");
    // Fatal等级报错同时退出程序
    // logExit("这是退出信息");
}

void pubTest() {
    auto pub =
        Publisher<std::string>::makeSharePtr("发布者1号", "话题测试", 10);
    auto pub_bak = Publisher<int>::makeSharePtr("发布者2号", "另一个话题", 10);
    pub->timerCallBack(
        [&pub] {
            static size_t i = 0;
            if (i == 9) {
                auto data = "杀死订阅者1号";
                pub->push(data);
                logInfo(pub->getLogger(), "发布数据:", '{', data, '}');
                ++i;
                return true;
            }
            auto data = "这是消息数据:" + std::to_string(i);
            pub->push(data);
            logInfo(pub->getLogger(), "发布数据:", '{', data, '}');
            ++i;
            if (i > 10) {
                i = 0;
                return false;
            }
            return true;
        },
        100);
}

void subTest() {
    sleep(1);
    Subscriber<std::string> sub("订阅者1号", "话题测试");
    sub.timerCallBack([&sub] {
        auto data = sub.pop();
        logInfo('<', sub.getName(), '/', sub.getTopicName(), '>',
                "接收数据:", '{', data, '}');
        if (data == "杀死订阅者1号") {
            return false;
        }
        return true;
    });
}


#endif // !INCLUDE_TEST_HPP
