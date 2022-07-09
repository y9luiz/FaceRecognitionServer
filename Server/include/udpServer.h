#pragma once
#include "messageHandler.h"
#include "socketManager.h"

#include <boost/asio.hpp>
#include <queue>
#include <vector>

class UdpServer {
public:
  UdpServer(const std::string &url, uint16_t port);
  ~UdpServer();

  void start();
  void stop();

  void setMessageHandler(std::unique_ptr<MessageHandler> messageHandler) {
    m_messageHandler = std::move(messageHandler);
  }

protected:
  std::string m_url;
  uint16_t m_port;

  std::unique_ptr<MessageHandler> m_messageHandler;

  std::unique_ptr<SocketManager> m_socketManager;

  boost::asio::io_context m_ioContext;
  std::vector<std::thread> m_clientThreads;
};