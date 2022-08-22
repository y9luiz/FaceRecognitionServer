#pragma once

#include "iMessageReceiver.h"

#include <memory>

class UdpSocket;
class UdpMessageReceiver;
class Endpoint;

class MessageReceiverBuilder {
public:
  MessageReceiverBuilder();

  std::unique_ptr<IMessageReceiver>
  createUdpServerMessageReceiver(const Endpoint &localEndpoint);
  std::unique_ptr<IMessageReceiver> createUdpClientMessageReceiver();
};
