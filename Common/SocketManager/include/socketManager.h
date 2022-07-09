#pragma once

#include <boost/asio.hpp>

class SocketManager {
public:
  SocketManager(boost::asio::io_context &ioContext,
                boost::asio::ip::udp::endpoint endpoint, bool isServer);

  boost::asio::ip::udp::socket &socket();

protected:
  void openSocket();
  void bindSocket();

private:
  bool m_isServer;

  boost::asio::io_context &m_ioContext;
  boost::asio::ip::udp::socket m_socket;
  boost::asio::ip::udp::endpoint m_endpoint;
};