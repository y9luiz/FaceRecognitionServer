#include "udpServer.h"
#include "boost/asio/ip/address.hpp"
#include "boost/asio/ip/udp.hpp"
#include "udpSocketFactory.h"

#include <memory>

using std::exception;
using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;
using std::vector;

using namespace boost::asio::ip;

UdpServer::UdpServer(const string &ip, uint16_t port) : m_ip(ip), m_port(port) {
  UdpSocketFactory socketFactory(m_ioContext);
  m_socket = socketFactory.createOpenAndBindSocket(
      udp::endpoint(address::from_string(ip), port));
}
UdpServer::~UdpServer() {}

void UdpServer::start() {
  unique_ptr<udp::endpoint> remoteEndpoint;
  unique_ptr<vector<uint8_t>> buffer;
  while (true) {
    remoteEndpoint = make_unique<udp::endpoint>();
    buffer = make_unique<vector<uint8_t>>(16);
    m_socket->receive_from(boost::asio::buffer(*buffer), *remoteEndpoint);

    if (m_messageHandler) {
      m_clientThreads.emplace_back(
          [this](unique_ptr<vector<uint8_t>> buffer,
                 unique_ptr<udp::endpoint> remoteEndpoint) {
            try {
              m_messageHandler->processMessage(move(*buffer));
              m_socket->send_to(boost::asio::buffer("Mensagem processada\n"),
                                *remoteEndpoint);
            } catch (exception &e) {
              const string msg = e.what();
              m_socket->send_to(boost::asio::buffer(msg), *remoteEndpoint);
            }
          },
          move(buffer), move(remoteEndpoint));
    }
  }
}

void UdpServer::stop() {}
