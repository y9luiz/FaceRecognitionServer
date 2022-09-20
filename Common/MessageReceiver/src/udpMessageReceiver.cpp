#include "udpMessageReceiver.h"
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

  vector<uint8_t> buffer(MaximumPacketSize);

  Endpoint endpoint;
  auto receivedDataSize = m_socket->receiveFrom(buffer, endpoint);

  if (!receivedDataSize) {
    return nullopt;
  }

  ApplicationMessage::Header header(buffer);

  for (int i = 0; i < sizeof(ApplicationMessage::Header); i++) {
    buffer.erase(buffer.begin());
  }

  ApplicationMessage message(header, move(buffer));

  uint32_t amountOfBytesReceived = receivedDataSize;
  const auto expectMessageTotalSize = header.payloadSize + sizeof(header);

  buffer.resize(MaximumPacketSize);

  while (amountOfBytesReceived < expectMessageTotalSize) {
    receivedDataSize = m_socket->receive(buffer);
    if (receivedDataSize) {
      amountOfBytesReceived += receivedDataSize;
      copy(buffer.begin(), buffer.begin() + receivedDataSize,
           back_inserter(message.payload()));
    } else {
      cout << "[ERROR::receiveMessage] could not receive message body\n";
      return nullopt;
    }
  }
  return make_pair<ApplicationMessage, Origin>(
      move(message), Serializer::EndpointToBytes(endpoint));
}
