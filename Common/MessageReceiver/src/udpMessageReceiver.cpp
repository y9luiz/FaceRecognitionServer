#include "udpMessageReceiver.h"
#include "endpoint.h"
#include "messageReceiverInterface.h"

#include <applicationMessages.h>
#include <serializer.h>

#include <iostream>
#include <thread>
#include <optional>

using namespace std::chrono_literals;

using std::cout;
using std::make_pair;
using std::make_shared;
using std::move;
using std::nullopt;
using std::optional;
using std::pair;
using std::shared_ptr;
using std::vector;
using std::unique_ptr;

UdpMessageReceiver::UdpMessageReceiver() : m_socket(make_shared<UdpSocket>()) {}

UdpMessageReceiver::UdpMessageReceiver(shared_ptr<UdpSocket> socket)
    : m_socket(move(socket)) {}

optional<pair<unique_ptr<ApplicationMessage>, MessageReceiverInterface::Origin>>
UdpMessageReceiver::receiveMessage() {

  vector<uint8_t> tempBuffer(MaximumPacketSize);

  Endpoint endpoint;
  auto receivedDataSize = m_socket->receiveFrom(tempBuffer, endpoint);

  if (!receivedDataSize) {
    return nullopt;
  }

  uint8_t code = tempBuffer[0];
  uint32_t payloadSize = Serializer::u32FromBytes(tempBuffer.begin() + 1);

  const auto expectMessageTotalSize =
      sizeof(ApplicationMessage::Header) + payloadSize;

  vector<uint8_t> message;
  message.reserve(expectMessageTotalSize);

  copy(tempBuffer.begin(), tempBuffer.begin() + receivedDataSize,
           back_inserter(message));

  while (message.size() < expectMessageTotalSize) {
    receivedDataSize = m_socket->receive(tempBuffer);
    if (receivedDataSize) {
        copy(tempBuffer.begin(), tempBuffer.begin() + receivedDataSize,
           back_inserter(message));
    } else {
      cout << "[ERROR::receiveMessage] could not receive message body\n";
      return nullopt;
    }
  }

  if(expectMessageTotalSize  == message.size())
  {
    return make_pair(
      FactoryApplicationMessage::create(move(message)), endpoint.serialize());
  }

  return nullopt;
}
