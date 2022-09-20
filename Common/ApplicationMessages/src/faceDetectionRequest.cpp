#include "faceDetectionRequest.h"
#include "applicationMessages.h"
#include <serializer.h>

using cv::Mat;
using std::vector;

FaceDetectionRequestMessage::FaceDetectionRequestMessage(const Mat &image)
    : ApplicationMessage({static_cast<uint8_t>(
                              ApplicationMessage::Types::FaceDetectionRequest),
                          Serializer::getNumberOfBytes(image)},
                         Serializer::MatToBytes(image)),
      m_image(image.rows, image.cols, image.type(),
              reinterpret_cast<void *>(payload().data())) {}

FaceDetectionRequestMessage::FaceDetectionRequestMessage(
    vector<uint8_t> &&payload)
    : FaceDetectionRequestMessage(Serializer::MatFromBytes(payload)) {}

Mat &FaceDetectionRequestMessage::image() { return m_image; }