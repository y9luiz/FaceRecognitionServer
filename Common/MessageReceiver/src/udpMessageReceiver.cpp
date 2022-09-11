#include "udpMessageReceiver.h"
#include <applicationMessages.h>
#include <udpSocket.h>

#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

using std::cout;
using std::move;
using std::nullopt;
using std::optional;
using std::unique_ptr;
using std::vector;

UdpMessageReceiver::UdpMessageReceiver(unique_ptr<UdpSocket> socket)
    : m_socket(move(socket)) {
  start();
}

UdpMessageReceiver::~UdpMessageReceiver() {
  try {
    stop();
  } catch (std::exception &e) {
    cout << "[ERROR::~UdpMessageReceiver()] " << e.what() << "\n";
  }
}

optional<ApplicationMessage> UdpMessageReceiver::receiveMessage() {
  vector<uint8_t> buffer(MaximumPacketSize);

  auto receivedDataSize = m_socket->receive(buffer);

  if (!receivedDataSize) {
    return nullopt;
  }

  ApplicationMessage::Header header(buffer);

  for (int i = 0; i < sizeof(ApplicationMessage::Header); i++) {
    buffer.erase(buffer.begin());
  }

  ApplicationMessage message(header.code, header.payloadSize, move(buffer));

  uint32_t amountOfBytesReceived = receivedDataSize;
  const auto expectMessageTotalSize = header.payloadSize + sizeof(header);

  buffer.resize(MaximumPacketSize);

  while (amountOfBytesReceived < expectMessageTotalSize) {
    receivedDataSize = m_socket->receive(buffer);
    if (receivedDataSize) {
      amountOfBytesReceived += receivedDataSize;
      copy(buffer.begin(),buffer.begin() + receivedDataSize,back_inserter(message.payload()));
    } else {
      cout << "[ERROR::receiveMessage] could not receive message body\n";
      return nullopt;
    }
  }

  return message;
}

void UdpMessageReceiver::start() {
  cout << "[INFO::UdpMessageReceiver] created UdpMessageReceiver\n";
  MessageReceiverInterface::start();
}

void UdpMessageReceiver::stop() { MessageReceiverInterface::stop(); }
