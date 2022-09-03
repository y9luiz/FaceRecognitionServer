#include "udpClient.h"
#include <applicationMessages.h>

#include <iostream>

using std::make_unique;
using std::string;
using std::cout;
using std::vector;

UdpClient::UdpClient(const string &address, uint16_t port)
    : m_endpoint{address, port} {
  m_socket = make_unique<UdpSocket>();
  m_socket->open();
}

void UdpClient::sendMessage(vector<uint8_t> &&message) {
  if(m_socket->sendTo(move(message), m_endpoint))
  {
    cout << "[INFO::UdpClient] Send message to " << m_endpoint.address << ":" <<m_endpoint.port << "\n";
  }
}

vector<uint8_t> UdpClient::receiveMessage() {
  vector<uint8_t> message;
  message.reserve(20);

  auto size = m_socket->receiveFrom(message, m_endpoint);

  return message;
}
