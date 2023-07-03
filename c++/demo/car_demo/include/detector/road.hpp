/**
 * @file road.hpp
 * @author luoyebai (2112216825@qq.com)
 * @brief road struct
 * @version 0.1
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef INCLUDE_EXCHANGE_STATION_HPP_
#define INCLUDE_EXCHANGE_STATION_HPP_

// std
#include <vector>
// opencv
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>

/**
 * @brief round have color,center,radius
 *
 */
struct Round {
  uint8_t color = 0;
  cv::Point center;
  float radius;
};

/**
 * @brief Arrows require input of contours and three constraint parameters
 *
 */
struct Arrow : public cv::RotatedRect {
  Arrow() = default;
  explicit Arrow(std::vector<cv::Point> contour, double min_ratio,
                 double max_ratio, int min_area)
      : cv::RotatedRect(cv::minAreaRect(contour)) {
    auto box = cv::minAreaRect(contour);
    auto ratio = box.boundingRect2f().width / box.boundingRect2f().height;
    auto area = cv::contourArea(contour);
    // Constraints that arrows need to comply with
    this->is_arrow = ratio > min_ratio && ratio < max_ratio && area > min_area;
    // centroid
    auto mu = cv::moments(contour);
    auto cx = static_cast<int>(mu.m10 / mu.m00);
    // Judging left and right
    left_or_right = static_cast<uint8_t>(cx - box.center.x < 0);
    this->contour = contour;
  }
  bool is_arrow;
  uint8_t color = 1;
  // left-0 right-1
  uint8_t left_or_right;
  std::vector<cv::Point> contour;
};

/**
 * @brief Traffic light status
 *
 */
struct TrafficLight {
  TrafficLight() = default;
  ~TrafficLight() = default;
  enum State { NONE, STOP, LEFT_RUN, RIGHT_RUN };
  Round round;
  Arrow arrow;
  // red-0 green-1
  uint8_t color;
  State state;
};

/**
 * @brief Lines on both sides of the road way
 *
 */
struct Roadway {
  Roadway() = default;
  ~Roadway() = default;
  struct Line {
    std::vector<cv::Point> contour;
    cv::Point center;
  };
  explicit Roadway(std::vector<cv::Point> contour1,
                   std::vector<cv::Point> contour2) {
    left_line.contour = contour1;
    right_line.contour = contour2;
    auto left_rect = cv::minAreaRect(contour1);
    auto right_rect = cv::minAreaRect(contour2);
    left_line.center = left_rect.center;
    right_line.center = right_rect.center;
    bool is_y_close = abs(left_line.center.y - right_line.center.y) < 100;
    // The y  of the two line must be close
    is_road_ok = is_y_close;
    center_x = (left_line.center.x + right_line.center.x) / 2;
  }
  bool is_road_ok;
  Line left_line;
  Line right_line;
  int center_x;
};

#endif // !INCLUDE_EXCHANGE_STATION_HPP_
