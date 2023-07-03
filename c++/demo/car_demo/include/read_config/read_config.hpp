/**
 * @file read_config.hpp
 * @author luoyebai (2112216825@qq.com)
 * @brief read config
 * @version 0.1
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef INCLUDE__READ_CONFIG_HPP
#define INCLUDE__READ_CONFIG_HPP

// std
#include <string>
// opencv
#include <opencv2/core.hpp>

#define LOGERR(message) printf("%s%s%s", "\n\e[31m", message, "\e[0m\n")
#define LOGDEBUG(message) printf("%s%s%s", "\n\e[32m", message, "\e[0m\n")
#define LOGWANNING(message) printf("%s%s%s", "\n\e[33m", message, "\e[0m\n")

class FileRead {
public:
  FileRead() = default;
  explicit FileRead(std::string file_path) {
    fs_.open(file_path.c_str(), cv::FileStorage::READ);
    if (!fs_.isOpened())
      puts("config file can't open!");
  }
  ~FileRead() = default;

  template <typename T>
  void getConfig(const char *config_name, T &config, T default_value = T{});

private:
  cv::FileStorage fs_;
};

template <typename T>
void FileRead::getConfig(const char *config_name, T &config, T default_value) {
  config = default_value;
  fs_[config_name] >> config;
  return;
}

#endif // !INCLUDE__READ_CONFIG_HPP=