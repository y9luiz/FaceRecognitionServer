#include "udpSocketFactory.h"
#include <memory>

using std::make_unique;
using std::unique_ptr;

UdpSocketFactory::UdpSocketFactory(boost::asio::io_context &ioContext)
    : m_ioContext(ioContext) {}

unique_ptr<UdpSocketFactory::UdpSocket>
UdpSocketFactory::createAndOpenSocket(Udp ipProtocol) {
  unique_ptr<UdpSocketFactory::UdpSocket> socket =
      make_unique<UdpSocketFactory::UdpSocket>(m_ioContext);

  openSocket(*socket, ipProtocol);

  return socket;
}

unique_ptr<UdpSocketFactory::UdpSocket>
UdpSocketFactory::createOpenAndBindSocket(Udp::endpoint endpoint) {
  unique_ptr<UdpSocketFactory::UdpSocket> socket =
      make_unique<UdpSocketFactory::UdpSocket>(m_ioContext);

  openSocket(*socket, endpoint.protocol());
  bindSocket(*socket, endpoint);

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
