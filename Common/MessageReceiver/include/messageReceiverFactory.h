#pragma once

#include "messageReceiverInterface.h"

#include <memory>

class UdpSocket;
class UdpMessageReceiver;
class Endpoint;

class MessageReceiverFactory {
public:
  MessageReceiverFactory();

  std::unique_ptr<MessageReceiverInterface>
  createUdpServerMessageReceiver(const Endpoint &localEndpoint);
  std::unique_ptr<MessageReceiverInterface> createUdpClientMessageReceiver();
};
