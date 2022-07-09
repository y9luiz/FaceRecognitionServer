#include "udpClient.h"
#include "socketManager.h"

using std::make_unique;
using std::string;
using std::vector;

UdpClient::UdpClient(const string &url, uint16_t port)
    : m_remoteEndpoint(boost::asio::ip::address::from_string(url), port) {
  m_socketManager =
      make_unique<SocketManager>(m_ioContext, m_remoteEndpoint, false);
}

void UdpClient::sendMessage(vector<uint8_t> &&message) {
  m_socketManager->socket().send_to(boost::asio::buffer(move(message)),
                                    m_remoteEndpoint);
}

vector<uint8_t> UdpClient::receiveMessage() {
  vector<uint8_t> message(20);

  auto size = m_socketManager->socket().receive_from(
      boost::asio::buffer(message), m_remoteEndpoint);

  message.resize(size);

  return message;
}
