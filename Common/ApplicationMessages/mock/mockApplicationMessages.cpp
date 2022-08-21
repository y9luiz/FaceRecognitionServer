#include "mockApplicationMessages.h"

using std::logic_error;
using std::move;
using std::vector;

namespace {
MockApplicationMessage *g_mock = nullptr;

void assertMockExists() {
  if (!g_mock) {
    throw logic_error("MockApplicationMessage is null!");
  }
}

void assertMockNotExists() {
  if (!g_mock) {
    throw logic_error("MockApplicationMessage should be null!");
  }
}
}; // namespace

MockApplicationMessage::MockApplicationMessage() {
  assertMockNotExists();
  g_mock = this;
}

MockApplicationMessage::~MockApplicationMessage() {
  assertMockExists();
  g_mock = nullptr;
}

ApplicationMessage::Header::Header(uint8_t code, uint16_t payloadSize)
    : m_code(code), m_payloadSize(payloadSize) {}

ApplicationMessage::Header::Header(const vector<uint8_t> &bytes) {}

ApplicationMessage::ApplicationMessage(uint8_t code, uint16_t payloadSize,
                                       vector<uint8_t> &&payload)
    : m_header(code, payloadSize) {
  assertMockExists();
  g_mock->constructor(code, payloadSize, move(payload));
}

ApplicationMessage::ApplicationMessage(vector<uint8_t> &&message)
    : m_header(message) {
  assertMockExists();
  g_mock->constructor(move(message));
}

uint8_t ApplicationMessage::code() { return g_mock->code(); }

uint16_t ApplicationMessage::payloadSize() { return g_mock->payloadSize(); }

vector<uint8_t> &ApplicationMessage::payload() {
  assertMockExists();
  return g_mock->payload();
}

vector<uint8_t> ApplicationMessage::convertToBytes() {
  assertMockExists();
  return g_mock->convertToBytes();
}