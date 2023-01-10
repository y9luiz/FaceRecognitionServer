#pragma once

#include "udpServer.h"

#include <opencv2/objdetect/face.hpp>

class FaceRecognitionUdpServer : public UdpServer {
public:
  FaceRecognitionUdpServer(const std::string &ip, uint16_t port);
  virtual ~FaceRecognitionUdpServer() = default;

private:
  void setMessageHandler(std::unique_ptr<MessageHandler> messageHandler);

  void processFaceDetectionMessage(std::unique_ptr<ApplicationMessage>,
                                   const Endpoint &endpoint);
  void processFaceRecognitionMessage(std::unique_ptr<ApplicationMessage>,
                                     const Endpoint &endpoint);

  cv::Ptr<cv::FaceDetectorYN> m_faceDetector;
  cv::Ptr<cv::FaceRecognizerSF> m_faceRecognizer;
};