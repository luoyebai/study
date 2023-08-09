#ifndef INCLUDE_EVENT_MANGER_TOPIC_HPP
#define INCLUDE_EVENT_MANGER_TOPIC_HPP

// std
#include <any>
#include <bits/types/struct_timeval.h>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstddef>
#include <ctime>
#include <deque>
#include <memory>
#include <mutex>
#include <string>
#include <sys/time.h>
#include <thread>
#include <utility>
#include <vector>
// log
#include "log/log.hpp"

// 可以存放任意类型的动态容器,用于实现话题指针容器类
using VecAny = std::vector<std::any>;

/**
 * @brief 将普通的数据类型带上时间戳信息
 *
 * @tparam T 数据类型
 */
template <typename T> struct AnyData {
    timeval time_stamp;
    T data;
};

/**
 * @brief 线程休眠函数
 *
 * @param seconds 休眠时间
 */
void inline sleep(double seconds) noexcept {
    auto sec = static_cast<int16_t>(seconds);
    auto msec = static_cast<int16_t>((seconds - sec) * 1e3);
    std::this_thread::sleep_for(std::chrono::seconds(sec));
    std::this_thread::sleep_for(std::chrono::milliseconds(msec));
    return;
}

/**
 * @brief   同上
 *
 * @param seconds 同上
 */
void inline sleep(int seconds) noexcept {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    return;
}

/**
 * @brief 简单的定时器实现
 *
 */
class Timer {
  public:
    timeval time_now;
    // 两个时间点的差值
    double diff_time = 0.0;
    // 计时器更新是否成功
    bool is_time_ok = true;

    /**
     * @brief 初始化两个时间点
     *
     */
    Timer() {
        gettimeofday(&time_start_, nullptr);
        gettimeofday(&time_now, nullptr);
    }

    /**
     * @brief 更新时间点
     *
     * @param timing 时间间隔/可选----小于真实时间间隔则更新成功
     */
    inline void update(double timing = 1.0) {
        // 更新当前时间点
        gettimeofday(&time_now, nullptr);
        // 更新差值
        diff_time = getTimeDiff(time_start_, time_now);
        // 时间比较小时
        if (std::isnan(timing)) {
            // 更新成功
            is_time_ok = true;
            resetTimeStart();
            return;
        }
        is_time_ok = diff_time >= timing;
        // 更新成功则重置上一次的时间点
        if (is_time_ok)
            resetTimeStart();
    }

    /**
     * @brief 重置上次时间点,即重置在计时的初始时间点
     *
     */
    inline void resetTimeStart() { gettimeofday(&time_start_, nullptr); }

  private:
    // 计时的初始时间点
    timeval time_start_;

    /**
     * @brief 获得两个时间点的差值
     *
     * @param time_s  初始时间点
     * @param time_e  最终时间点
     * @return double 时间差----单位(s)
     */
    inline double getTimeDiff(timeval time_s, timeval time_e) {
        // 秒的差值
        auto sec_diff = time_e.tv_sec - time_s.tv_sec;
        // 微秒的差值
        auto usec_diff = time_e.tv_usec - time_s.tv_usec;
        return sec_diff + usec_diff * 1e-6;
    }
};

/**
 * @brief 话题类的实现
 *
 * @tparam T 容器内存放的数据类型
 */
