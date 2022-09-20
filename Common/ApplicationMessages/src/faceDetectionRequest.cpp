#include "faceDetectionRequest.h"
#include "applicationMessages.h"
#include <serializer.h>

#include <stdexcept>

using cv::Mat;

using std::invalid_argument;
using std::vector;

FaceDetectionRequestMessage::FaceDetectionRequestMessage(const Mat &image)
    : ApplicationMessage({static_cast<uint8_t>(
                              ApplicationMessage::Types::FaceDetectionRequest),
                          Serializer::getNumberOfBytes(image)},
                         Serializer::MatToBytes(image)),
      m_image(image.rows, image.cols, image.type(),
              reinterpret_cast<void *>(payload().data())) {
  if (m_image.rows <= 0 || m_image.cols <= 0 || m_image.channels() <= 0) {
    throw invalid_argument(
        "Could not create Face Detection Request Message, image is null");
  }
}

FaceDetectionRequestMessage::FaceDetectionRequestMessage(
    vector<uint8_t> &&payload)
    : FaceDetectionRequestMessage(Serializer::MatFromBytes(payload)) {}

Mat &FaceDetectionRequestMessage::image() { return m_image; }