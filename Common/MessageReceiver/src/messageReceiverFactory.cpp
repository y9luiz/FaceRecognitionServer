#include "messageReceiverFactory.h"
#include "messageReceiverInterface.h"
#include "udpMessageReceiver.h"
#include "udpParallelMessageReceiver.h"

#include <udpSocket.h>

#include <iostream>
#include <memory>
#include <stdexcept>

using std::logic_error;
using std::make_unique;
using std::move;
using std::shared_ptr;
using std::unique_ptr;

unique_ptr<ParallelMessageReceiver>
MessageReceiverFactory::createUdpServerMessageReceiver(
    const Endpoint &localEndpoint) {
  unique_ptr<UdpSocket> socket = make_unique<UdpSocket>();

  socket->bind(localEndpoint);

  auto udpMessageReceiver =
      make_unique<UdpParallelMessageReceiver>(move(socket));

  return udpMessageReceiver;
}

unique_ptr<MessageReceiverInterface>
MessageReceiverFactory::createUdpClientMessageReceiver(
    shared_ptr<UdpSocket> socket) {
  if (!socket->isOpen()) {
    throw logic_error(
        "Could not create  UDP Message Receiver, socket is not open");
  }

  auto udpMessageReceiver = make_unique<UdpMessageReceiver>(socket);

  return udpMessageReceiver;
}