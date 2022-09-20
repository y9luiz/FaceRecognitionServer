#pragma once

#include "applicationMessages.h"
#include "messageSenderInterface.h"
#include "udpSocket.h"

#include <memory>
#include <queue>
#include <thread>
#include <utility>

class UdpMessageSender : public MessageSenderInterface {
public:
  struct UdpDestination : public MessageSenderInterface::Destination {
    Endpoint endpoint;
  };
  UdpMessageSender();
  UdpMessageSender(UdpMessageSender &) = delete;
  virtual ~UdpMessageSender();

  void sendMessage(ApplicationMessage &&applicationMessage,
                   const Endpoint &destination);

  std::shared_ptr<UdpSocket> socket() { return m_socket; }

private:
  void sendMessage(ApplicationMessage &&applicationMessage,
                   const Destination &destination) override;
  void sendMessageWorker();

  std::shared_ptr<UdpSocket> m_socket;
  std::atomic<bool> m_isRunning;
  std::mutex m_messageQueueMutex;
  std::queue<std::pair<ApplicationMessage, Endpoint>> m_messageQueue;
  std::unique_ptr<std::thread> m_sendMessageThread;
};