#pragma once

#include "applicationMessages.h"

#include <opencv2/opencv.hpp>
#include <vector>

class FaceRecognitionResponseMessage : public ApplicationMessage {
public:
  using PayloadT = std::vector<cv::Mat>;

  FaceRecognitionResponseMessage(const PayloadT &payload);
  FaceRecognitionResponseMessage(std::vector<uint8_t> &&payload);

  std::vector<uint8_t> serialize() const override;

  const PayloadT &descriptors() const;

private:
  std::vector<cv::Mat> m_descriptors;
};