#include "udpClient.h"

#include <faceDetectionResponse.h>
#include <messageReceiverFactory.h>

#include <iostream>
#include <memory>
#include <stdexcept>

using std::cout;
using std::logic_error;
using std::make_unique;
using std::move;
using std::string;

UdpClient::UdpClient(const string &ipAddress, uint16_t port)
    : m_destinationEndpoint{ipAddress, port} {
  m_udpMessageSender = make_unique<UdpMessageSender>();

  m_udpMessageReceiver = MessageReceiverFactory::createUdpClientMessageReceiver(
      m_udpMessageSender->socket());
}

void UdpClient::sendMessage(ApplicationMessage &&applicationMessage) {

  if (!m_udpMessageSender) {
    throw logic_error(
        "Could not send application  message. UDP Message Sender is null.");
  }

  m_udpMessageSender->sendMessage(move(applicationMessage),
                                  m_destinationEndpoint);
  cout << "[INFO::UdpClient] Sent message to " << m_destinationEndpoint.address
       << ":" << m_destinationEndpoint.port << "\n";
}

ApplicationMessage UdpClient::receiveMessage() {
  auto optionalMessageAndSender = m_udpMessageReceiver->receiveMessage();

  if (optionalMessageAndSender.has_value()) {
    return optionalMessageAndSender.value().first;
  }

  const auto invalidCode =
      static_cast<uint8_t>(ApplicationMessage::Types::InvalidMessage);

  // Todo: Create InvalidMessage class
  return ApplicationMessage(invalidCode, {});
}