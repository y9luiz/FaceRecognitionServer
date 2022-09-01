#include "udpMessageReceiver.h"
#include <applicationMessages.h>
#include <udpSocket.h>

#include <memory>
#include <stdexcept>
#include <vector>
#include <optional>


using std::move;
using std::nullopt;
using std::optional;
using std::runtime_error;
using std::unique_ptr;
using std::vector;

UdpMessageReceiver::UdpMessageReceiver(unique_ptr<UdpSocket> socket)
    : m_socket(move(socket)) {
      start();
}

 UdpMessageReceiver::~UdpMessageReceiver()
{
  stop();
}

optional<ApplicationMessage> UdpMessageReceiver::receiveMessage() {
  try {
    const auto header = receiveMessageHeader();

    vector<uint8_t> buffer(header.m_payloadSize +
                           sizeof(ApplicationMessage::Header));

    receiveMessagePayload(buffer);

    return ApplicationMessage(header.m_code, header.m_payloadSize,
                              move(buffer));
  } catch (const runtime_error &error) {
    return nullopt;
  }
}

ApplicationMessage::Header UdpMessageReceiver::receiveMessageHeader() {
  vector<uint8_t> buffer(sizeof(ApplicationMessage::Header));

  const auto receivedDataSize = m_socket->receiveFrom(buffer, m_endpoint);

  if (!receivedDataSize) {
    throw runtime_error("Could not receive the message header");
  }

  return {move(buffer)};
}

void UdpMessageReceiver::receiveMessagePayload(vector<uint8_t> &buffer) {
  size_t numberOfReceivedBytes = 0;
  const auto payloadSize =
      buffer.capacity() - sizeof(ApplicationMessage::Header);
  while (numberOfReceivedBytes < payloadSize) {
    const auto receivedDataSize = m_socket->receiveFrom(buffer, m_endpoint);

    if (receivedDataSize > 0) {
      numberOfReceivedBytes += receivedDataSize;
    } else {
      throw runtime_error("Could not receive the message");
    }
  }
}

void UdpMessageReceiver::start()
{
  MessageReceiverInterface::start();
}

void UdpMessageReceiver::stop()
{
  MessageReceiverInterface::stop();
}
