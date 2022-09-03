#pragma once

#include <messageReceiverInterface.h>
#include <udpSocket.h>

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <vector>


class UdpClient {
public:
  UdpClient(const std::string &url, uint16_t port);
  virtual ~UdpClient();
  void sendMessage(std::vector<uint8_t> &&message);

private:
  std::unique_ptr<MessageReceiverInterface> m_messageReceiver;
  UdpSocket m_socket;
  Endpoint m_endpoint;
};