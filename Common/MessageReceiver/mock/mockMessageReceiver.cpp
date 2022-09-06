#include "mockMessageReceiver.h"

#include <optional>
#include <stdexcept>
#include <utility>

using std::logic_error;
using std::move;
using std::optional;

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

void MessageReceiverInterface::setReceiveMessageCallback(
    ReceiveMessageCallbackT callback) {
  assertMockExists();
  g_mock->setReceiveMessageCallback(callback);
}

void MessageReceiverInterface::start() {
  assertMockExists();
  g_mock->start();
}

void MessageReceiverInterface::stop() {
  assertMockExists();
  g_mock->stop();
}

bool MessageReceiverInterface::isRunning() {
  assertMockExists();
  return g_mock->isRunning();
}

optional<ApplicationMessage> MessageReceiverInterface::receiveMessage() {
  assertMockExists();
  return g_mock->receiveMessage();
}

void MessageReceiverInterface::processMessage(ApplicationMessage &&message) {
  assertMockExists();
  return g_mock->processMessage(move(message));
}