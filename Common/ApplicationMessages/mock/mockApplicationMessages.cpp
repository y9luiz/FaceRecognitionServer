#include "mockApplicationMessages.h"
#include "applicationMessages.h"

using std::move;
using std::vector;

namespace {
MockApplicationMessage *g_mock = nullptr;
};

MockApplicationMessage::MockApplicationMessage() { g_mock = this; }

MockApplicationMessage::~MockApplicationMessage() { g_mock = nullptr; }

ApplicationMessage::Header::Header(uint8_t code, uint16_t payloadSize)
    : m_code(code), m_payloadSize(payloadSize) {}

ApplicationMessage::Header::Header(const vector<uint8_t> &bytes) {}

ApplicationMessage::ApplicationMessage(uint8_t code, uint16_t payloadSize,
                                       vector<uint8_t> &&payload)
    : m_header(code, payloadSize) {
  g_mock->constructor(code, payloadSize, move(payload));
}

ApplicationMessage::ApplicationMessage(vector<uint8_t> &&message)
    : m_header(message) {
  g_mock->constructor(move(message));
}

uint8_t ApplicationMessage::code() { return g_mock->code(); }

uint16_t ApplicationMessage::payloadSize() { return g_mock->payloadSize(); }

std::vector<uint8_t> &ApplicationMessage::payload() {
  return g_mock->payload();
}

std::vector<uint8_t> ApplicationMessage::convertToBytes() {
  return g_mock->convertToBytes();
}