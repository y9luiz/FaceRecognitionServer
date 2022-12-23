#include "faceDetectionResponse.h"
#include "applicationMessages.h"
#include <serializer.h>

using cv::Rect2i;
using std::vector;

namespace {
size_t getNumberOfBytes(const vector<Rect2i> &payload) {
  return sizeof(Rect2i) * payload.size() + sizeof(uint32_t);
}

} // namespace

FaceDetectionResponseMessage::FaceDetectionResponseMessage(
    const vector<Rect2i> &payload)
    : ApplicationMessage(ApplicationMessage::Code::FaceDetectionResponse),
      m_boudingBoxes(payload) {}

FaceDetectionResponseMessage::FaceDetectionResponseMessage(
    vector<uint8_t> &&payload)
    : FaceDetectionResponseMessage(Serializer::vectorRectFromBytes(payload)) {}

const vector<Rect2i> &FaceDetectionResponseMessage::facesBoudingBoxes() {
  return m_boudingBoxes;
}

vector<uint8_t> FaceDetectionResponseMessage::serialize() const {

  vector<uint8_t> bytes;
  const auto totalMessageSize =
      sizeof(ApplicationMessage::Header) + getNumberOfBytes(m_boudingBoxes);
  bytes.reserve(totalMessageSize);

  bytes.emplace_back(static_cast<uint8_t>(m_code));

  const auto payloadSizeBytes =
      Serializer::u32ToBytes(totalMessageSize - sizeof(ApplicationMessage::Header));
  const auto rectsBytes = Serializer::vectorRectToBytes(m_boudingBoxes);

  copy(payloadSizeBytes.begin(), payloadSizeBytes.end(), back_inserter(bytes));
  copy(rectsBytes.begin(), rectsBytes.end(), back_inserter(bytes));

  return bytes;
}