#ifndef INCLUDE_TEST_HPP
#define INCLUDE_TEST_HPP

#include "log/log.hpp"
// std
#include <string>
// include
#include "event_manger/publisher.hpp"
#include "event_manger/subscriber.hpp"

/**
 * @brief
 * 这是日志测试函数
 */
void logTest() {
    // 初始化日志系统
    // logSystemInit("洛叶白", "../../../../log/", KUNKNOW);
    // log_system->log_mode[1] = 0;
    // log_system->log_mode[2] = 0;
    log_d("日志系统初始化完毕");

    log_u("这是未知信息");
    log_d("这是调试信息");
    log_i("这是正常信息");
    log_w("这是警告信息");
    log_e("这是错误信息");
    // Fatal等级报错同时退出程序
    // log_f("这是失败信息");
}

/**
 * @brief
 * 这是发布者测试函数
 */
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
            // log_i(pub->getLogger(), "发布数据:", '{', data, '}');
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
        // log_i(sub->getLogger(), "接收数据:", '{', data, '}');
        if (data == "杀死订阅者") {
            return false;
        }
        return true;
    });
}

/**
 * @brief
 * 这是订阅者测试函数
 */
void subTest() {
    auto pub =
        Publisher<std::string>::makeSharePtr("发布者2号", "另一个话题", 10);
    auto sub =
        Subscriber<std::string>::makeSharePtr("订阅者1号", "话题测试", 0.5);
    sub->timerCallBack([&sub, &pub] {
        // 取出订阅者1号,让发布者2号发布
        auto data = sub->pop();
        // log_i(sub->getLogger(), "接收数据:", '{', data, '}');
        pub->push(data);
        // log_i(pub->getLogger(), "发布数据:", '{', data, '}');
        if (data == "杀死订阅者") {
            // 此时跳出,发布者2号也会停止发送
            return false;
        }
        return true;
    });
}

#endif // !INCLUDE_TEST_HPP
