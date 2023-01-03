#pragma once

#include <cstdint>
#include <opencv2/core.hpp>
#include <vector>

namespace utils {
uint32_t getNumberOfBytes(const cv::Mat &image);
uint32_t getNumberOfBytes(const std::vector<cv::Rect2i> &boudingBoxes);
} // namespace utils