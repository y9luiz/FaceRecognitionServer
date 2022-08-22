#pragma once

#include "udpMessageReceiver.h"
#include <messageReceiverBuilder.h>
#include <udpSocket.h>

#include <memory>

using std::make_unique;
using std::move;
using std::unique_ptr;

MessageReceiverBuilder::MessageReceiverBuilder() {}

unique_ptr<IMessageReceiver>
MessageReceiverBuilder::createUdpServerMessageReceiver(
    const Endpoint &localEndpoint) {
  unique_ptr<UdpSocket> socket = make_unique<UdpSocket>();

  socket->bind(localEndpoint);

  unique_ptr<UdpMessageReceiver> udpMessageReceiver =
      make_unique<UdpMessageReceiver>(move(socket));

  return udpMessageReceiver;
}

unique_ptr<IMessageReceiver>
MessageReceiverBuilder::createUdpClientMessageReceiver() {
  unique_ptr<UdpSocket> socket = make_unique<UdpSocket>();

  socket->open();

  unique_ptr<UdpMessageReceiver> udpMessageReceiver =
      make_unique<UdpMessageReceiver>(move(socket));

  return udpMessageReceiver;
}