template <typename T> class Topic {
  public:
    /**
     * @brief 构造新的话题
     *
     * @param name 话题名字
     */
    explicit Topic(std::string name) : name_(name) {}

    // 指向话题的指针类型
    using ShartPtr = std::shared_ptr<Topic<T>>;
    // 带有时间戳的数据类型
    using Data = AnyData<T>;

    // 订阅者发布者计数器
    uint16_t pubs_num = 0;
    uint16_t subs_num = 0;

    /**
     * @brief 获取话题名字
     *
     * @return std::string 名字
     */
    std::string getName() { return this->name_; }

    /**
     * @brief 向话题容器中存放数据
     *
     * @param data 数据
     */
    inline void pushData(const Data &data) {
        // 上锁
        std::lock_guard<std::mutex> lock(queue_mutex_);
        datas_.push_front(data);
        // 防止忙等
        queue_cv_.notify_one();
        return;
    }

    /**
     * @brief 取出数据
     *
     * @return auto
     */
    inline auto popData() {
        static bool is_loged = false;
        while (true) {
            if (is_loged && pubs_num != 0)
                is_loged = false;

            if (pubs_num == 0 && !is_loged) {
                logWarn('<', getName(), '>',
                        "没有发布者发布数据,容器内数据仅剩:", datas_.size());
                is_loged = true;
            }
            // 上锁
            std::lock_guard<std::mutex> lock(queue_mutex_);
            if (datas_.empty())
                continue;

            auto data = datas_.back();
            datas_.pop_back();
            return data;
        }
    }

    /**
     * @brief 获取容器当前的数据数量
     *
     * @return size_t
     */
    inline size_t getDataSize() { return datas_.size(); }

    /**
     * @brief 获取容器当前数据数量占目标数据数量的多少
     *
     * @param size  目标数据数量大小
     * @return double 返回百分比形式的占比(无百分号)
     */
    inline double getFillRate(size_t size) {
        return static_cast<double>(100 * getDataSize() / size);
    }

    /**
     * @brief 限制容器数据数量
     *
     * @param size 同上
     */
    inline void resize(size_t size) {
        double fill_rate = getFillRate(size);
        // size小10则不用打印该信息
        if (size >= 10) {
            if (fill_rate - 90 > 1e-6)
                logWarn('<', getName(), '>',
                        "话题内容器数据过多,可能出现丢失情况:", fill_rate,
                        "%填充率");
        }
        // 当填充占比大于100,限制大小
        if (fill_rate - 100.0 > 1e-6) {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            datas_.resize(size);
        }
        return;
    }

  private:
    std::string name_;
    std::deque<Data> datas_;
    std::mutex queue_mutex_;
    std::condition_variable queue_cv_;
};

// 话题指针类型
template <typename T> using TopicPtr = typename Topic<T>::ShartPtr;

/**
 * @brief 话题指针容器类,即订阅者发布者的通讯组件,目前使用static变量实现
 *
 */
class VecTopicPtr {
  public:
    /**
     * @brief 添加新的话题指针
     *
     * @tparam T 话题的容器类型
     * @param data 新的话题指针
     */
    template <typename T> static void addTopicPtr(const TopicPtr<T> &data) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::any var;
        try {
            var = data;
        } catch (std::exception &e) {
            logError('<', data->getName(), '>',
                     "话题指针添加发生异常:", e.what());
        }
        topic_ptr_vec_.push_back(var);
        return;
    }

    /**
     * @brief 找到对应名字的话题指针
     *
     * @tparam T 同上
     * @param name 话题名
     * @return auto 返回话题指针(没找到则为nullptr)
     */
    template <typename T> static auto findTopicPtr(const std::string &name) {
        TopicPtr<T> result_topic_ptr = nullptr;
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto &var : topic_ptr_vec_) {
            // 类型不同
            if (var.type() != typeid(result_topic_ptr))
                continue;
            try {
                result_topic_ptr = std::any_cast<TopicPtr<T>>(var);
            } catch (std::exception &e) {
                logError('<', name, '>', "话题指针查找发生异常:", e.what());
                continue;
            }
            if (result_topic_ptr->getName() == name)
                return result_topic_ptr;
        }
        TopicPtr<T> null_ptr = nullptr;
        return null_ptr;
    }

    /**
     * @brief 删除对应名字的话题指针
     *
     * @tparam T 同上
     * @param name 同上
     * @return true 找到话题并删除
     * @return false 没有找到话题
     */
    template <typename T> static bool removeTopicPtr(const std::string &name) {
        TopicPtr<T> result_topic_ptr = nullptr;
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto it = topic_ptr_vec_.begin(); it != topic_ptr_vec_.end();
             ++it) {
            // 类型不同
            if (it->type() != typeid(result_topic_ptr))
                continue;
            try {
                result_topic_ptr = std::any_cast<TopicPtr<T>>(*it);
            } catch (std::exception &e) {
                logError('<', name, '>', "话题指针移除发生异常:", e.what());
                continue;
            }
            if (result_topic_ptr->getName() != name)
                continue;
            it = topic_ptr_vec_.erase(it);
            return true;
        }
        return false;
    }

  private:
    static VecAny topic_ptr_vec_;
    static std::mutex mutex_;
};
// static的外部定义
VecAny VecTopicPtr::topic_ptr_vec_;
std::mutex VecTopicPtr::mutex_;

