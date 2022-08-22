#include "udpClient.h"
#include "applicationMessages.h"

using std::make_unique;
using std::string;
using std::vector;

UdpClient::UdpClient(const string &address, uint16_t port)
    : m_endpoint{address, port} {
  m_socket = make_unique<UdpSocket>();
  m_socket->open();
}

void UdpClient::sendMessage(vector<uint8_t> &&message) {
  m_socket->sendTo(move(message), m_endpoint);
}

vector<uint8_t> UdpClient::receiveMessage() {
  vector<uint8_t> message;
  message.reserve(20);

  auto size = m_socket->receiveFrom(message, m_endpoint);

  return message;
}
