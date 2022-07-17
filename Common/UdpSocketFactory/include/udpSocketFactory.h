#pragma once

#include <boost/asio.hpp>

class UdpSocketFactory {
public:
  using Udp = boost::asio::ip::udp;
  using UdpSocket = Udp::socket;

  UdpSocketFactory(boost::asio::io_context &ioContext);

  UdpSocket createAndOpenSocket(Udp ipProtocol);
  UdpSocket createOpenAndBindSocket(Udp::endpoint endpoint);

protected:
  void openSocket(UdpSocket &socket, Udp ipProtocol);
  void bindSocket(UdpSocket &socket, Udp::endpoint endpoint);

private:
  boost::asio::io_context &m_ioContext;
};