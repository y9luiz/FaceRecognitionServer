#include "mockApplicationMessages.h"
#include "applicationMessages.h"
#include <gmock/gmock-actions.h>
#include <gmock/gmock-spec-builders.h>

using namespace testing;

using std::logic_error;
using std::move;
using std::size_t;
using std::vector;

namespace {
MockApplicationMessage *g_mock = nullptr;

const vector<uint8_t> DefaultPayload = {'p', 'a', 'y', 'l', 'o', 'a', 'd'};
constexpr auto DefaultCode = 0u;

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

  ON_CALL(*this, payload).WillByDefault(ReturnRefOfCopy(DefaultPayload));
  ON_CALL(*this, header)
      .WillByDefault(Return(
          ApplicationMessage::Header(DefaultCode, DefaultPayload.size())));

  g_mock = this;
}

MockApplicationMessage::~MockApplicationMessage() {
  assertMockExists();
  g_mock = nullptr;
}

ApplicationMessage::Header::Header(uint8_t code, uint32_t payloadSize)
    : code(code), payloadSize(payloadSize) {}

ApplicationMessage::Header::Header(vector<uint8_t> &bytes) {}

vector<uint8_t> ApplicationMessage::Header::toBytes() const { return {}; }

ApplicationMessage::ApplicationMessage(uint8_t code, vector<uint8_t> &&payload)
    : m_header(code, payload.size()) {
  assertMockExists();
  g_mock->constructor(code, move(payload));
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