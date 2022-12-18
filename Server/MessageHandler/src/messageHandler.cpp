#include "messageHandler.h"

#include <applicationMessages.h>
#include <endpoint.h>

#include <functional>
#include <stdexcept>

using std::function;
using std::invalid_argument;
using std::move;

void MessageHandler::registerCallback(
    uint8_t code,
    function<void(std::unique_ptr<ApplicationMessage>, const Endpoint &)> callback) {
  if (!callback) {
    throw invalid_argument("Could not register null callback");
  }
  m_messageHandlerCallbackMap[code] = callback;
}

void MessageHandler::processMessage(std::unique_ptr<ApplicationMessage> message,
                                    const Endpoint &endpoint) {
  auto code = message->code();
  invokeCallback(static_cast<uint8_t>(code), move(message), endpoint);
}

void MessageHandler::invokeCallback(uint8_t code, std::unique_ptr<ApplicationMessage> message,
                                    const Endpoint &endpoint) {
  const auto &it = m_messageHandlerCallbackMap.find(code);

  const bool haveRegisteredCallback = it != m_messageHandlerCallbackMap.end();

  if (!haveRegisteredCallback) {
    throw invalid_argument("Callback not registered");
  }

  it->second(move(message), endpoint);
}