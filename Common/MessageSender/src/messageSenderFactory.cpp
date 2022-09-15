#include "messageSenderFactory.h"
#include "udpMessageSender.h"
#include "udpSocket.h"

#include <memory>
#include <utility>

using std::make_unique;
using std::move;
using std::unique_ptr;

unique_ptr<MessageSenderInterface>
MessageSenderFactory::createUdpMessageSender() {
  return make_unique<UdpMessageSender>();
}

void MessageSenderFactory::sendMessage(MessageSenderInterface &udpMessageSender,
                                       ApplicationMessage &&message,
                                       const Endpoint &endpoint) {
  UdpMessageSender::UdpDestination destination;
  destination.endpoint = endpoint;
  udpMessageSender.sendMessage(move(message), destination);
}