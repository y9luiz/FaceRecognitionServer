#include "udpMessageReceiver.h"
#include <messageReceiverFactory.h>
#include <udpSocket.h>

#include <memory>

using std::make_unique;
using std::move;
using std::unique_ptr;

MessageReceiverFactory::MessageReceiverFactory() {}

unique_ptr<MessageReceiverInterface>
MessageReceiverFactory::createUdpServerMessageReceiver(
    const Endpoint &localEndpoint) {
  unique_ptr<UdpSocket> socket = make_unique<UdpSocket>();

  socket->bind(localEndpoint);

  unique_ptr<MessageReceiverInterface> udpMessageReceiver =
      make_unique<UdpMessageReceiver>(move(socket));

  return udpMessageReceiver;
}

unique_ptr<MessageReceiverInterface>
MessageReceiverFactory::createUdpClientMessageReceiver() {
  unique_ptr<UdpSocket> socket = make_unique<UdpSocket>();

  socket->open();

  unique_ptr<MessageReceiverInterface> udpMessageReceiver =
      make_unique<UdpMessageReceiver>(move(socket));

  return udpMessageReceiver;
}