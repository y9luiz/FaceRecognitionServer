#pragma once

#include "applicationMessages.h"
#include "messageHandler.h"
#include "parallelMessageReceiver.h"
#include "udpMessageSender.h"

#include <memory>

class UdpServer {
public:
  UdpServer(const std::string &ip, uint16_t port);
  virtual ~UdpServer();

  void stop();

  void setMessageHandler(std::unique_ptr<MessageHandler> messageHandler) {
    m_messageHandler = std::move(messageHandler);
  }

protected:
  void initializeMessageReceiver();
  void registerMessageReceiverCallback();

  void handleMessage(std::unique_ptr<ApplicationMessage> message, const Endpoint &endpoint);

  std::string m_ip;
  uint16_t m_port;

  std::unique_ptr<ParallelMessageReceiver> m_messageReceiver;
  std::unique_ptr<UdpMessageSender> m_messageSender;
  std::unique_ptr<MessageHandler> m_messageHandler;

  boost::asio::io_context m_ioContext;
};