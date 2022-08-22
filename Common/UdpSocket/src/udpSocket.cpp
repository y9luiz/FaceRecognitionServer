#include <stdexcept>
#include <udpSocket.h>

using boost::asio::ip::address;
using BoostEndpoint = boost::asio::ip::udp::endpoint;
using std::invalid_argument;
using std::make_unique;
using BoostUdp = boost::asio::ip::udp;
using std::logic_error;
using std::move;
using std::vector;

BoostUdp IpProtocolVersionToBoost(IpProtocolVersion ipProtocolVersion) {

  switch (ipProtocolVersion) {
  case IpProtocolVersion::V4:
    return BoostUdp::v4();
  case IpProtocolVersion::V6:
    return BoostUdp::v6();
  };

  throw invalid_argument(
      "Ip Protocol isn't is convertable to boost protocol types");
}

UdpSocket::UdpSocket(IpProtocolVersion ipProtocolVersion)
    : m_ipProtocolVersion(ipProtocolVersion) {
  m_socket = make_unique<BoostUdp::socket>(m_ioContext);
}

void UdpSocket::bind(const Endpoint &endpoint) {

  if (!m_socket->is_open()) {
    open();
  }

  m_socket->bind(
      BoostEndpoint(address::from_string(endpoint.address), endpoint.port));
}

void UdpSocket::open() {
  m_socket->open(IpProtocolVersionToBoost(m_ipProtocolVersion));
}

size_t UdpSocket::receiveFrom(vector<uint8_t> &buffer,
                              Endpoint &remoteEndpoint) {
  BoostUdp::endpoint boostRemoteEndpoint;

  const auto amountOfBytesReceived =
      m_socket->receive_from(boost::asio::buffer(buffer), boostRemoteEndpoint);

  remoteEndpoint = {boostRemoteEndpoint.address().to_string(),
                    boostRemoteEndpoint.port()};

  return amountOfBytesReceived;
}

size_t UdpSocket::sendTo(const std::vector<uint8_t> &buffer,
                         const Endpoint &endpoint) {
  BoostUdp::endpoint remoteEndpoint(address::from_string(endpoint.address),
                                    endpoint.port);

  return m_socket->send_to(boost::asio::buffer(buffer), remoteEndpoint);
}