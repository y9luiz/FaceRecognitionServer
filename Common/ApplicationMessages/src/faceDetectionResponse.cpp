#include "faceDetectionResponse.h"
#include "applicationMessages.h"
#include <serializer.h>

using cv::Rect2i;
using std::vector;

namespace {
size_t getNumberOfBytes(const vector<Rect2i> &payload) {
  return sizeof(Rect2i) * payload.size();
}

} // namespace

FaceDetectionResponseMessage::FaceDetectionResponseMessage(
    const vector<Rect2i> &payload)
    : ApplicationMessage(ApplicationMessage::Code::FaceDetectionResponse),
      m_boudingBoxes(payload) {}

FaceDetectionResponseMessage::FaceDetectionResponseMessage(
    vector<uint8_t> &&payload)
    : FaceDetectionResponseMessage(Serializer::vectorRectFromBytes(payload)) {}

vector<Rect2i> &FaceDetectionResponseMessage::facesBoudingBoxes() {
  return m_boudingBoxes;
}

vector<uint8_t> FaceDetectionResponseMessage::serialize() const {

  vector<uint8_t> bytes;
  const auto totalMessageSize =
      sizeof(Code) + sizeof(uint32_t) + getNumberOfBytes(m_boudingBoxes);
  bytes.reserve(totalMessageSize);

  bytes.emplace_back(static_cast<uint8_t>(m_code));

  const auto payloadSize =
      Serializer::u32ToBytes(totalMessageSize - sizeof(Code));
  const auto rectsBytes = Serializer::vectorRectToBytes(m_boudingBoxes);

  copy(payloadSize.begin(), payloadSize.end(), back_inserter(bytes));
  copy(rectsBytes.begin(), rectsBytes.end(), back_inserter(bytes));

  return bytes;
}