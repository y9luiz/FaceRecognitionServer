#pragma once

#include <messageReceiverInterface.h>
#include <messageSenderInterface.h>
#include <udpSocket.h>

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <vector>

class UdpClient {
public:
  UdpClient(const std::string &url, uint16_t port);
  virtual ~UdpClient();
  void sendMessage(ApplicationMessage &&message);

private:
  std::unique_ptr<MessageReceiverInterface> m_udpMessageReceiver;
  std::unique_ptr<MessageSenderInterface> m_udpMessageSender;
  Endpoint m_destinationEndpoint;
};