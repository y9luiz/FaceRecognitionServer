#pragma once

#include "serializable.h"

#include <opencv2/opencv.hpp>

class Image : public Serializable {
public:
  Image() = default;
  Image(cv::Mat &&image);
  Image(std::vector<uint8_t> & data);
  
  cv::Mat &get();

  std::vector<uint8_t> serialize() const override;
  void deserialize(std::vector<uint8_t> &) override;

private:
  cv::Mat m_image;
};