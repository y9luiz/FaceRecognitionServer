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
using std::runtime_error;
using std::unique_ptr;
using std::vector;

namespace {
void receiveMessageHeader(UdpSocket &socket, vector<uint8_t> &outputBuffer) {}
} // namespace

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

  uint32_t totalMessageSize = receivedDataSize;

  while (receivedDataSize < header.payloadSize) {
    auto receivedDataSize = m_socket->receive(message.payload());

    if (receivedDataSize) {
      totalMessageSize += receivedDataSize;
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
