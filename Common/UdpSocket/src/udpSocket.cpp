#include <udpSocket.h>

#include <future>
#include <stdexcept>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

using boost::asio::ip::address;
using BoostEndpoint = boost::asio::ip::udp::endpoint;
using std::invalid_argument;
using std::make_unique;
using BoostUdp = boost::asio::ip::udp;
using std::logic_error;
using std::vector;

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
  boost::system::error_code errorCode;
  boost::asio::ip::make_address(endpoint.address, errorCode);

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

void assertSocketIsBinded(const UdpSocket &udpSocket) {
  if (!udpSocket.isBindMode()) {
    throw logic_error("UdpSocket isn't in bind mode");
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
                              Endpoint &remoteEndpoint) const {
  assertSocketIsOpen(*this);

  BoostUdp::endpoint boostRemoteEndpoint;

  auto futureAmountOfBytes = m_socket->async_receive_from(
      boost::asio::buffer(buffer), boostRemoteEndpoint,
      boost::asio::use_future);

  size_t numberOfReceivedBytes;

  std::future_status status =
      futureAmountOfBytes.wait_for(m_receiveFromTimeoutMs);

  if (status == std::future_status::ready) {
    remoteEndpoint.address = boostRemoteEndpoint.address().to_string();
    remoteEndpoint.port = boostRemoteEndpoint.port();
    numberOfReceivedBytes = futureAmountOfBytes.get();
    return numberOfReceivedBytes;
  }

  m_socket->cancel();

  return 0;
}

size_t UdpSocket::sendTo(const std::vector<uint8_t> &buffer,
                         const Endpoint &endpoint) const {

  assertValidEndpoint(endpoint);
  assertSocketIsOpen(*this);

  BoostUdp::endpoint remoteEndpoint(address::from_string(endpoint.address),
                                    endpoint.port);

  return m_socket->send_to(boost::asio::buffer(buffer), remoteEndpoint);
}