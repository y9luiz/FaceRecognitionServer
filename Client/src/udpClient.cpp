#include "udpClient.h"
#include "udpMessageSender.h"

#include <faceDetectionResponse.h>
#include <memory>
#include <messageReceiverFactory.h>

#include <iostream>
#include <stdexcept>

using std::cout;
using std::logic_error;
using std::make_unique;
using std::move;
using std::string;

UdpClient::UdpClient(const string &address, uint16_t port)
    : m_destinationEndpoint{address, port} {
  m_udpMessageSender = make_unique<UdpMessageSender>();

  m_udpMessageReceiver = MessageReceiverFactory::createUdpClientMessageReceiver(
      m_udpMessageSender->socket());
}

void UdpClient::sendMessage(ApplicationMessage &&applicationMessage) {

  if (!m_udpMessageSender) {
    throw logic_error(
        "Could not sendapplication  message. UDP Message Sender is null.");
  }

  m_udpMessageSender->sendMessage(move(applicationMessage),
                                  m_destinationEndpoint);
  cout << "[INFO::UdpClient] Send message to " << m_destinationEndpoint.address
       << ":" << m_destinationEndpoint.port << "\n";
}

ApplicationMessage UdpClient::receiveMessage() {
  auto optionalMessageAndSender = m_udpMessageReceiver->receiveMessage();
  return optionalMessageAndSender.value().first;
}