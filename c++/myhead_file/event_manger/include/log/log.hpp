/**
 * @file log.hpp
 * @author luoyebai (2112216825@qq.com)
 * @brief 日志系统的封装实现
 * @version 1.2
 * @date 2023-08-04
 *
 * @copyright Copyright (c) 2023
 *
 */

// luoyebai TODO:fatal退出后保存栈堆

#ifndef INCLUDE__LOG__HPP
#define INCLUDE__LOG__HPP

// std
#include <sys/types.h>

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
// log sys
#include "log_system.hpp"

/**
 * @brief log类宏,用来打印信息
 * LOG宏为其他log宏的基础
 * 7类log宏,等级依次上升,Fatal会退出程序
 * dlog保证任何等级日志下都能看到数据通常不要使用
 */
#define LOG(MODE, ...)                                                    \
    log_system_ptr->addMsg(MODE, __PRETTY_FUNCTION__, __FILE__, __LINE__, \
                           std::this_thread::get_id(), ##__VA_ARGS__)
#define LOG_UNKNOW(...) LOG(KUNKNOW, ##__VA_ARGS__)
#define LOG_DEBUG(...) LOG(KDEBUG, ##__VA_ARGS__)
#define LOG_INFO(...) LOG(KINFO, ##__VA_ARGS__)
#define LOG_WARN(...) LOG(KWARN, ##__VA_ARGS__)
#define LOG_ERROR(...) LOG(KERROR, ##__VA_ARGS__)
#define LOG_FATAL(...) LOG(KFATAL, ##__VA_ARGS__)
#define dlog(...) LOG(log_system_ptr->log_level, ##__VA_ARGS__)
// // 缩写
#define log_u(...) LOG_UNKNOW(__VA_ARGS__)
#define log_d(...) LOG_DEBUG(__VA_ARGS__)
#define log_i(...) LOG_INFO(__VA_ARGS__)
#define log_w(...) LOG_WARN(__VA_ARGS__)
#define log_e(...) LOG_ERROR(__VA_ARGS__)
#define log_f(...) LOG_FATAL(__VA_ARGS__)

/**
 * @brief 日志系统初始化
 *
 * @param name 本次修改代码者的名字
 * @param dir_path 日志目录路径/可选----默认放在父级目录下
 * @param level 日志等级/可选----默认INFO及以上
 * @param clear_timing 日志清理时间间隔/可选----默认3天(单位:h)
 */
void logSystemInit(const std::string &name, const std::string &dir_path = "../",
                   uint8_t level = 0, uint32_t clear_timing = 72,
                   size_t data_max_len = 100) {
    namespace fs = std::filesystem;
    using namespace std::chrono_literals;

    // 年月日时
    std::array<int, 4> ymdh{
        log_system_ptr->getInitTime(KYEAR), log_system_ptr->getInitTime(KMOON),
        log_system_ptr->getInitTime(KMDAY), log_system_ptr->getInitTime(KHOUR)};
    // 月日时,若只有个位则在十位添加0
    std::array<std::string, 4> ymdh_str;
    for (size_t i = 0; i < 4; ++i) {
        ymdh_str[i] = std::to_string(ymdh[i]);
        if (ymdh_str[i].length() == 1) ymdh_str[i] = '0' + ymdh_str[i];
    }

    // 日志路径初始化
    std::string log_dir_path =
        (*--dir_path.end() != '/') ? dir_path + '/' : dir_path;

    // 删除过期日志
    if (fs::exists(log_dir_path)) {
        for (const auto &entry : fs::directory_iterator(log_dir_path)) {
            if (!entry.is_directory()) continue;
            // 获取文件时间
            auto ftime =
                std::chrono::file_clock::to_sys(fs::last_write_time(entry));
            // 当前时间
            const auto now = std::chrono::system_clock::now();
            auto diff_hours = (now - ftime) / 1h;
            if (diff_hours > clear_timing) fs::remove_all(entry);
        }
    }

    // 创建log目录
    log_dir_path += ymdh_str[0] + '-' + ymdh_str[1] + '-' + ymdh_str[2] + '-' +
                    ymdh_str[3] + '/';
    if (!fs::exists(log_dir_path)) fs::create_directories(log_dir_path);

    // 日志系统属性赋值
    log_system_ptr->author = name;
    log_system_ptr->dir_path = log_dir_path;
    log_system_ptr->log_level = (level > KMODE_MAX) ? KMODE_MAX : level;
    log_system_ptr->data_max_len = data_max_len;
    log_system_ptr->run();
}

#endif  // !INCLUDE__LOG__HPP
