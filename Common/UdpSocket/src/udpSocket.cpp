#include "udpSocket.h"

#include <memory>
#include <stdexcept>

using boost::asio::ip::address;
using boost::asio::ip::make_address;
using boost::system::error_code;
using std::invalid_argument;
using std::logic_error;
using std::make_unique;
using std::vector;

using BoostUdp = boost::asio::ip::udp;
using BoostEndpoint = boost::asio::ip::udp::endpoint;

namespace {

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

void assertValidEndpoint(const Endpoint &endpoint) {
  error_code errorCode;
  make_address(endpoint.address, errorCode);

  if (errorCode) {
    throw invalid_argument("Invalid IP Address");
  }
  if (endpoint.port == 0) {
    throw invalid_argument("Port number could not be zero");
  }
}

void assertSocketIsOpen(const UdpSocket &udpSocket) {
  if (!udpSocket.isOpen()) {
    throw logic_error("UdpSocket is closed");
  }
}
} // namespace

UdpSocket::UdpSocket(IpProtocolVersion ipProtocolVersion)
    : m_ipProtocolVersion(ipProtocolVersion), m_isBindMode{false} {
  m_socket = make_unique<BoostUdp::socket>(m_ioContext);
}

bool UdpSocket::isOpen() const { return m_socket->is_open(); }

bool UdpSocket::isBindMode() const { return m_isBindMode; }

void UdpSocket::bind(const Endpoint &endpoint) {

  assertValidEndpoint(endpoint);

  if (!m_socket->is_open()) {
    open();
  }

  m_socket->bind(
      BoostEndpoint(address::from_string(endpoint.address), endpoint.port));

  m_isBindMode = true;
}

void UdpSocket::open() {
  m_socket->open(IpProtocolVersionToBoost(m_ipProtocolVersion));
}

size_t UdpSocket::receiveFrom(vector<uint8_t> &buffer,
                              Endpoint &remoteEndpoint) {
  assertSocketIsOpen(*this);

  BoostEndpoint endpoint;
  auto numberOfBytesReceived =
      m_socket->receive_from(boost::asio::buffer(buffer), endpoint);
  if (numberOfBytesReceived) {
    remoteEndpoint.address = endpoint.address().to_string();
    remoteEndpoint.port = endpoint.port();
  }

  return numberOfBytesReceived;
}

size_t UdpSocket::receive(vector<uint8_t> &buffer) {
  assertSocketIsOpen(*this);

  return m_socket->receive(boost::asio::buffer(buffer));
}

size_t UdpSocket::sendTo(const std::vector<uint8_t> &buffer,
                         const Endpoint &endpoint) {

  assertValidEndpoint(endpoint);
  assertSocketIsOpen(*this);

  BoostUdp::endpoint destinationEndpoint(address::from_string(endpoint.address),
                                         endpoint.port);

  return m_socket->send_to(boost::asio::buffer(buffer), destinationEndpoint);
}