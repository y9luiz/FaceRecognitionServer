#include "udpClient.h"
#include "messageSenderFactory.h"

#include <applicationMessages.h>
#include <messageReceiverFactory.h>
#include <udpSocket.h>

#include <iostream>

using std::cout;
using std::string;
using std::vector;

UdpClient::UdpClient(const string &address, uint16_t port)
    : m_destinationEndpoint{address, port} {
  m_udpMessageReceiver =
      MessageReceiverFactory().createUdpClientMessageReceiver();
  m_udpMessageSender = MessageSenderFactory::createUdpMessageSender();
}

UdpClient::~UdpClient() { m_udpMessageReceiver->stop(); }

void UdpClient::sendMessage(ApplicationMessage &&applicationMessage) {

  MessageSenderFactory::sendMessage(
      *m_udpMessageSender, std::move(applicationMessage),
                                    m_destinationEndpoint);

  cout << "[INFO::UdpClient] Send message to " << m_destinationEndpoint.address
       << ":" << m_destinationEndpoint.port << "\n";
}