#include "faceDetectionRequest.h"
#include "applicationMessages.h"
#include "utils.h"
#include <serializer.h>

#include <stdexcept>

using cv::Mat;

using std::back_inserter;
using std::copy;
using std::invalid_argument;
using std::vector;

using namespace utils;

FaceDetectionRequestMessage::FaceDetectionRequestMessage(const Mat &image)
    : ApplicationMessage(ApplicationMessage::Code::FaceDetectionRequest),
      m_image(image.clone()) {
  if (m_image.rows <= 0 || m_image.cols <= 0 || m_image.channels() <= 0) {
    throw invalid_argument(
        "Could not create Face Detection Request Message, image is null");
  }
}

FaceDetectionRequestMessage::FaceDetectionRequestMessage(
    vector<uint8_t> &&payload)
    : FaceDetectionRequestMessage(Serializer::matFromBytes(payload)) {}

const Mat &FaceDetectionRequestMessage::image() { return m_image; }

vector<uint8_t> FaceDetectionRequestMessage::serialize() const {
  vector<uint8_t> bytes;
  const auto totalMessageSize = sizeof(Header) + getNumberOfBytes(m_image);
  bytes.reserve(totalMessageSize);

  bytes.emplace_back(static_cast<uint8_t>(m_code));

  const auto payloadSizeBytes =
      Serializer::u32ToBytes(totalMessageSize - sizeof(Header));
  const auto imageBytes = Serializer::matToBytes(m_image);

  copy(payloadSizeBytes.begin(), payloadSizeBytes.end(), back_inserter(bytes));
  copy(imageBytes.begin(), imageBytes.end(), back_inserter(bytes));

  return bytes;
}