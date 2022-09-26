#include "mockApplicationMessages.h"
#include "applicationMessages.h"
#include <algorithm>

using namespace testing;

using std::copy;
using std::logic_error;
using std::move;
using std::size_t;
using std::vector;

namespace {
MockApplicationMessage *g_mock = nullptr;

const vector<uint8_t> DefaultPayload{'p', 'a', 'y', 'l', 'o', 'a', 'd'};
constexpr auto DefaultCode = 0u;
const auto DefaultHeader =
    ApplicationMessage::Header(DefaultCode, DefaultPayload.size());

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

vector<uint8_t> getDefaultBytes() {
  vector<uint8_t> bytes;
  bytes.reserve(DefaultPayload.size() + sizeof(uint32_t) + sizeof(DefaultCode));
  bytes.emplace_back(DefaultCode);

  uint32_t payloadSize = DefaultPayload.size();
  bytes.emplace_back(payloadSize);
  bytes.emplace_back(payloadSize >> 8);
  bytes.emplace_back(payloadSize >> 16);
  bytes.emplace_back(payloadSize >> 24);

  copy(DefaultPayload.begin(), DefaultPayload.end(), back_inserter(bytes));

  return bytes;
}
}; // namespace

MockApplicationMessage::MockApplicationMessage() {
  assertMockNotExists();

  ON_CALL(*this, payload).WillByDefault(ReturnRefOfCopy(DefaultPayload));
  ON_CALL(*this, header).WillByDefault(Return(DefaultHeader));
  ON_CALL(*this, size())
      .WillByDefault(Return(sizeof(DefaultCode) + DefaultPayload.size() +
                            sizeof(uint32_t)));
  ON_CALL(*this, convertToBytes()).WillByDefault(Return(getDefaultBytes()));

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

void ApplicationMessage::reserve(uint32_t size) {
  assertMockExists();
  return g_mock->reserve(size);
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