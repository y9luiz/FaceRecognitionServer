#include "messageReceiverInterface.h"

#include <stdexcept>

using std::logic_error;
using std::move;

void MessageReceiverInterface::start() {
  m_isRunning = true;
  while (m_isRunning) {
    auto applicationMessage = receiveMessage();
    processMessage(move(applicationMessage));
  }
}

void MessageReceiverInterface::setReceiveMessageCallback(
    ReceiveMessageCallbackT callback) {
  m_receiveMessageCallback = callback;
}

void MessageReceiverInterface::processMessage(ApplicationMessage &&message) {
  if (!m_receiveMessageCallback) {
    throw logic_error(
        "Could not process message, message receive callback is null");
  }

  m_receiveMessageCallback(move(message));
}