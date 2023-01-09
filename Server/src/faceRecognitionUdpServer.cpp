#include "faceRecognitionUdpServer.h"
#include <assets.h>
#include <faceDetectionRequest.h>
#include <faceDetectionResponse.h>
#include <faceRecognitionRequest.h>
#include <faceRecognitionResponse.h>

#include <opencv2/core.hpp>

#include <iostream>
#include <memory>
#include <stdexcept>

using namespace assets::models;

using cv::FaceDetectorYN;
using cv::Mat;
using cv::Rect2i;
using cv::Size;

using std::cout;
using std::logic_error;
using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;
using std::vector;

FaceRecognitionUdpServer::FaceRecognitionUdpServer(const string &ip,
                                                   uint16_t port)
    : UdpServer(ip, port) {

  cout << "Face recognition UDP server running at: " << ip << ":" << port
       << "\n";

  m_faceDetector =
      FaceDetectorYN::create(FACE_DETECTION_MODEL, "", Size(320, 240));

  m_faceRecognizer = cv::FaceRecognizerSF::create(FACE_RECOGITION_MODEL, "");

  m_messageHandler = make_unique<MessageHandler>();
  m_messageHandler->registerCallback(
      static_cast<uint8_t>(ApplicationMessage::Code::FaceDetectionRequest),
      [this](auto message, const Endpoint &endpoint) {
        processFaceDetectionMessage(move(message), endpoint);
        cout << "Face detection request processed!\n";
      });

  m_messageHandler->registerCallback(
      static_cast<uint8_t>(ApplicationMessage::Code::FaceRecognitionRequest),
      [this](auto message, const Endpoint &endpoint) {
        processFaceRecognitionMessage(move(message), endpoint);
        cout << "Face recognition request processed!\n";
      });
}

void FaceRecognitionUdpServer::processFaceDetectionMessage(
    unique_ptr<ApplicationMessage> message, const Endpoint &endpoint) {
  auto request = static_cast<FaceDetectionRequestMessage *>(message.get());

  auto image = request->image();

  if (m_faceDetector) {
    Mat faces;
    m_faceDetector->setInputSize(image.size());
    m_faceDetector->detect(image, faces);

    if (faces.rows >= 0) {
      vector<Rect2i> boundingBoxes;
      boundingBoxes.reserve(faces.rows);
      for (int i = 0; i < faces.rows; i++) {
        boundingBoxes.emplace_back(
            Rect2i(faces.at<float>(i, 0), faces.at<float>(i, 1),
                   faces.at<float>(i, 2), faces.at<float>(i, 3)));
      }
      auto response = make_unique<FaceDetectionResponseMessage>(boundingBoxes);
      if (!m_messageSender) {
        throw logic_error("Could not send application message."
                          "UDP Message Sender is null.");
      }

      m_messageSender->sendMessage(move(response), endpoint);
    }
  }
}

void FaceRecognitionUdpServer::processFaceRecognitionMessage(
    unique_ptr<ApplicationMessage> message, const Endpoint &endpoint) {
  auto request = static_cast<FaceRecognitionRequestMessage *>(message.get());

  if (m_faceRecognizer) {
    const auto boudingBoxes = request->facesBoudingBoxes();
    vector<Mat> descriptors(boudingBoxes.size());
    for (int i = 0; i < boudingBoxes.size(); i++) {
      const auto face = Mat(request->image(), boudingBoxes[i]);
      m_faceRecognizer->feature(face, descriptors[i]);
    }
    if (!descriptors.empty()) {
      if (!m_messageSender) {
        throw logic_error("Could not send application message."
                          "UDP Message Sender is null.");
      }

      auto response = make_unique<FaceRecognitionResponseMessage>(descriptors);

      m_messageSender->sendMessage(move(response), endpoint);
    }
  }
}