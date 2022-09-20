#include "udpClient.h"

#include <faceDetectionResponse.h>
#include <messageReceiverFactory.h>
#include <messageSenderFactory.h>

#include <iostream>
#include <stdexcept>

using std::cout;
using std::move;
using std::string;

UdpClient::UdpClient(const string &address, uint16_t port)
    : m_destinationEndpoint{address, port} {
  m_udpMessageSender = MessageSenderFactory::createUdpMessageSender();

  m_udpMessageReceiver = MessageReceiverFactory::createUdpClientMessageReceiver(
      m_udpMessageSender->socket());
}

void UdpClient::sendMessage(ApplicationMessage &&applicationMessage) {

  MessageSenderFactory::sendMessage(
      *m_udpMessageSender, move(applicationMessage), m_destinationEndpoint);

  cout << "[INFO::UdpClient] Send message to " << m_destinationEndpoint.address
       << ":" << m_destinationEndpoint.port << "\n";
}

ApplicationMessage UdpClient::receiveMessage() {
  auto optionalMessageAndSender = m_udpMessageReceiver->receiveMessage();
  if (optionalMessageAndSender.has_value()) {
    return optionalMessageAndSender.value().first;
  }
}