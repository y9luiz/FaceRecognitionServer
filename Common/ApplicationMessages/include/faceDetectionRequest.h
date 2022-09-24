#pragma once

#include "applicationMessages.h"

#include <opencv2/core/mat.hpp>

class FaceDetectionRequestMessage : public ApplicationMessage {
public:
  FaceDetectionRequestMessage(const cv::Mat &payload);
  FaceDetectionRequestMessage(std::vector<uint8_t> &&payload);

  cv::Mat &image();

private:
  cv::Mat m_image;
};