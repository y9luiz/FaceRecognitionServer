#pragma once

#include <endpoint.h>

#include <cstdint>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <vector>

class Serializer {
public:
  static std::vector<uint8_t> u16ToBytes(uint16_t val);
  static uint16_t u16FromBytes(std::vector<uint8_t> &bytes);
  static std::vector<uint8_t> u32ToBytes(uint32_t val);
  static uint32_t u32FromBytes(std::vector<uint8_t> &bytes);
  static std::vector<uint8_t> stringToBytes(const std::string &str);
  static std::string stringFromBytes(std::vector<uint8_t> &bytes);

  static std::vector<uint8_t> matToBytes(const cv::Mat &image);
  static cv::Mat matFromBytes(std::vector<uint8_t> &bytes);
  static std::vector<uint8_t> rectToBytes(const cv::Rect2i &rect);
  static cv::Rect2i rectFromBytes(std::vector<uint8_t> &bytes);
  static std::vector<uint8_t>
  vectorRectToBytes(const std::vector<cv::Rect2i> &rects);
  static std::vector<cv::Rect2i>
  vectorRectFromBytes(std::vector<uint8_t> &bytes);
};