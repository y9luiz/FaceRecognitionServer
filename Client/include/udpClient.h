#pragma once

#include <udpSocketFactory.h>

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <vector>

class UdpClient {
public:
  UdpClient(const std::string &url, uint16_t port);

  void sendMessage(std::vector<uint8_t> &&message);
  std::vector<uint8_t> receiveMessage();

private:
  std::unique_ptr<boost::asio::ip::udp::socket> m_socket;
  boost::asio::io_context m_ioContext;
  boost::asio::ip::udp::endpoint m_remoteEndpoint;
};