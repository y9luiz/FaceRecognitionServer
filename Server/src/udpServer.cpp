#include "udpServer.h"
#include <iostream>

using std::string;
using std::vector;
using namespace boost::asio::ip;

UdpServer::UdpServer(const string &url, uint16_t port)
    : m_url(url), m_port(port), m_socket(m_ioContext) {}

UdpServer::~UdpServer() {}

void UdpServer::start() {
  udp::endpoint localEndpoint(make_address(m_url), m_port);

  m_socket.open(localEndpoint.protocol());
  m_socket.bind(localEndpoint);

  udp::endpoint remoteEndpoint;
  vector<uint8_t> buffer(10);
  while (true) {
    m_socket.receive_from(boost::asio::buffer(buffer), remoteEndpoint);

    if (m_processMessageCallback) {
      m_processMessageCallback(buffer);
    }
  }
}

void UdpServer::stop() {}

void UdpServer::setProcessMessageCallback(
    std::function<void(std::vector<uint8_t> &)> processMessageCallback) {
  m_processMessageCallback = processMessageCallback;
}