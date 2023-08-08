/**
 * @file log_system.hpp
 * @author luoyebai (2112216825@qq.com)
 * @brief   简单的日志系统
 * @version 0.1
 * @date 2023-08-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef INCLUDE_LOG_SYSTEM_HPP
#define INCLUDE_LOG_SYSTEM_HPP

// std
#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <utility>

/**
 * @brief
 * 改变log颜色的宏
 */
#define LOG_SET(mode) std::cout << "\e[" << mode << "m";

/**
 * @brief   日志的各个等级
 * UNKNOW:  不清楚等级,通常不可见
 * DEBUG:   各个模块内部调试信息
 * INFO:    实际代码运行时的信息
 * WARN:    模块存在问题的警告信息
 * ERROR:   模块有错误的错误信息
 * FATAL:   程序运行失败的信息,一般与MODE_MAX大小相同
 */
static constexpr uint8_t KMODE_MAX = 5;
static constexpr uint8_t KUNKNOW = 0;
static constexpr uint8_t KDEBUG = 1;
static constexpr uint8_t KINFO = 2;
static constexpr uint8_t KWARN = 3;
static constexpr uint8_t KERROR = 4;
static constexpr uint8_t KFATAL = 5;

/**
 * @brief   时间类型宏
 * YEAR:    年
 * MOON:    月
 * MDAY:    月日[0-31]
 * YDAY:    年日[0-365]
 * WDAY:    周日[0-7]
 * HOUR:    小时
 * MIN:     分钟
 * SEC:     秒钟
 */
static constexpr uint8_t KYEAR = 0;
static constexpr uint8_t KMOON = 1;
static constexpr uint8_t KMDAY = 2;
static constexpr uint8_t KYDAY = 3;
static constexpr uint8_t KWDAY = 4;
static constexpr uint8_t KHOUR = 5;
static constexpr uint8_t KMIN = 5;
static constexpr uint8_t KSEC = 6;
static constexpr uint8_t KLOG_CLEAR_INTERVAL = 7;

/**
 * @brief 日志系统的实现,使用单例模式作为基础
 *
 */
class LogSystem {
  public:
    // 删除其他构造
    LogSystem(LogSystem &&) = delete;
    LogSystem(const LogSystem &) = delete;
    LogSystem &operator=(LogSystem &&) = delete;
    LogSystem &operator=(const LogSystem &) = delete;

    /**
     * @brief 获取实例
     *
     * @return shared_ptr 返回共享指针
     */
    static auto getInstance() {
        static std::shared_ptr<LogSystem> log_system = nullptr;
        static std::once_flag singleton_flag;
        std::call_once(singleton_flag, [] {
            log_system = std::shared_ptr<LogSystem>(new LogSystem());
        });
        return log_system;
    }

    // 打印一些结尾信息
    ~LogSystem() {
        for (const auto &file_name : file_names_) {
            auto file_path = dir_path + file_name;
            std::ofstream out_file;
            out_file.open(file_path, std::ios::out | std::ios::app);
            out_file << "----------------";
            out_file << "编译日期:" << __TIME__ << " " << __DATE__;
            out_file << "\t最后修改者[" << author << "]";
            out_file << "----------------\n";
            out_file.close();
        }
    }

    // 0-全部打印 1-info及以上 2-warn以上 3-error以上
    uint8_t log_level = 0;
    // 保存日志的文件夹
    std::string dir_path;
    // 本次代码修改者名字
    std::string author;

    /**
     * @brief 打印信息流
     *
     * @tparam T  折叠包类型
     * @param mode 打印信息的模式
     * @param fun 打印信息时所在函数
     * @param file 打印信息时所在文件
     * @param line 打印信息时所在文件行
     * @param message 信息
     */
    template <typename... Args>
    inline void logMsg(uint8_t mode, const char *fun, const char *file,
                       size_t line, Args &&...message) {
        std::unique_lock<std::mutex> lock(mutex_);
        writeLogFile(mode, fun, file, line, message...);
        lock.unlock();
        if (this->log_level > mode)
            return;
        lock.lock();
        LOG_SET(modes_[mode]);
        std::cout << strems_[mode].str();
        LOG_SET(0);
        lock.unlock();
        // 最高等级报错则退出程序
        if (mode == KMODE_MAX)
            exit(1);
        return;
    }

    /**
     * @brief 获取日志系统初始化时不同类型时间参数的值
     *
     * @param mode 时间类型
     * @return int 返回的时间结果
     */
    int getInitTime(uint8_t mode) {
        if (mode == KYEAR)
            return t_m_->tm_year + 1900;
        else if (mode == KMOON)
            return t_m_->tm_mon + 1;
        else if (mode == KMDAY)
            return t_m_->tm_mday;
        else if (mode == KYDAY)
            return t_m_->tm_yday;
        else if (mode == KWDAY)
            return t_m_->tm_wday;
        else if (mode == KHOUR)
            return t_m_->tm_hour;
        else if (mode == KMIN)
            return t_m_->tm_min;
        else if (mode == KSEC)
            return t_m_->tm_sec;
        else
            return -1;
    }

  private:
    // 防止被外部调用
    LogSystem() {
        using namespace std::literals;
        const auto now = std::chrono::system_clock::now();
        const std::time_t t_c = std::chrono::system_clock::to_time_t(now - 24h);
        t_m_ = std::localtime(&t_c);
    }

    // 模式对应的字体颜色
    std::array<uint, KMODE_MAX + 1> modes_{0, 32, 34, 33, 31, 41};
    // 模式对应开头
    std::array<const char *, KMODE_MAX + 1> out_start_{
        "[UKNOW]\t", "[DEBUG]\t", "[INFO]\t",
        "[WARN]\t",  "[ERROR]\t", "[FATAL]\t"};
    // 各个模式文件名
    const std::array<const char *, KMODE_MAX + 1> file_names_{
        "unknow.txt", "debug.txt", "info.txt",
        "warn.txt",   "error.txt", "fatal.txt"};
    // 时间
    tm *t_m_;
    // 各个模式文件数据流
    std::array<std::stringstream, KMODE_MAX + 1> strems_;
    // 锁
    std::mutex mutex_;

    /**
     * @brief 写入日志文件
     *
     * @tparam T 信息类型
     * @param mode 信息模式
     * @param fun 信息所在函数
     * @param file 信息所在b文件
     * @param line 信息所在文件行
     * @param message 信息
     */
    template <typename... Args>
    inline void writeLogFile(uint8_t mode, const char *fun, const char *file,
                             size_t line, Args &&...message) {
        // 清空字符串流
        strems_[mode].clear();
        strems_[mode].str("");
        const auto now = std::chrono::system_clock::now();
        using namespace std::literals;
        const std::time_t t_c = std::chrono::system_clock::to_time_t(now - 24h);
        auto file_path = dir_path + file_names_[mode];
        std::ofstream out_file;
        // 打开文件
        out_file.open(file_path, std::ios::out | std::ios::app);
        // 写入信息流
        strems_[mode] << out_start_[mode];
        strems_[mode] << std::fixed << std::setprecision(2);
        ((strems_[mode] << message), ...);
        strems_[mode] << '\t' << file << ':' << line;
        strems_[mode] << '\t' << fun;
        strems_[mode] << std::put_time(std::localtime(&t_c), "\t%F %T.\n");
        // 信息流写入文件
        out_file << strems_[mode].str();
        out_file.close();
    }
};

#endif // !INCLUDE_LOG_SYSTEM_HPP_
