#pragma once

#include "messageReceiverInterface.h"
#include "parallelMessageReceiver.h"

#include <memory>

class UdpSocket;
class Endpoint;

class MessageReceiverFactory {
public:
  static std::unique_ptr<ParallelMessageReceiver>
  createUdpServerMessageReceiver(const Endpoint &localEndpoint);
  static std::unique_ptr<MessageReceiverInterface>
  createUdpClientMessageReceiver(std::shared_ptr<UdpSocket> socket);
};
