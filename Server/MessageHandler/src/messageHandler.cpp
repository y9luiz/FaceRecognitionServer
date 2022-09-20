#include "messageHandler.h"
#include <applicationMessages.h>

#include <stdexcept>

using std::function;
using std::invalid_argument;
using std::move;

void MessageHandler::registerCallback(
    uint8_t code,
    function<void(ApplicationMessage &&, const Endpoint &)> callback) {
  if (!callback) {
    throw invalid_argument("Could not register null callback");
  }
  m_messageHandlerCallbackMap[code] = callback;
}

void MessageHandler::processMessage(ApplicationMessage &&message,
                                    const Endpoint &endpoint) {
  auto code = message.header().code;
  invokeCallback(code, move(message), endpoint);
}

void MessageHandler::invokeCallback(uint8_t code, ApplicationMessage &&message,
                                    const Endpoint &endpoint) {
  const auto &it = m_messageHandlerCallbackMap.find(code);

  const bool haveRegisteredCallback = it != m_messageHandlerCallbackMap.end();

  if (!haveRegisteredCallback) {
    throw invalid_argument("Callback not registered");
  }

  it->second(move(message), endpoint);
}