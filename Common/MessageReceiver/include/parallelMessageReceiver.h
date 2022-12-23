#pragma once

#include "messageReceiverInterface.h"

#include <atomic>
#include <condition_variable>
#include <thread>

class ParallelMessageReceiver : public MessageReceiverInterface {
public:
  using ReceiveMessageCallbackT =
      std::function<void(std::unique_ptr<ApplicationMessage>, const Origin &)>;

  void setReceiveMessageCallback(ReceiveMessageCallbackT callback);

  virtual ~ParallelMessageReceiver();

  void start();

  void stop();

  bool isRunning();

protected:
  ParallelMessageReceiver(
      std::unique_ptr<MessageReceiverInterface> messageReceiver);

  std::optional<std::pair<std::unique_ptr<ApplicationMessage>, Origin>>
  receiveMessage() override;

  void processMessage(std::unique_ptr<ApplicationMessage> message, const Origin &origin);

  ReceiveMessageCallbackT m_receiveMessageCallback;

  std::unique_ptr<std::thread> m_receiveMessageThread;
  std::atomic<bool> m_isRunning;

  std::unique_ptr<MessageReceiverInterface> m_messageReceiver;
};
