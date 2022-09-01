#include "messageReceiverInterface.h"

#include <memory>
#include <stdexcept>

using std::invalid_argument;
using std::logic_error;
using std::make_unique;
using std::move;
using std::thread;

void MessageReceiverInterface::start() {
  m_isRunning = true;
  m_receiveMessageThread = make_unique<thread>([this]() {
    while (m_isRunning) {
      auto applicationMessage = receiveMessage();

      if (applicationMessage.has_value()) {
        try{
          processMessage(move(applicationMessage.value()));
        }
        catch(const logic_error & e)
        {
          m_isRunning = false;
        }
      }
    }
  });
}

void MessageReceiverInterface::stop() {
  if (m_receiveMessageThread) {
    m_isRunning = false;
    if (m_receiveMessageThread->joinable()) {
      m_receiveMessageThread->join();
    }
  }
}

bool MessageReceiverInterface::isRunning()
{
  return m_isRunning;
}

void MessageReceiverInterface::setReceiveMessageCallback(
    ReceiveMessageCallbackT callback) {

  if (!callback) {
    throw invalid_argument("Input callback is null");
  }

  m_receiveMessageCallback = callback;
}

void MessageReceiverInterface::processMessage(ApplicationMessage &&message) {
  if (!m_receiveMessageCallback) {
    throw logic_error(
        "Could not process message, message receive callback is null");
  }

  m_receiveMessageCallback(move(message));
}