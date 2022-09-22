#include "parallelMessageReceiver.h"
#include "messageReceiverInterface.h"
#include <applicationMessages.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>

using namespace std::chrono_literals;

using std::cout;
using std::invalid_argument;
using std::logic_error;
using std::make_unique;
using std::move;
using std::optional;
using std::pair;
using std::thread;
using std::unique_ptr;
using std::vector;

namespace this_thread = std::this_thread;

ParallelMessageReceiver::ParallelMessageReceiver(
    unique_ptr<MessageReceiverInterface> messageReceiver)
    : m_messageReceiver(move(messageReceiver)) {
  if (!m_messageReceiver) {
    throw invalid_argument(
        "Could not create ParallelMessageReceiver, message receiver is null!");
  }
}

ParallelMessageReceiver::~ParallelMessageReceiver() {
  cout << "[INFO::ParallelMessageReceiver] stopping\n";
  stop();
}

void ParallelMessageReceiver::start() {
  m_isRunning = true;
  m_receiveMessageThread = make_unique<thread>([this]() {
    while (m_isRunning) {
      auto applicationMessageAndOrigin = receiveMessage();

      if (applicationMessageAndOrigin.has_value()) {
        try {
          cout << "[INFO::ParallelMessageReceiver] Processing "
                  "message\n";
          auto [applicationMessage, origin] =
              applicationMessageAndOrigin.value();
          processMessage(move(applicationMessage), origin);
        } catch (const logic_error &e) {
          cout << "[ERROR::ParallelMessageReceiver] " << e.what() << "\n";
          m_isRunning = false;
        }
      }
    }
  });
}

void ParallelMessageReceiver::stop() {
  if (m_receiveMessageThread) {
    m_isRunning = false;
    m_receiveMessageThread->detach();
    m_receiveMessageThread.reset();
  }
}

bool ParallelMessageReceiver::isRunning() { return m_isRunning; }

void ParallelMessageReceiver::setReceiveMessageCallback(
    ReceiveMessageCallbackT callback) {

  if (!callback) {
    throw invalid_argument("Input callback is null");
  }

  m_receiveMessageCallback = callback;
}

void ParallelMessageReceiver::processMessage(ApplicationMessage &&message,
                                             const vector<uint8_t> &origin) {
  if (!m_receiveMessageCallback) {
    throw logic_error(
        "Could not process message, message receive callback is null");
  }
  m_receiveMessageCallback(move(message), origin);
}

optional<pair<ApplicationMessage, MessageReceiverInterface::Origin>>
ParallelMessageReceiver::receiveMessage() {
  return m_messageReceiver->receiveMessage();
}