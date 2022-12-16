#include "faceDetectionRequest.h"
#include "applicationMessages.h"
#include <serializer.h>

#include <stdexcept>

using cv::Mat;

using std::invalid_argument;
using std::vector;

FaceDetectionRequestMessage::FaceDetectionRequestMessage(const Mat &image)
    : m_image(image){
  if (m_image.rows <= 0 || m_image.cols <= 0 || m_image.channels() <= 0) {
    throw invalid_argument(
        "Could not create Face Detection Request Message, image is null");
  }
  m_payload = reinterpret_cast<char *>(m_image.data);
}

FaceDetectionRequestMessage::FaceDetectionRequestMessage(
    vector<uint8_t> &&payload)
    : FaceDetectionRequestMessage(Serializer::matFromBytes(payload)) {}

Mat &FaceDetectionRequestMessage::image() { return m_image; }