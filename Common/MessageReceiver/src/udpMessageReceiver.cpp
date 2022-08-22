#include "udpMessageReceiver.h"
#include "applicationMessages.h"
#include <udpSocket.h>

#include <memory>
#include <vector>

using std::move;
using std::unique_ptr;
using std::vector;

UdpMessageReceiver::UdpMessageReceiver(unique_ptr<UdpSocket> socket)
    : m_socket(move(socket)) {}

ApplicationMessage UdpMessageReceiver::receiveMessage() {

  const auto header = receiveMessageHeader();

  vector<uint8_t> buffer(header.m_payloadSize +
                         sizeof(ApplicationMessage::Header));

  receiveMessagePayload(buffer);

  return ApplicationMessage(header.m_code, header.m_payloadSize, move(buffer));
}

ApplicationMessage::Header UdpMessageReceiver::receiveMessageHeader() {
  vector<uint8_t> buffer(sizeof(ApplicationMessage::Header));

  m_socket->receiveFrom(buffer, m_endpoint);

  return {move(buffer)};
}

void UdpMessageReceiver::receiveMessagePayload(vector<uint8_t> &buffer) {
  size_t numberOfReceivedBytes = 0;
  const auto payloadSize =
      buffer.capacity() - sizeof(ApplicationMessage::Header);
  while (numberOfReceivedBytes < payloadSize) {
    numberOfReceivedBytes += m_socket->receiveFrom(buffer, m_endpoint);
  }
}