#include "faceDetectionUdpServer.h"
#include "faceDetectionRequest.h"
#include "faceDetectionResponse.h"
#include "messageHandler.h"
#include <assets.h>

#include <memory>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
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
using std::vector;

FaceDetectionUdpServer::FaceDetectionUdpServer(const string &ip, uint16_t port)
    : UdpServer(ip, port) {
  m_faceDetector =
      FaceDetectorYN::create(FACE_DETECTION_MODEL, "", Size(320, 240));

  m_messageHandler = make_unique<MessageHandler>();
  m_messageHandler->registerCallback(
      static_cast<uint8_t>(ApplicationMessage::Types::FaceDetectionRequest),
      [this](ApplicationMessage &&message, const Endpoint &endpoint) {
        FaceDetectionRequestMessage request(move(message.payload()));

        auto image = request.image();

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
            FaceDetectionResponseMessage response(boundingBoxes);
            if (!m_messageSender) {
              throw logic_error("Could not send application message."
                                "UDP Message Sender is null.");
            }

            m_messageSender->sendMessage(move(response), endpoint);
          }
        }
        cout << "callback finished\n";
      });
}