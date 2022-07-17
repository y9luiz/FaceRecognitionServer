#include "udpSocketFactory.h"

UdpSocketFactory::UdpSocketFactory(boost::asio::io_context &ioContext)
    : m_ioContext(ioContext) {}

UdpSocketFactory::UdpSocket
UdpSocketFactory::createAndOpenSocket(Udp ipProtocol) {
  UdpSocketFactory::UdpSocket socket(m_ioContext);
  openSocket(socket, ipProtocol);

  return socket;
}

UdpSocketFactory::UdpSocket
UdpSocketFactory::createOpenAndBindSocket(Udp::endpoint endpoint) {
  UdpSocketFactory::UdpSocket socket(m_ioContext);
  openSocket(socket, endpoint.protocol());
  bindSocket(socket, endpoint);

  return socket;
}

void UdpSocketFactory::openSocket(UdpSocketFactory::UdpSocket &socket,
                                  Udp ipProtocol) {
  socket.open(ipProtocol);
}

void UdpSocketFactory::bindSocket(UdpSocketFactory::UdpSocket &socket,
                                  Udp::endpoint endpoint) {
  socket.bind(endpoint);
}
