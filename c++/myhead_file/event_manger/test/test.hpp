#ifndef INCLUDE_TEST_HPP
#define INCLUDE_TEST_HPP

// std
#include <string>
// include
#include "event_manger/publisher.hpp"
#include "event_manger/subscriber.hpp"
#include "log/log.hpp"

void logTest() {
    // 看不到debug
    logSystemInit("洛叶白", "../../../../log/", KDEBUG);
    // log_system->log_mode[1] = 0;
    // log_system->log_mode[2] = 0;
    logDebug("日志系统初始化完毕");
    logUnknow("这是未知信息");
    logDebug("这是调试信息");
    logInfo("这是正常信息");
    logWarn("这是警告信息");
    logError("这是错误信息");
    // Fatal等级报错同时退出程序
    // logFatal("这是失败信息");
}

void pubTest() {
    auto pub =
        Publisher<std::string>::makeSharePtr("发布者1号", "话题测试", 10);
    auto sub = Subscriber<std::string>::makeSharePtr("订阅者2号", "另一个话题");
    // 测试数据
    std::string str[10];
    for (auto i = 0; i < 10; ++i) {
        if (i == 8) {
            str[i] = "杀死订阅者";
            continue;
        }
        str[i] = "这是消息数据:" + std::to_string(i);
    }
    // 发布者回调
    pub->timerCallBack(
        [&pub, &str] {
            static size_t i = 0;
            auto data = str[i];
            pub->push(data);
            // logInfo(pub->getLogger(), "发布数据:", '{', data, '}');
            ++i;
            if (i >= 10) {
                // 跳出callback
                return false;
            }
            // 继续callback
            return true;
        },
        // 1hz
        1);
    // 订阅者回调
    sub->timerCallBack([&sub] {
        auto data = sub->pop();
        // logInfo(sub->getLogger(), "接收数据:", '{', data, '}');
        if (data == "杀死订阅者") {
            return false;
        }
        return true;
    });
}

void subTest() {
    auto pub =
        Publisher<std::string>::makeSharePtr("发布者2号", "另一个话题", 10);
    auto sub =
        Subscriber<std::string>::makeSharePtr("订阅者1号", "话题测试", 0.5);
    sub->timerCallBack([&sub, &pub] {
        // 取出订阅者1号,让发布者2号发布
        auto data = sub->pop();
        // logInfo(sub->getLogger(), "接收数据:", '{', data, '}');
        pub->push(data);
        // logInfo(pub->getLogger(), "发布数据:", '{', data, '}');
        if (data == "杀死订阅者") {
            // 此时跳出,发布者2号也会停止发送
            return false;
        }
        return true;
    });
}

#endif // !INCLUDE_TEST_HPP
