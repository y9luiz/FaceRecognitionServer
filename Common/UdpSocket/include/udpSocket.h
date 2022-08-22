#pragma once

#include <boost/asio.hpp>
#include <memory>

struct Endpoint {
  std::string address;
  uint16_t port;
};

enum class IpProtocolVersion : uint8_t { V4, V6 };

class UdpSocket {
public:
  UdpSocket(const UdpSocket &udpSocket) = delete;

  UdpSocket(UdpSocket &&udpSocket) = delete;

  UdpSocket(IpProtocolVersion ipProtocolVersion = IpProtocolVersion::V4);

  void bind(const Endpoint &endpoint);
  void open();

  std::size_t receiveFrom(std::vector<uint8_t> &buffer,
                          Endpoint &remoteEndpoint);

  std::size_t sendTo(const std::vector<uint8_t> &buffer,
                     const Endpoint &endpoint);

private:
  IpProtocolVersion m_ipProtocolVersion;
  std::unique_ptr<boost::asio::ip::udp::socket> m_socket;
  boost::asio::ip::udp::endpoint m_endpoint;

  boost::asio::io_context m_ioContext;
};