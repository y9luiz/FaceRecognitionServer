#pragma once

#include "applicationMessages.h"
#include "iMessageReceiver.h"
#include "messageHandler.h"

#include <boost/asio.hpp>
#include <memory>
#include <queue>
#include <vector>

class UdpServer {
public:
  UdpServer(const std::string &ip, uint16_t port);
  ~UdpServer();

  void start();
  void stop();

  void setMessageHandler(std::unique_ptr<MessageHandler> messageHandler) {
    m_messageHandler = std::move(messageHandler);
  }

protected:
  void initializeMessageReceiver();
  void registerMessageReceiverCallback();

  void handleMessage(ApplicationMessage &&message);

  std::string m_ip;
  uint16_t m_port;

  std::unique_ptr<IMessageReceiver> m_messageReceiver;
  std::unique_ptr<MessageHandler> m_messageHandler;

  boost::asio::io_context m_ioContext;
};