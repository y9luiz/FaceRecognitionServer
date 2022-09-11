#include "udpClient.h"

#include <applicationMessages.h>
#include <messageReceiverFactory.h>
#include <udpSocket.h>

#include <iostream>

using std::cout;
using std::string;
using std::vector;

UdpClient::UdpClient(const string &address, uint16_t port)
    : m_destinationEndpoint{address, port} {
  m_messageReceiver = MessageReceiverFactory().createUdpClientMessageReceiver();
  m_socket.open();
}

UdpClient::~UdpClient() { m_messageReceiver->stop(); }

void UdpClient::sendMessage(vector<uint8_t> &&message) {
  if (m_socket.sendTo(move(message), m_destinationEndpoint)) {
    cout << "[INFO::UdpClient] Send message to "
         << m_destinationEndpoint.address << ":" << m_destinationEndpoint.port
         << "\n";
  }
}