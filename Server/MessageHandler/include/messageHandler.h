#pragma once

#include <functional>
#include <stdint.h>
#include <unordered_map>
#include <vector>

class ApplicationMessage;
class Endpoint;

class MessageHandler {
public:
  using MessageHandlerCallbackMap = std::unordered_map<
      uint8_t,
      std::function<void(ApplicationMessage &&, const Endpoint &endpoint)>>;

  MessageHandler() = default;
  ~MessageHandler() = default;

  void registerCallback(
      uint8_t code,
      std::function<void(ApplicationMessage &&, const Endpoint &)> callback);
  void processMessage(ApplicationMessage &&message, const Endpoint &endpoint);

private:
  void invokeCallback(uint8_t code, ApplicationMessage &&message,
                      const Endpoint &endpoint);

  MessageHandlerCallbackMap m_messageHandlerCallbackMap;
};