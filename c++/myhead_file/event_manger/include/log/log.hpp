/**
 * @file log.hpp
 * @author luoyebai (2112216825@qq.com)
 * @brief 日志系统的封装实现
 * @version 0.1
 * @date 2023-08-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef INCLUDE__LOG__HPP
#define INCLUDE__LOG__HPP

// std
#include <chrono>
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
#include <sys/types.h>
// log sys
#include "log_system.hpp"

static std::mutex log_sys_mutex;
static std::shared_ptr<LogSystem> log_system = nullptr;

#define LOG(MODE, ...)                                                         \
    log_system->logMsg(MODE, __PRETTY_FUNCTION__, __FILE__, __LINE__,          \
                       ##__VA_ARGS__)

#define logUnknow(...) LOG(KUNKNOW, ##__VA_ARGS__)
#define logDebug(...) LOG(KDEBUG, ##__VA_ARGS__)
#define logInfo(...) LOG(KINFO, ##__VA_ARGS__)
#define logWarn(...) LOG(KWARN, ##__VA_ARGS__)
#define logError(...) LOG(KERROR, ##__VA_ARGS__)
#define logFatal(...) LOG(KFATAL, ##__VA_ARGS__)
#define logExit(...) LOG(KFATAL + 1, ##__VA_ARGS__)
// 默认log----当前等级
#define dlog(...) LOG(log_system->log_level, ##__VA_ARGS__)

void logSystemInit(const std::string &name, const std::string &dir_path = "../",
                   uint8_t level = 0, uint32_t clear_timing = 72) {

    if (log_system == nullptr) {
        std::lock_guard<std::mutex> lock(log_sys_mutex);
        if (log_system == nullptr) {
            auto temp = std::shared_ptr<LogSystem>(new LogSystem());
            log_system = temp;
        }
    }

    namespace fs = std::filesystem;
    using namespace std::chrono_literals;

    // 年月日时
    std::array<int, 4> ymdh{
        log_system->getInitTime(KYEAR), log_system->getInitTime(KMOON),
        log_system->getInitTime(KMDAY), log_system->getInitTime(KHOUR)};

    std::array<std::string, 4> ymdh_str;
    for (size_t i = 0; i < 4; ++i) {
        ymdh_str[i] = std::to_string(ymdh[i]);
        if (ymdh_str[i].length() == 1)
            ymdh_str[i] = '0' + ymdh_str[i];
    }

    std::string log_dir_path =
        (*--dir_path.end() != '/') ? dir_path + '/' : dir_path;

    // 删除过期日志
    if (fs::exists(log_dir_path)) {
        for (const auto &entry : fs::directory_iterator(log_dir_path)) {
            if (!entry.is_directory())
                continue;
            // 获取文件时间
            auto ftime =
                std::chrono::file_clock::to_sys(fs::last_write_time(entry));
            // 当前时间
            const auto now = std::chrono::system_clock::now();
            auto diff_hours = (now - ftime) / 1h;
            if (diff_hours > clear_timing)
                fs::remove_all(entry);
        }
    }

    // 创建log目录
    log_dir_path += ymdh_str[0] + '-' + ymdh_str[1] + '-' + ymdh_str[2] + '-' +
                    ymdh_str[3] + '/';
    if (!fs::exists(log_dir_path))
        fs::create_directories(log_dir_path);

    log_system->author = name;
    log_system->dir_path = log_dir_path;
    log_system->log_level = (level > KMODE_MAX) ? KMODE_MAX : level;
}

#endif // !INCLUDE__LOG__HPP
