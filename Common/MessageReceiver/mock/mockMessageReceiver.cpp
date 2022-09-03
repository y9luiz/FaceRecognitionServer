#include "mockMessageReceiver.h"
#include "applicationMessages.h"
#include "messageReceiverInterface.h"

#include <optional>
#include <stdexcept>
#include <utility>

using std::move;
using std::optional;
using std::logic_error;

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

MockMessageReceiver::MockMessageReceiver() { g_mock = this; }

MockMessageReceiver::~MockMessageReceiver() { g_mock = nullptr; }

void MessageReceiverInterface::setReceiveMessageCallback(
    ReceiveMessageCallbackT callback) {
  g_mock->setReceiveMessageCallback(callback);
}

void MessageReceiverInterface::start() { g_mock->start(); }

void MessageReceiverInterface::stop() { g_mock->stop(); }

bool MessageReceiverInterface::isRunning() { return g_mock->isRunning(); }

optional<ApplicationMessage> MessageReceiverInterface::receiveMessage() {
  return g_mock->receiveMessage();
}

void MessageReceiverInterface::processMessage(ApplicationMessage &&message) {
  return g_mock->processMessage(move(message));
}