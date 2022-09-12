#include "applicationMessages.h"
#include <serializer.h>

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

ApplicationMessage::Header::Header(const vector<uint8_t> &bytes) {
  const bool isValidMessage = bytes.size() >= sizeof(m_header);
  if (!isValidMessage) {
    throw invalid_argument(
        "Message size doesn't match with the expected ApplicationMessage size");
  }

  auto it = bytes.begin();
  code = *(it++);
  payloadSize = Serializer::u32FromBytes({it,bytes.end()});
}

vector<uint8_t> ApplicationMessage::Header::convertToBytes() const {
  vector<uint8_t> bytes(sizeof(Header));

  bytes[0] = code;
  auto payloadSizeInBytes = Serializer::u32ToBytes(payloadSize);
  copy(payloadSizeInBytes.begin(),payloadSizeInBytes.end(),bytes.begin() + 1);

  return bytes;
}

ApplicationMessage::ApplicationMessage(Header header, vector<uint8_t> &&payload)
    : m_header(header), m_payload(move(payload)) {}

ApplicationMessage::ApplicationMessage(vector<uint8_t> &&message)
    : m_header(message) {
  move(message.begin() + sizeof(Header), message.end(),
       back_inserter(m_payload));
}

ApplicationMessage::Header ApplicationMessage::header() const {
  return m_header;
}

vector<uint8_t> &ApplicationMessage::payload() { return m_payload; }

vector<uint8_t> ApplicationMessage::convertToBytes() const {
  vector<uint8_t> bytes = m_header.convertToBytes();
  copy(m_payload.begin(), m_payload.end(), back_inserter(bytes));

  return bytes;
}

size_t ApplicationMessage::size() const {
  return sizeof(Header) + m_payload.size();
}