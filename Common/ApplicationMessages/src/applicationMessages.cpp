#include "applicationMessages.h"

#include <iostream>
#include <stdexcept>
#include <vector>

using std::invalid_argument;
using std::move;
using std::vector;

ApplicationMessage::ApplicationMessage(uint8_t code, uint16_t payloadSize,
                                       vector<uint8_t> &&payload)
    : m_code(code), m_payloadSize(payloadSize), m_payload(move(payload)) {}

ApplicationMessage::ApplicationMessage(vector<uint8_t> &&message) {

  const bool isValidMessage =
      message.size() == sizeof(m_code) + sizeof(m_payloadSize);
  if (!isValidMessage) {
    throw invalid_argument(
        "Message size doesn't match with the expected ApplicationMessage size");
  }

  m_code = message[0];
  message.erase(message.begin());

  m_payloadSize = (message[0] | message[1] << 8);

  message.erase(message.begin(), message.begin() + 2);

  m_payload = move(message);
}

uint8_t ApplicationMessage::code() { return m_code; }

uint16_t ApplicationMessage::payloadSize() { return m_payloadSize; }

std::vector<uint8_t> &ApplicationMessage::payload() { return m_payload; }
