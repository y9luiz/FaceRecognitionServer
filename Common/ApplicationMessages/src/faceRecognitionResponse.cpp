#include "faceRecognitionResponse.h"
#include "utils.h"

#include <serializer.h>

#include <iostream>

using cv::Mat;
using std::back_inserter;
using std::copy;
using std::vector;

namespace {
FaceRecognitionResponseMessage::PayloadT
parsePayload(vector<uint8_t> &&payload) {
  const auto numberOfDescriptors = Serializer::u32FromBytes(payload);
  vector<Mat> descriptors;

  for (int i = 0; i < numberOfDescriptors; i++) {
    descriptors.emplace_back(Serializer::matFromBytes(payload));
  }

  return descriptors;
}
} // namespace

FaceRecognitionResponseMessage::FaceRecognitionResponseMessage(
    const PayloadT &payload)
    : ApplicationMessage(Code::FaceRecognitionResponse),
      m_descriptors(payload) {}

FaceRecognitionResponseMessage::FaceRecognitionResponseMessage(
    vector<uint8_t> &&payload)
    : FaceRecognitionResponseMessage(parsePayload(move(payload))) {}

vector<uint8_t> FaceRecognitionResponseMessage::serialize() const {
  vector<uint8_t> bytes;

  const auto totalMessageSize = sizeof(ApplicationMessage::Header) +
                                utils::getNumberOfBytes(m_descriptors);

  bytes.reserve(totalMessageSize);

  bytes.emplace_back(static_cast<uint8_t>(m_code));

  const auto payloadSizeBytes =
      Serializer::u32ToBytes(totalMessageSize - sizeof(Header));
  copy(payloadSizeBytes.begin(), payloadSizeBytes.end(), back_inserter(bytes));

  const auto numberOfDescriptorsBytes =
      Serializer::u32ToBytes(m_descriptors.size());
  copy(numberOfDescriptorsBytes.begin(), numberOfDescriptorsBytes.end(),
       back_inserter(bytes));

  for (const auto &descriptor : m_descriptors) {
    auto descriptorBytes = Serializer::matToBytes(descriptor);
    copy(descriptorBytes.begin(), descriptorBytes.end(), back_inserter(bytes));
  }

  return bytes;
}

const FaceRecognitionResponseMessage::PayloadT &
FaceRecognitionResponseMessage::descriptors() const {
  return m_descriptors;
}