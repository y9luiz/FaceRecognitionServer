#include "applicationMessages.h"

#include <iostream>
#include <stdexcept>
#include <vector>

using std::back_inserter;
using std::copy;
using std::invalid_argument;
using std::move;
using std::vector;

ApplicationMessage::Header::Header(uint8_t code, uint16_t payloadSize)
    : m_code(code), m_payloadSize(payloadSize) {}

ApplicationMessage::Header::Header(const vector<uint8_t> &bytes) {
  const bool isValidMessage = bytes.size() >= sizeof(m_header);
  if (!isValidMessage) {
    throw invalid_argument(
        "Message size doesn't match with the expected ApplicationMessage size");
  }

  m_code = bytes[0];
  m_payloadSize = (bytes[1] | bytes[2] << 8);
}

std::vector<uint8_t> ApplicationMessage::Header::convertToBytes() {
  vector<uint8_t> bytes(sizeof(Header));

  bytes[0] = m_code;
  bytes[1] = m_payloadSize;
  bytes[2] = m_payloadSize >> 8;

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

uint8_t ApplicationMessage::code() { return m_header.m_code; }

uint16_t ApplicationMessage::payloadSize() { return m_header.m_payloadSize; }

vector<uint8_t> &ApplicationMessage::payload() { return m_payload; }

vector<uint8_t> ApplicationMessage::convertToBytes() {
  vector<uint8_t> bytes = m_header.convertToBytes();
  copy(m_payload.begin(), m_payload.end(), back_inserter(bytes));

  return bytes;
}