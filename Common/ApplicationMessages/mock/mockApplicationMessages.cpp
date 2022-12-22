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
constexpr ApplicationMessage::Code DefaultCode = ApplicationMessage::Code::FaceDetectionRequest;

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

vector<uint8_t> serializeDefaultMessage() {
  vector<uint8_t> bytes;
  bytes.reserve(DefaultPayload.size() + sizeof(uint32_t) + sizeof(DefaultCode));
  bytes.emplace_back(static_cast<uint8_t>(DefaultCode));

  uint32_t payloadSize = DefaultPayload.size();
  bytes.emplace_back(payloadSize);
  bytes.emplace_back(payloadSize >> 8);
  bytes.emplace_back(payloadSize >> 16);
  bytes.emplace_back(payloadSize >> 24);

  copy(DefaultPayload.begin(), DefaultPayload.end(), back_inserter(bytes));

  return bytes;
}
}; // namespace

MockApplicationMessage::MockApplicationMessage() : ApplicationMessage(DefaultCode) {
  assertMockNotExists();

  ON_CALL(*this, code).WillByDefault(Return(DefaultCode));
  ON_CALL(*this, serialize()).WillByDefault(Return(serializeDefaultMessage()));

  g_mock = this;
}

MockApplicationMessage::~MockApplicationMessage() {
  assertMockExists();
  g_mock = nullptr;
}

vector<uint8_t> ApplicationMessage::serialize() const {
  assertMockExists();
  return g_mock->serialize();
}
