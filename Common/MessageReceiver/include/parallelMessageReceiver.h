#pragma once

#include "messageReceiverInterface.h"

#include <atomic>
#include <thread>

class ParallelMessageReceiver : public MessageReceiverInterface {
public:
  using ReceiveMessageCallbackT =
      std::function<void(ApplicationMessage &&, const Origin &)>;

  ParallelMessageReceiver(
      std::unique_ptr<MessageReceiverInterface> messageReceiver);

  void setReceiveMessageCallback(ReceiveMessageCallbackT callback);

  virtual ~ParallelMessageReceiver();

  void start();

  void stop();

  bool isRunning();

protected:
  std::optional<std::pair<ApplicationMessage, Origin>>
  receiveMessage() override;

  void processMessage(ApplicationMessage &&message, const Origin &origin);

  ReceiveMessageCallbackT m_receiveMessageCallback;

  std::unique_ptr<std::thread> m_receiveMessageThread;
  std::atomic<bool> m_isRunning;

  std::unique_ptr<MessageReceiverInterface> m_messageReceiver;
};