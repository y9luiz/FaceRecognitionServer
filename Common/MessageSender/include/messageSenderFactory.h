#pragma once

#include "applicationMessages.h"
#include "messageSenderInterface.h"

class UdpSocket;
class UdpMessageSender;
class Endpoint;

class MessageSenderFactory {
public:
  static std::unique_ptr<UdpMessageSender> createUdpMessageSender();

  static void sendMessage(MessageSenderInterface &udpMessageSender,
                          ApplicationMessage &&message,
                          const Endpoint &Endpoint);
};