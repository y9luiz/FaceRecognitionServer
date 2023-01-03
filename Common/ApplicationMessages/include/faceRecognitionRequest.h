#pragma once

#include "applicationMessages.h"

#include <opencv2/core/mat.hpp>

class FaceRecognitionRequestMessage : public ApplicationMessage {
public:
  using PayloadT = std::pair<cv::Mat /*Image*/,std::vector<cv::Rect2i>/*Bouding Boxes*/>;

  FaceRecognitionRequestMessage(const PayloadT &payload);
  FaceRecognitionRequestMessage(std::vector<uint8_t> &&payload);

  std::vector<uint8_t> serialize() const override;

  const cv::Mat &image() const;

  const std::vector<cv::Rect2i> &facesBoudingBoxes() const;

private:
  cv::Mat m_image;
  std::vector<cv::Rect2i> m_facesBoudingBoxes;
};