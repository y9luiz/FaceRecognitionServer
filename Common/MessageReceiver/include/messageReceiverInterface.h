#pragma once

#include <applicationMessages.h>

#include <atomic>
#include <functional>
#include <thread>

class MessageReceiverInterface {
public:
  using ReceiveMessageCallbackT = std::function<void(ApplicationMessage &&)>;

  void setReceiveMessageCallback(ReceiveMessageCallbackT callback);

  virtual ~MessageReceiverInterface() = default;

  void start();

protected:
  virtual ApplicationMessage receiveMessage() = 0;

  void processMessage(ApplicationMessage &&message);

  ReceiveMessageCallbackT m_receiveMessageCallback;

  std::thread m_receiveMessageThread;
  std::atomic<bool> m_isRunning;
};