#include "socketManager.h"

SocketManager::SocketManager(boost::asio::io_context &ioContext,
                             boost::asio::ip::udp::endpoint endpoint,
                             bool isServer)
    : m_ioContext(ioContext), m_socket(ioContext), m_endpoint(endpoint) {

  openSocket();

  if (isServer) {
    bindSocket();
  }
}

void SocketManager::openSocket() { m_socket.open(m_endpoint.protocol()); }

void SocketManager::bindSocket() { m_socket.bind(m_endpoint); }

boost::asio::ip::udp::socket &SocketManager::socket() { return m_socket; }
