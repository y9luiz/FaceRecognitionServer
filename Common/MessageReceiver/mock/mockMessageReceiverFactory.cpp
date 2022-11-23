#include "mockMessageReceiverFactory.h"
#include "messageReceiverFactory.h"
#include "parallelMessageReceiver.h"

#include <memory>

using std::shared_ptr;
using std::unique_ptr;

namespace {
MockMessageReceiverFactory *g_mock = nullptr;
}

MockMessageReceiverFactory::MockMessageReceiverFactory() { g_mock = this; }

MockMessageReceiverFactory::~MockMessageReceiverFactory() { g_mock = nullptr; }

unique_ptr<ParallelMessageReceiver>
MessageReceiverFactory::createUdpServerMessageReceiver(
    const Endpoint &endpoint) {
  return g_mock->localCreateUdpServerMessageReceiver(endpoint);
}

unique_ptr<MessageReceiverInterface>
MessageReceiverFactory::createUdpClientMessageReceiver(
    shared_ptr<UdpSocket> socket) {
  return g_mock->localCreateUdpClientMessageReceiver(socket);
}