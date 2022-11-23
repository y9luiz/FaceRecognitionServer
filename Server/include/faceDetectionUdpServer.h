#pragma once

#include "udpServer.h"

#include <opencv2/objdetect/face.hpp>

class FaceDetectionUdpServer : public UdpServer {
public:
  FaceDetectionUdpServer(const std::string &ip, uint16_t port);
  virtual ~FaceDetectionUdpServer() = default;

private:
  void setMessageHandler(std::unique_ptr<MessageHandler> messageHandler);

  cv::Ptr<cv::FaceDetectorYN> m_faceDetector;
};