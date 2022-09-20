#pragma once

#include <endpoint.h>

#include <cstdint>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <vector>

class Serializer {
public:
  static std::vector<uint8_t> u16ToBytes(uint16_t val);
  static uint16_t u16FromBytes(const std::vector<uint8_t> &bytes);
  static std::vector<uint8_t> u32ToBytes(uint32_t val);
  static uint32_t u32FromBytes(const std::vector<uint8_t> &bytes);
  static std::vector<uint8_t> stringToBytes(const std::string &str);
  static std::string stringFromBytes(std::vector<uint8_t> &bytes);

  static void concatenate(std::vector<uint8_t> &left,
                          const std::vector<uint8_t> &right);
  static std::vector<uint8_t> concatenate(const std::vector<uint8_t> &left,
                                          const std::vector<uint8_t> &right);

  static uint32_t getNumberOfBytes(const cv::Mat &image);
  static std::vector<uint8_t> MatToBytes(const cv::Mat &image);
  static cv::Mat MatFromBytes(std::vector<uint8_t> &bytes);
  static std::vector<uint8_t> RectToBytes(const cv::Rect2i &rect);
  static cv::Rect2i RectFromBytes(std::vector<uint8_t> &bytes);
  static std::vector<uint8_t>
  VectorRectToBytes(const std::vector<cv::Rect2i> &rects);
  static std::vector<cv::Rect2i>
  VectorRectFromBytes(std::vector<uint8_t> &bytes);

  static std::vector<uint8_t> EndpointToBytes(const Endpoint &endpoint);
  static Endpoint EndpointFromBytes(std::vector<uint8_t> &bytes);
};