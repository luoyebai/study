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
#include <cassert>
#include <cstdint>
#include <ctime>
#include <deque>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <utility>

/**
 * @brief
 * 改变log颜色的宏
 */
#ifndef CLOSE_COLOR
#define LOG_SET(mode) std::cout << "\e[" << mode << "m";
#endif // !CLOSE_COLOR

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

    // mode file fun time
    std::array<bool, 4> log_mode{1, 0, 0, 1};
    size_t data_max_len = 100;

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

    /**
     * @brief 程序运行结束后的日志
     *
     */
    ~LogSystem() {
        for (const auto &file_name : file_names_) {
            auto file_path = dir_path + file_name;
            std::ofstream out_file;
            out_file.open(file_path, std::ios::out | std::ios::app);
            std::string str;
            str += "编译日期";
            str += __TIME__;
            str += __DATE__;
            str += "\t最后修改者:[" + author + ']';
            const int total_w = data_max_len + 180;
            const int left_w = (total_w - str.length()) / 2;
            const int right_w = total_w - left_w - str.length();
            out_file << std::setw(left_w) << std::setfill('x') << "";
            out_file << str;
            out_file << std::setw(right_w) << std::setfill('x') << "";
            out_file << "\n\n";
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
     * @brief 循环
     *
     */
    void run() {
        assert(author != "" && dir_path != "");
        // 开头
        for (const auto &file_name : file_names_) {
            auto file_path = dir_path + file_name;
            std::ofstream out_file;
            out_file.open(file_path, std::ios::out | std::ios::app);
            out_file << '/' << std::setfill('x') << std::setw(12) << "时间"
                     << std::setw(10) << ""
                     << "\\";
            out_file << '/' << std::setfill('x')
                     << std::setw(data_max_len / 2 + 1) << "信息"
                     << std::setw(data_max_len / 2 - 1) << ""
                     << "\\";
            out_file << '/' << std::setfill('x') << std::setw(26) << "文件"
                     << std::setw(24) << ""
                     << "\\";
            out_file << '/' << std::setfill('x') << std::setw(51) << "函数"
                     << std::setw(49) << ""
                     << "\\";
            out_file << "\n";
            out_file.close();
        }
        // 死循环
        auto run_f = [&]() {
            while (1) {
                if (msgs_.empty())
                    continue;
                auto msg = msgs_.front();
                msgs_.pop_front();
                writeLogFile(msg);
                if (this->log_level > msg.mode)
                    continue;
                logMsg(msg);
            }
            return;
        };
        // 异步实现
        std::thread(run_f).detach();
        return;
    }

    /**
     * @brief 将信息加入队列
     *
     * @tparam Args... 形参包
     * @param mode 打印信息的模式
     * @param fun 打印信息时所在函数
     * @param file 打印信息时所在文件
     * @param line 打印信息时所在文件行
     * @param message 信息
     */
    template <typename... Args>
    inline void addMsg(uint8_t mode, const char *fun, const char *file,
                       size_t line, Args &&...message) {
        auto data = toString(message...);
        Msg new_msg{mode, fun, file, line, data};
        std::lock_guard<std::mutex> lock(mutex_);
        msgs_.push_back(new_msg);
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
    /**
     * @brief 构造时获取时间
     *
     */
    LogSystem() {
        using namespace std::literals;
        const auto now = std::chrono::system_clock::now();
        const std::time_t t_c = std::chrono::system_clock::to_time_t(now - 24h);
        t_m_ = std::localtime(&t_c);
    }

    // 存放数据的类型
    struct Msg {
        uint8_t mode;
        const char *fun;
        const char *file;
        const size_t line;
        std::string data;
    };

#ifndef CLOSE_COLOR
    // 模式对应的字体颜色
    std::array<uint, KMODE_MAX + 1> modes_{34, 32, 0, 33, 31, 41};
#endif // !CLOSE_COLOR

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
    // 锁
    std::mutex mutex_;
    bool is_log_end = true;
    std::deque<Msg> msgs_;

    /**
     * @brief 形参包数据转为字符串
     *
     * @tparam Args... 形参包
     * @param args 数据
     *
     * @return string 返回的字符串结果
     */
    template <typename... Args> inline std::string toString(Args &&...args) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ((ss << args), ...);
        return ss.str();
    }

    /**
     * @brief 格式化数据
     *
     * @param msg 信息
     * @param is_mode 是否打印模式
     * @param is_file  所在文件
     * @param is_fun  所在函数
     * @param is_time  时间
     *
     * @return string 返回的字符串结果
     */
    inline std::string format(const Msg &msg, bool is_mode = true,
                              bool is_file = true, bool is_fun = true,
                              bool is_time = true) {
        std::stringstream ss;

        if (is_mode)
            ss << out_start_[msg.mode];

        if (is_time) {
            const auto now = std::chrono::system_clock::now();
            using namespace std::literals;
            const std::time_t t_c =
                std::chrono::system_clock::to_time_t(now - 24h);
            ss << std::put_time(std::localtime(&t_c), "%F %T  |");
        }

        size_t show_data_len = data_max_len;
        size_t diff = getChineseCharacterNums(msg.data);
        show_data_len += diff;

        if (msg.data.length() > show_data_len) {
            show_data_len = msg.data.length();
        }

        ss << std::left << std::setfill(' ') << std::setw(show_data_len)
           << msg.data;

        if (is_file) {
            std::string file_name;
            file_name += msg.file;
            file_name += ':';
            file_name += std::to_string(msg.line);
            ss << std::left << std::setw(50) << file_name;
        }

        if (is_fun)
            ss << std::left << std::setw(100) << msg.fun;

        ss << '\n';
        return ss.str();
    }

    /**
     * @brief 打印信息流
     *
     * @param mode 模式
     */
    inline void logMsg(const Msg &msg) {
        std::lock_guard<std::mutex> lock(mutex_);

#ifndef CLOSE_COLOR
        LOG_SET(modes_[msg.mode]);
#endif // !CLOSE_COLOR

        std::cout << format(msg, log_mode[0], log_mode[1], log_mode[2],
                            log_mode[3]);

#ifndef CLOSE_COLOR
        LOG_SET(0);
#endif // !CLOSE_COLOR

        // 最高等级报错则退出程序
        if (msg.mode != KMODE_MAX)
            return;
        exit(-1);
        return;
    }

    /**
     * @brief 写入日志文件
     *
     * @param msg 信息
     */
    inline void writeLogFile(const Msg &msg) {
        auto file_path = dir_path + file_names_[msg.mode];
        std::ofstream out_file;
        std::lock_guard<std::mutex> lock(mutex_);
        // 打开文件
        out_file.open(file_path, std::ios::out | std::ios::app);
        // 写入文件
        out_file << format(msg, 0);
        out_file.close();
    }

    /**
     * @brief 得到字符串内非正常字母数字的数量
     *
     * @param str 字符串
     * @return size_t 字母数量
     */
    inline size_t getChineseCharacterNums(const std::string &str) {
        size_t nums = 0;
        for (auto c : str) {
            // 是在正常字母和数字
            if (c >= 33 && c <= 126)
                continue;
            ++nums;
        }
        // utf8中中文占3个字节
        return nums / 3;
    }
};

#endif // !INCLUDE_LOG_SYSTEM_HPP_
