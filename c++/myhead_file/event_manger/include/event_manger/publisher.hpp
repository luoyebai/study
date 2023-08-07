/**
 * @file publisher.hpp
 * @author luoyebai (2112216825@qq.com)
 * @brief 简单的订阅者实现
 * @version 0.1
 * @date 2023-08-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef INCLUDE_EVENT_MANGER_PUBLISHER_HPP
#define INCLUDE_EVENT_MANGER_PUBLISHER_HPP

// topic
#include "topic.hpp"

/**
 * @brief 发布者的简单实现,继承自BasePubSub,将通过话题容器和订阅者进行沟通
 *
 * @tparam T 发布的数据类型
 */
template <typename T> class Publisher : public BasePubSub<T> {
  public:
    /**
     * @brief 构造一个新的发布者
     *
     * @param pub_name 发布者名字
     * @param topic_name 话题名字----不要与其他话题同名
     * @param queue_size 容器大小/可选----该数据越小消息时效性越高,默认为1000
     */
    explicit Publisher(const std::string &pub_name,
                       const std::string &topic_name, size_t queue_size = 1e3)
        : BasePubSub<T>(pub_name, topic_name), queue_size_(queue_size) {
        // 话题指针为空
        if (topic_ptr_ == nullptr) {
            // 构造新的话题
            topic_ptr_ = std::make_shared<Topic<T>>(topic_name);
            std::any var = topic_ptr_;
            topic_ptrs_vec.push_back(var);
            logDebug(getLogger(), "初始化完毕");
        }
        // 该话题发布者计数器加1
        ++topic_ptr_->pubs_num;
    }

    /**
     * @brief 销毁该发布者
     * 如果订阅者和发布者都为空,会删除话题
     */
    ~Publisher() {
        --topic_ptr_->pubs_num;
        logDebug(getLogger(), "停止话题数据发布");
    }

    /**
     * @brief 构造一个新的发布者同时返回指向其的指针
     *
     * @param name 同构造函数
     * @param topic_name 同上
     * @param queue_size 同上
     * @return shared_ptr<Publisher<T>> 返回共享指针
     */
    static auto makeSharePtr(const std::string &name,
                             const std::string &topic_name,
                             size_t queue_size = 1e3) {
        return std::make_shared<Publisher<T>>(name, topic_name, queue_size);
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
                              size_t queue_size = 1e3) {
        return std::make_unique<Publisher<T>>(name, topic_name, queue_size);
    }

    /**
     * @brief 基类函数
     *
     */
    using BasePubSub<T>::getName;
    using BasePubSub<T>::getTopicName;
    using BasePubSub<T>::getLogger;

    /**
     * @brief 向话题容器中放入数据
     *
     * @param data 数据----放入话题容器中
     * @param is_update_timer 是否更新定时器----一般用于统计发布数据的频率
     */
    void push(const T &data, bool is_update_timer = true) {
        if (is_update_timer)
            one_sec_timer_.update();
        ++count_;
        if (one_sec_timer_.is_time_ok && is_update_timer) {
            logDebug(getLogger(),
                     "发布数据率:", count_ / one_sec_timer_.diff_time, " Hz\t");
            count_ = 0;
        }

        // 初始化话题数据
        AnyData<T> new_topic_data{timer_.time_now, data};
        // 通过话题指针发布数据
        topic_ptr_->pushData(new_topic_data);
        // 重置容器内数据大小
        topic_ptr_->resize(queue_size_);
        return;
    }

  private:
    size_t queue_size_;
    // 基类成员变量
    using BasePubSub<T>::topic_ptr_;
    using BasePubSub<T>::timer_;
    using BasePubSub<T>::one_sec_timer_;
    using BasePubSub<T>::count_;
};

#endif // !INCLUDE_EVENT_MANGER_PUBLISHER_HPP
