#pragma once

#include <functional>
#include <stdint.h>
#include <unordered_map>
#include <vector>
#include <memory>

class ApplicationMessage;
class Endpoint;

class MessageHandler {
public:
  using MessageHandlerCallbackMap = std::unordered_map<
      uint8_t,
      std::function<void(std::unique_ptr<ApplicationMessage>, const Endpoint &endpoint)>>;

  MessageHandler() = default;
  ~MessageHandler() = default;

  void registerCallback(
      uint8_t code,
      std::function<void(std::unique_ptr<ApplicationMessage>, const Endpoint &)> callback);

  void processMessage(std::unique_ptr<ApplicationMessage>, const Endpoint &endpoint);

private:
  void invokeCallback(uint8_t code, std::unique_ptr<ApplicationMessage> message,
                      const Endpoint &endpoint);

  MessageHandlerCallbackMap m_messageHandlerCallbackMap;
};