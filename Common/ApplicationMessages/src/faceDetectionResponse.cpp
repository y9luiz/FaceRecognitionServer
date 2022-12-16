#include "faceDetectionResponse.h"
#include "applicationMessages.h"
#include <serializer.h>

using cv::Rect2i;
using std::vector;

FaceDetectionResponseMessage::FaceDetectionResponseMessage(
    const vector<Rect2i> &payload):
      m_boudingBoxes(payload) {
    m_payload = reinterpret_cast<char *>(m_boudingBoxes.data());
}

FaceDetectionResponseMessage::FaceDetectionResponseMessage(
    vector<uint8_t> &&payload)
    : FaceDetectionResponseMessage(Serializer::vectorRectFromBytes(payload)) {}

vector<Rect2i> &FaceDetectionResponseMessage::facesBoudingBoxes() {
  return m_boudingBoxes;
}