/**
 * @brief 订阅者发布者的基类
 *
 * @tparam T 订阅/发布的数据类型
 */
template <typename T> class BasePubSub {
  public:
    /**
     * @brief 构造新的基类实例
     *
     * @param name 该实例名字
     * @param topic_name 话题名字
     */
    explicit BasePubSub(const std::string &name, const std::string &topic_name)
        : name_(name), topic_name_(topic_name) {
        topicPtrInit();
    }

    /**
     * @brief 销毁基类实例
     *
     */
    ~BasePubSub() {
        // 话题的订阅者发布者计数器都归零
        if (topic_ptr_->pubs_num <= 0 && topic_ptr_->subs_num <= 0)
            topicPtrRemove();
    }

    // 当前订阅到的数据时间戳
    timeval now_time_stamp;

    /**
     * @brief 基础的回调模式,在死循环内进行回调
     *
     * @tparam CBF 回调函数类型
     * @tparam RF  响应函数类型
     * @param call_back_f 回调函数----返回值为bool,返回flase时退出循环回调
     * @param response_f 响应函数----返回值为bool,一旦返回true则开始回调
     */
    template <typename CBF, typename RF>
    inline void baseCallBack(CBF &&call_back_f, RF &&response_f) {
        while (1) {
            if (!response_f())
                continue;
            if (!call_back_f())
                break;
        }
        return;
    }

    /**
     * @brief 定时回调函数
     *
     * @tparam Func 回调函数类型
     * @param call_back_f 回调函数
     * @param speed 回调的速率
     */
    template <typename Func>
    void timerCallBack(Func &&call_back_f, double speed = 1e6) {
        baseCallBack(std::forward<Func>(call_back_f), [speed, this]() -> bool {
            // 通过计时器来判断是否回调
            this->timer_.update(1.0 / speed);
            return this->timer_.is_time_ok;
        });
        return;
    }

  protected:
    // 话题指针
    TopicPtr<T> topic_ptr_ = nullptr;
    // 两个定时器
    Timer timer_;
    Timer one_sec_timer_;
    // 统计帧率的计数器
    size_t count_ = 0;

    /**
     * @brief 返回订阅者或发布者名字
     *
     * @return std::string
     */
    std::string getName() { return name_; }

    /**
     * @brief 返回话题名字
     *
     * @return std::string
     */
    std::string getTopicName() { return topic_name_; }

    /**
     * @brief 返回 <名字/话题名字>
     *
     * @return std::string
     */
    std::string getLogger() {
        return '<' + getName() + '/' + getTopicName() + '>';
    }

    /**
     * @brief 通过话题指针容器对话题指针初始化
     *
     */
    inline void topicPtrInit() {
        topic_ptr_ = VecTopicPtr::findTopicPtr<T>(getTopicName());
        if (topic_ptr_ != nullptr) {
            logDebug(getLogger(), "找到对应话题,初始化完毕");
            return;
        }
        topic_ptr_ = std::make_shared<Topic<T>>(getTopicName());
        VecTopicPtr::addTopicPtr<T>(topic_ptr_);
        logDebug(getLogger(), "未找到目标话题,创建对应话题,初始化完毕");
        return;
    }

    /**
     * @brief
     * 从话题指针容器内移除话题指针,利用话题类对话题指针容器的迭代器进行保存,
     * 可以实现不用查找地删除,但是可能会导致之前保存的迭代器失效,所以这里使用遍历
     *
     */
    inline void topicPtrRemove() {
        logDebug(getLogger(), "订阅者和发布者都已注销,该话题将被删除");
        VecTopicPtr::removeTopicPtr<T>(getTopicName());
        return;
    }

  private:
    std::string name_;
    std::string topic_name_;
};

#endif // !INCLUDE_EVENT_MANGER_TOPIC_HPP
