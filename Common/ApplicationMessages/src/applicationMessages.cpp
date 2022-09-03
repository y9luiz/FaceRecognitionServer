#include "applicationMessages.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using std::back_inserter;
using std::copy;
using std::invalid_argument;
using std::move;
using std::vector;
using std::remove;

ApplicationMessage::Header::Header(uint8_t code, uint16_t payloadSize)
    : code(code), payloadSize(payloadSize) {}

ApplicationMessage::Header::Header(const vector<uint8_t> &bytes) {
  const bool isValidMessage = bytes.size() >= sizeof(m_header);
  if (!isValidMessage) {
    throw invalid_argument(
        "Message size doesn't match with the expected ApplicationMessage size");
  }

  code = bytes[0];
  payloadSize = (bytes[1] | bytes[2] << 8);
}

vector<uint8_t> ApplicationMessage::Header::convertToBytes() const {
  vector<uint8_t> bytes(sizeof(Header));

  bytes[0] = code;
  bytes[1] = payloadSize;
  bytes[2] = payloadSize >> 8;

  return bytes;
}

ApplicationMessage::ApplicationMessage(uint8_t code, uint16_t payloadSize,
                                       vector<uint8_t> &&payload)
    : m_header(code, payloadSize), m_payload(move(payload)) {}

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

std::size_t ApplicationMessage::size() const
{
  return sizeof(Header) + m_payload.size();
}

bool ApplicationMessage::operator==(const ApplicationMessage & other) const
{
  return other.m_header.code == m_header.code
          && other.m_header.payloadSize == m_header.payloadSize
          && other.m_payload == m_payload;
}