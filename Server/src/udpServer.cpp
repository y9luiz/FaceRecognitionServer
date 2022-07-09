#include "udpServer.h"
#include "socketManager.h"
#include <iostream>
#include <memory>

using std::exception;
using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;
using std::vector;

using namespace boost::asio::ip;

UdpServer::UdpServer(const string &url, uint16_t port)
    : m_url(url), m_port(port) {
  m_socketManager = make_unique<SocketManager>(
      m_ioContext, udp::endpoint(make_address(url), port), true);
}
UdpServer::~UdpServer() {}

void UdpServer::start() {
  unique_ptr<udp::endpoint> remoteEndpoint;
  unique_ptr<vector<uint8_t>> buffer;
  while (true) {
    remoteEndpoint = make_unique<udp::endpoint>();
    buffer = make_unique<vector<uint8_t>>(16);
    m_socketManager->socket().receive_from(boost::asio::buffer(*buffer),
                                           *remoteEndpoint);

    if (m_messageHandler) {
      m_clientThreads.emplace_back(
          [this](unique_ptr<vector<uint8_t>> buffer,
                 unique_ptr<udp::endpoint> remoteEndpoint) {
            try {
              m_messageHandler->processMessage(move(*buffer));
              m_socketManager->socket().send_to(
                  boost::asio::buffer("Mensagem processada\n"),
                  *remoteEndpoint);
            } catch (exception &e) {
              const string msg = e.what();
              m_socketManager->socket().send_to(boost::asio::buffer(msg),
                                                *remoteEndpoint);
            }
          },
          move(buffer), move(remoteEndpoint));
    }
  }
}

void UdpServer::stop() {}
