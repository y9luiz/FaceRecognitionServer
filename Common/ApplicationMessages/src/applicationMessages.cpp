#include "applicationMessages.h"

#include <serializer.h>

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

using std::back_inserter;
using std::copy;
using std::invalid_argument;
using std::move;
using std::size_t;
using std::vector;

ApplicationMessage::Header::Header(uint8_t code, uint32_t payloadSize)
    : code(code), payloadSize(payloadSize) {}

ApplicationMessage::Header::Header(vector<uint8_t> &bytes) {
  const bool isValidMessage = bytes.size() >= sizeof(m_header);
  if (!isValidMessage) {
    throw invalid_argument(
        "Message size doesn't match with the expected ApplicationMessage size");
  }

  code = bytes[0];
  bytes.erase(bytes.begin());
  payloadSize = Serializer::u32FromBytes(bytes);
}

vector<uint8_t> ApplicationMessage::Header::toBytes() const {
  vector<uint8_t> bytes;
  bytes.reserve(sizeof(Header));

  bytes.emplace_back(code);

  auto payloadSizeInBytes = Serializer::u32ToBytes(payloadSize);
  copy(payloadSizeInBytes.begin(), payloadSizeInBytes.end(),
       back_inserter(bytes));

  return bytes;
}

ApplicationMessage::ApplicationMessage(uint8_t code, vector<uint8_t> &&payload)
    : m_header(code, payload.size()), m_payload(move(payload)) {}

ApplicationMessage::ApplicationMessage(vector<uint8_t> &&message)
    : m_header(message), m_payload(move(message)) {}

ApplicationMessage::Header ApplicationMessage::header() const {
  return m_header;
}

void ApplicationMessage::reserve(uint32_t size) {
  m_header.payloadSize = size;
  m_payload.reserve(size);
}

vector<uint8_t> &ApplicationMessage::payload() { return m_payload; }

vector<uint8_t> ApplicationMessage::convertToBytes() const {
  vector<uint8_t> bytes;
  bytes.reserve(size());
  auto headerBytes = m_header.toBytes();
  copy(headerBytes.begin(), headerBytes.end(), back_inserter(bytes));
  copy(m_payload.begin(), m_payload.end(), back_inserter(bytes));

  return bytes;
}

size_t ApplicationMessage::size() const {
  return sizeof(Header) + m_payload.size();
}
