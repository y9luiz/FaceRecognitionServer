#include "mockMessageReceiver.h"
#include "messageReceiverInterface.h"

#include <stdexcept>

using std::logic_error;
using std::optional;
using std::pair;

namespace {
MockMessageReceiver *g_mock = nullptr;
void assertMockExists() {
  if (!g_mock) {
    throw logic_error("MockMessageReceiver is null!");
  }
}

void assertMockNotExists() {
  if (g_mock) {
    throw logic_error("MockMessageReceiver should be null!");
  }
}
} // namespace

MockMessageReceiver::MockMessageReceiver() {
  assertMockNotExists();
  g_mock = this;
}

MockMessageReceiver::~MockMessageReceiver() {
  assertMockExists();
  g_mock = nullptr;
}

optional<pair<ApplicationMessage, MessageReceiverInterface::Origin>>
MessageReceiverInterface::receiveMessage() {
  assertMockExists();
  return g_mock->receiveMessage();
}
