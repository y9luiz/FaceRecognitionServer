#include "faceDetectionResponse.h"
#include "applicationMessages.h"
#include <serializer.h>

using cv::Rect2i;
using std::vector;

FaceDetectionResponseMessage::FaceDetectionResponseMessage(
    const vector<Rect2i> &payload)
    : ApplicationMessage({static_cast<uint8_t>(
                              ApplicationMessage::Types::FaceDetectionResponse),
                          static_cast<uint32_t>(payload.size()) *
                              static_cast<uint32_t>(sizeof(Rect2i))},
                         Serializer::VectorRectToBytes(payload)),
      m_boudingBoxes(payload) {}

FaceDetectionResponseMessage::FaceDetectionResponseMessage(
    vector<uint8_t> &&payload)
    : FaceDetectionResponseMessage(Serializer::VectorRectFromBytes(payload)) {}

vector<Rect2i> &FaceDetectionResponseMessage::facesBoudingBoxes() {
  return m_boudingBoxes;
}