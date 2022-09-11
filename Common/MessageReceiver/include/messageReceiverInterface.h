#pragma once

#include <applicationMessages.h>

#include <atomic>
#include <functional>
#include <thread>
#include <optional>

class MessageReceiverInterface {
public:
  using ReceiveMessageCallbackT = std::function<void(ApplicationMessage &&)>;

  void setReceiveMessageCallback(ReceiveMessageCallbackT callback);

  virtual ~MessageReceiverInterface() = default;

  virtual void start();

  virtual void stop();

  bool isRunning();

protected:

  virtual std::optional<ApplicationMessage> receiveMessage() = 0;

  void processMessage(ApplicationMessage &&message);

  ReceiveMessageCallbackT m_receiveMessageCallback;

  std::unique_ptr<std::thread> m_receiveMessageThread;
  std::atomic<bool> m_isRunning;
};