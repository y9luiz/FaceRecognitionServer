#include "udpClient.h"

using std::string;
using std::vector;

UdpClient::UdpClient(const string &url, uint16_t port)
    : m_socket(m_ioContext),
      m_remoteEndpoint(boost::asio::ip::address::from_string(url), port) {
  m_socket.open(m_remoteEndpoint.protocol());
}

void UdpClient::sendMessage(vector<uint8_t> &&message) {
  m_socket.send_to(boost::asio::buffer(move(message)), m_remoteEndpoint);
}

vector<uint8_t> UdpClient::receiveMessage() {
  vector<uint8_t> message(20);

  auto size =
      m_socket.receive_from(boost::asio::buffer(message), m_remoteEndpoint);

  message.resize(size);

  return message;
}
