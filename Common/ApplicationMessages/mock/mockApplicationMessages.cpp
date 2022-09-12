#include "mockApplicationMessages.h"

using std::logic_error;
using std::move;
using std::size_t;
using std::vector;

namespace {
MockApplicationMessage *g_mock = nullptr;

void assertMockExists() {
  if (!g_mock) {
    throw logic_error("MockApplicationMessage is null!");
  }
}

void assertMockNotExists() {
  if (g_mock) {
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

ApplicationMessage::Header::Header(uint8_t code, uint32_t payloadSize)
    : code(code), payloadSize(payloadSize) {}

ApplicationMessage::Header::Header(const vector<uint8_t> &bytes) {}

vector<uint8_t> ApplicationMessage::Header::convertToBytes() const {
  return {};
}

ApplicationMessage::ApplicationMessage(ApplicationMessage::Header header,vector<uint8_t> &&payload)
    : m_header(header) {
  assertMockExists();
  g_mock->constructor(header, move(payload));
}

ApplicationMessage::ApplicationMessage(vector<uint8_t> &&message)
    : m_header(message) {
  assertMockExists();
  g_mock->constructor(move(message));
}

ApplicationMessage::Header ApplicationMessage::header() const {
  assertMockExists();
  return g_mock->header();
}

std::vector<uint8_t> &ApplicationMessage::payload() {
  assertMockExists();
  return g_mock->payload();
}

vector<uint8_t> ApplicationMessage::convertToBytes() const {
  assertMockExists();
  return g_mock->convertToBytes();
}

size_t ApplicationMessage::size() const {
  assertMockExists();
  return g_mock->size();
}