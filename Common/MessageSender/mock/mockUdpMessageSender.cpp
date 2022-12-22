#include "mockUdpMessageSender.h"
#include "applicationMessages.h"
#include "udpMessageSender.h"

#include <stdexcept>

using std::logic_error;
using std::move;
using std::unique_ptr;

namespace {
MockUdpMessageSender *g_mock = nullptr;

void assertMockIsNull() {
  if (g_mock) {
    throw logic_error("MockUdpMessageSender not null!");
  }
}

void assertMockIsNotNull() {
  if (!g_mock) {
    throw logic_error("MockUdpMessageSender is null!");
  }
}

} // namespace

MockUdpMessageSender::MockUdpMessageSender() {
  assertMockIsNull();
  g_mock = this;
}

MockUdpMessageSender::~MockUdpMessageSender() {
  assertMockIsNotNull();
  g_mock = nullptr;
}

UdpMessageSender::UdpMessageSender() {
  assertMockIsNotNull();
  g_mock->constructor();
}

UdpMessageSender::~UdpMessageSender() {}

void UdpMessageSender::sendMessage(unique_ptr<ApplicationMessage> message,
                                   const Endpoint &endpoint) {
  assertMockIsNotNull();
  g_mock->sendMessage(move(message), endpoint);
}