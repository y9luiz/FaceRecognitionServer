#include "applicationMessages.h"
#include "faceDetectionRequest.h"
#include "faceDetectionResponse.h"

#include <serializer.h>

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

using std::back_inserter;
using std::copy;
using std::move;
using std::size_t;
using std::vector;
using std::make_unique;
using std::unique_ptr;

vector<uint8_t> ApplicationMessage::Header::toBytes() const {
  vector<uint8_t> bytes;
  bytes.reserve(sizeof(Header));

  bytes.emplace_back(code);

  auto payloadSizeInBytes = Serializer::u32ToBytes(payloadSize);
  copy(payloadSizeInBytes.begin(), payloadSizeInBytes.end(),
       back_inserter(bytes));

  return bytes;
}

ApplicationMessage::ApplicationMessage() {}

ApplicationMessage::Header ApplicationMessage::header() const {
  return m_header;
}

void ApplicationMessage::reserve(uint32_t size) {
  m_header.payloadSize = size;
}

const char * ApplicationMessage::payload() { return m_payload; }

size_t ApplicationMessage::size() const {
  return sizeof(Header) + m_header.payloadSize;
}

vector<uint8_t> ApplicationMessage::serialize() const
{
    vector<uint8_t> bytes;
    bytes.reserve(size());

    const auto headerBytes = m_header.toBytes();

    copy(headerBytes.begin(),headerBytes.end(),back_inserter(bytes));
    copy(m_payload,m_payload + m_header.payloadSize,back_inserter(bytes));

    return bytes;
}

unique_ptr<ApplicationMessage> create(std::vector<uint8_t> && byteSequence)
{
  const auto messageType = static_cast<ApplicationMessage::Types>(byteSequence[0]);

  switch (messageType) {
    case ApplicationMessage::Types::FaceDetectionRequest:
      return make_unique<FaceDetectionRequestMessage>(move(byteSequence));
      break;
    case ApplicationMessage::Types::FaceDetectionResponse:
      return make_unique<FaceDetectionResponseMessage>(move(byteSequence));
      break;
  }
}