#include "faceRecognitionUdpClient.h"

#include <faceDetectionRequest.h>
#include <faceDetectionResponse.h>
#include <faceRecognitionRequest.h>
#include <faceRecognitionResponse.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using cv::imread;
using cv::imshow;
using cv::rectangle;
using cv::waitKey;
using std::cout;
using std::invalid_argument;
using std::make_unique;
using std::move;
using std::runtime_error;

FaceRecognitionUdpClient::FaceRecognitionUdpClient(const std::string &ipAddress,
                                               uint16_t port)
    : UdpClient(ipAddress, port) {}

void FaceRecognitionUdpClient::run(int argc, char *argv[]) {
  if (argc != 2) {
    throw invalid_argument("./Client <image_path>");
  }

  const auto imagePath = argv[1];
  const auto image = imread(imagePath);

  auto faceDetectionRequest =
      make_unique<FaceDetectionRequestMessage>(image.clone());
  sendMessage(move(faceDetectionRequest));

  auto response = receiveMessage();
  if (response->code() != ApplicationMessage::Code::FaceDetectionResponse) {
    throw runtime_error("invalid message response");
  }

  auto faceDetectionResponse =
      static_cast<FaceDetectionResponseMessage *>(response.get());

  for (const auto boundingBox : faceDetectionResponse->facesBoudingBoxes()) {
    rectangle(image, boundingBox, cv::Scalar(255, 0, 0));
  }

  auto faceRecognitionRequest = make_unique<FaceRecognitionRequestMessage>(
      make_pair(image, faceDetectionResponse->facesBoudingBoxes()));

  sendMessage(move(faceRecognitionRequest));

  response = receiveMessage();
  if (response->code() != ApplicationMessage::Code::FaceRecognitionResponse) {
    throw runtime_error("invalid message response");
  }

  auto faceRecognitionResponse =
      static_cast<FaceRecognitionResponseMessage *>(response.get());

  cout << "\033[1;31mFace recognition response received\n";
  cout << "This feature is not complete yet, the client must implement a "
          "internal database"
          " and match the incoming descriptors with the stored ones\n";

  cout << "\033[1;m";

  auto descriptors = faceRecognitionResponse->descriptors();
  if (!descriptors.empty()) {
    cout << "Descriptors shape:\n";
    for (const auto &descriptor : descriptors) {
      cout << "\t" << descriptor.size << "\n";
    }
    cout << "\n";
  }
}