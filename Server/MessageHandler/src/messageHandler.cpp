#include "messageHandler.h"
#include <stdexcept>

using std::function;
using std::invalid_argument;
using std::move;
using std::vector;

void MessageHandler::registerCallback(
    uint8_t code, function<void(vector<uint8_t> &&)> callback) {
  m_messageHandlerCallbackMap[code] = callback;
}

void MessageHandler::processMessage(vector<uint8_t> &&message) {
  if (message.empty()) {
    throw invalid_argument("Could not process empty message");
  }

  const auto code = message[0];
  message.erase(message.begin());

  invokeCallback(code, move(message));
}

void MessageHandler::invokeCallback(uint8_t code, vector<uint8_t> &&message) {
  const auto &it = m_messageHandlerCallbackMap.find(code);

  const bool haveRegisteredCallback = it != m_messageHandlerCallbackMap.end();

  if (!haveRegisteredCallback) {
    throw invalid_argument("Callback not registered");
  }

  it->second(move(message));
}