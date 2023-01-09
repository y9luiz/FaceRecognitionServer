#pragma once

#include <functional>
#include <unordered_map>
#include <memory>

class ApplicationMessage;
class Endpoint;

class MessageHandler {
public:
  using CallbackT = std::function<void(std::unique_ptr<ApplicationMessage>, const Endpoint &endpoint)>;

  using CallbackMap = std::unordered_map<
      uint8_t,CallbackT>;

  MessageHandler() = default;
  ~MessageHandler() = default;

  void registerCallback(
      uint8_t code,
      std::function<void(std::unique_ptr<ApplicationMessage>, const Endpoint &)> callback);

  void processMessage(std::unique_ptr<ApplicationMessage>, const Endpoint &endpoint);

private:
  void invokeCallback(uint8_t code, std::unique_ptr<ApplicationMessage> message,
                      const Endpoint &endpoint);

  CallbackMap m_callbacks;
};