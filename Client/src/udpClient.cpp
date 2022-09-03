#include "udpClient.h"

#include <messageReceiverBuilder.h>
#include <applicationMessages.h>
#include <udpSocket.h>

#include <iostream>

using std::cout;
using std::string;
using std::vector;

UdpClient::UdpClient(const string &address, uint16_t port)
    : m_endpoint{address, port} {
  m_messageReceiver = MessageReceiverBuilder().createUdpClientMessageReceiver();
  m_socket.open();
}

UdpClient::~UdpClient() { m_messageReceiver->stop(); }

void UdpClient::sendMessage(vector<uint8_t> &&message) {
  if (m_socket.sendTo(move(message), m_endpoint)) {
    cout << "[INFO::UdpClient] Send message to " << m_endpoint.address << ":"
         << m_endpoint.port << "\n";
  }
}