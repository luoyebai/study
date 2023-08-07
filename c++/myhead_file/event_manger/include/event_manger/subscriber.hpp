#ifndef INCLUDE_EVENT_MANGER_SUBSCRIBER_HPP
#define INCLUDE_EVENT_MANGER_SUBSCRIBER_HPP

// topic
#include "topic.hpp"

/**
 * @brief
 * 订阅者的简单实现,继承自BasePubSub,将通过话题容器接收订阅者数据
 *
 * @tparam T 订阅的数据类型
 */
template <typename T> class Subscriber : public BasePubSub<T> {
  public:
    /**
     * @brief 创建一个新的发布者
     *
     * @param sub_name 订阅者名字
     * @param topic_name 话题名字----不要与其他话题同名
     * @param sleep_time
     * 睡眠时间/可选,在没有发布者时的睡眠时间,默认为0
     */
    explicit Subscriber(const std::string &sub_name,
                        const std::string &topic_name, double sleep_time = 0.)
        : BasePubSub<T>(sub_name, topic_name) {
        while (1) {
            // 在基类构造时找到话题
            if (topic_ptr_ != nullptr) {
                break;
            }
            if (sleep_time > 0.)
                logWarn(getLogger(), "还没有相应发布者,等待", sleep_time, 's');
            sleep(sleep_time);
            // 重新初始化
            topicPtrInit();
        }
        // 该话题订阅者计数器加1
        ++topic_ptr_->subs_num;
    }

    /**
     * @brief 销毁该订阅者
     * 如果订阅者和发布者都为空,会删除话题
     */
    ~Subscriber() {
        --topic_ptr_->subs_num;
        logDebug(getLogger(), "退出话题订阅");
    }

    // 订阅到的数据时间戳
    timeval temp_time_stamp;

    /**
     * @brief 构造一个新的订阅者同时返回指向其的指针
     *
     * @param name 同构造函数
     * @param topic_name 同上
     * @param sleep_time 同上
     * @return shared_ptr<Subscriber<T>> 返回共享指针
     */
    static auto makeSharePtr(const std::string &name,
                             const std::string &topic_name,
                             double sleep_time = 0.) {
        return std::make_shared<Subscriber<T>>(name, topic_name, sleep_time);
    }

    /**
     * @brief 基本同上,为独有指针
     *
     * @param name
     * @param topic_name
     * @param queue_size
     * @return auto
     */
    static auto makeUniquePtr(const std::string &name,
                              const std::string &topic_name,
                              double sleep_time = 0.) {
        return std::make_unique<Subscriber<T>>(name, topic_name, sleep_time);
    }

    // 基类函数
    using BasePubSub<T>::getName;
    using BasePubSub<T>::getTopicName;
    using BasePubSub<T>::getLogger;

    /**
     * @brief 取出话题容器中最早的数据
     *
     * @param is_update_timer
     * 是否更新定时器----一般用于统计发布数据的频率
     * @return auto 取出来的数据
     */
    auto pop(bool is_update_timer = true) {
        if (is_update_timer)
            one_sec_timer_.update();
        ++count_;
        if (one_sec_timer_.is_time_ok && is_update_timer) {
            logDebug(getLogger(),
                     "接收数据频率:", count_ / one_sec_timer_.diff_time,
                     " Hz\t");
            count_ = 0;
        }

        auto topic_data = topic_ptr_->popData();
        temp_time_stamp = topic_data.time_stamp;
        return topic_data.data;
    }

  private:
    // 基类成员变量
    using BasePubSub<T>::topic_ptr_;
    using BasePubSub<T>::timer_;
    using BasePubSub<T>::one_sec_timer_;
    using BasePubSub<T>::count_;
    // 基类函数
    using BasePubSub<T>::topicPtrInit;
};
#endif // !INCLUDE_EVENT_MANGER_SUBSCRIBER_HPP
