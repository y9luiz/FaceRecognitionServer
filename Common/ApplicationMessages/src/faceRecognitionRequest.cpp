#include "faceRecognitionRequest.h"
#include "utils.h"
#include <serializer.h>

#include <stdexcept>

using cv::Mat;
using cv::Rect2i;
using std::invalid_argument;
using std::make_pair;
using std::pair;
using std::vector;

using namespace utils;

namespace {
FaceRecognitionRequestMessage::PayloadT
parsePayload(vector<uint8_t> &&payload) {
  auto image = Serializer::matFromBytes(payload);
  auto boudingBoxes = Serializer::vectorRectFromBytes(payload);

  return make_pair(image, boudingBoxes);
}
} // namespace

FaceRecognitionRequestMessage::FaceRecognitionRequestMessage(
    const FaceRecognitionRequestMessage::PayloadT &payload)
    : ApplicationMessage(Code::FaceRecognitionRequest), m_image(payload.first),
      m_facesBoudingBoxes(payload.second) {

  if (m_image.rows <= 0 || m_image.cols <= 0 || m_image.channels() <= 0) {
    throw invalid_argument(
        "Could not create Face Recognition Request Message, image is null");
  }

  if (m_facesBoudingBoxes.empty()) {
    throw invalid_argument("Could not create Face Recognition Request Message, "
                           "no bouding boxes provided");
  }
}

FaceRecognitionRequestMessage::FaceRecognitionRequestMessage(
    vector<uint8_t> &&payload)
    : FaceRecognitionRequestMessage(parsePayload(move(payload))) {}

vector<uint8_t> FaceRecognitionRequestMessage::serialize() const {
  vector<uint8_t> bytes;
  const auto totalMessageSize = sizeof(Header) + getNumberOfBytes(m_image) +
                                getNumberOfBytes(m_facesBoudingBoxes);
  bytes.reserve(totalMessageSize);

  bytes.emplace_back(static_cast<uint8_t>(m_code));

  const auto payloadSizeBytes =
      Serializer::u32ToBytes(totalMessageSize - sizeof(Header));
  copy(payloadSizeBytes.begin(), payloadSizeBytes.end(), back_inserter(bytes));

  const auto imageBytes = Serializer::matToBytes(m_image);
  copy(imageBytes.begin(), imageBytes.end(), back_inserter(bytes));

  const auto faceBoudingBoxesBytes =
      Serializer::vectorRectToBytes(m_facesBoudingBoxes);
  copy(faceBoudingBoxesBytes.begin(), faceBoudingBoxesBytes.end(),
       back_inserter(bytes));

  return bytes;
}

const Mat &FaceRecognitionRequestMessage::image() const { return m_image; }

const vector<Rect2i> &FaceRecognitionRequestMessage::facesBoudingBoxes() const {
  return m_facesBoudingBoxes;
}