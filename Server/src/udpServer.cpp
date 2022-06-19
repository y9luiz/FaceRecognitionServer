#include "udpServer.h"
#include <iostream>
#include <memory>

using std::exception;
using std::string;
using std::unique_ptr;
using std::vector;

using namespace boost::asio::ip;

UdpServer::UdpServer(const string &url, uint16_t port)
    : m_url(url), m_port(port), m_socket(m_ioContext) {}

UdpServer::~UdpServer() {}

void UdpServer::start() {
  udp::endpoint localEndpoint(make_address(m_url), m_port);

  m_socket.open(localEndpoint.protocol());
  m_socket.bind(localEndpoint);

  std::unique_ptr<udp::endpoint> remoteEndpoint;
  std::unique_ptr<vector<uint8_t>> buffer;
  while (true) {
    remoteEndpoint = std::make_unique<udp::endpoint>();
    buffer = std::make_unique<vector<uint8_t>>(16);
    m_socket.receive_from(boost::asio::buffer(*buffer), *remoteEndpoint);

    if (m_messageHandler) {
      m_clientThreads.emplace_back(
          [this](unique_ptr<vector<uint8_t>> buffer,
                 unique_ptr<udp::endpoint> remoteEndpoint) {
            try {
              m_messageHandler->processMessage(std::move(*buffer));
              m_socket.send_to(boost::asio::buffer("Mensagem processada\n"),
                               *remoteEndpoint);
            } catch (exception &e) {
              const string msg = e.what();
              m_socket.send_to(boost::asio::buffer(msg), *remoteEndpoint);
            }
          },
          std::move(buffer), std::move(remoteEndpoint));
    }
  }
}

void UdpServer::stop() {}
