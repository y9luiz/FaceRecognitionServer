#include "udpMessageReceiver.h"
#include "endpoint.h"
#include "messageReceiverInterface.h"

#include <applicationMessages.h>
#include <serializer.h>

#include <iostream>

using std::cout;
using std::make_pair;
using std::make_shared;
using std::move;
using std::nullopt;
using std::optional;
using std::pair;
using std::shared_ptr;
using std::vector;

UdpMessageReceiver::UdpMessageReceiver() : m_socket(make_shared<UdpSocket>()) {}

UdpMessageReceiver::UdpMessageReceiver(shared_ptr<UdpSocket> socket)
    : m_socket(move(socket)) {}

optional<pair<ApplicationMessage, MessageReceiverInterface::Origin>>
UdpMessageReceiver::receiveMessage() {

  vector<uint8_t> tempBuffer(MaximumPacketSize);

  Endpoint endpoint;
  auto receivedDataSize = m_socket->receiveFrom(tempBuffer, endpoint);

  if (!receivedDataSize) {
    return nullopt;
  }

  ApplicationMessage::Header header(tempBuffer);

  uint32_t amountOfBytesReceived = receivedDataSize;
  const auto expectMessageTotalSize =
      header.payloadSize + sizeof(ApplicationMessage::Header);

  vector<uint8_t> message;
  message.reserve(expectMessageTotalSize);

  copy(tempBuffer.begin(), tempBuffer.end(), back_inserter(message));
  tempBuffer.resize(MaximumPacketSize);
  while (amountOfBytesReceived < expectMessageTotalSize) {
    receivedDataSize = m_socket->receive(tempBuffer);
    if (receivedDataSize) {
      amountOfBytesReceived += receivedDataSize;
      move(tempBuffer.begin(), tempBuffer.begin() + receivedDataSize,
           back_inserter(message));
    } else {
      cout << "[ERROR::receiveMessage] could not receive message body\n";
      return nullopt;
    }
  }

  return make_pair<ApplicationMessage, Origin>(
      ApplicationMessage(header.code, move(message)), endpoint.toBytes());
}
