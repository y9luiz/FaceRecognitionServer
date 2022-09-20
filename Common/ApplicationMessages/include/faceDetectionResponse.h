#pragma once

#include "applicationMessages.h"

#include <opencv2/core/types.hpp>

class FaceDetectionResponseMessage : public ApplicationMessage {
public:
  FaceDetectionResponseMessage(const std::vector<cv::Rect> &payload);
  FaceDetectionResponseMessage(std::vector<uint8_t> &&payload);

  std::vector<cv::Rect> &facesBoudingBoxes();

private:
  std::vector<cv::Rect> m_boudingBoxes;
};