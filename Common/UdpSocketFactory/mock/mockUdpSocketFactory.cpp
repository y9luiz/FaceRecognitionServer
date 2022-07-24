#include "mockUdpSocketFactory.h"
#include "udpSocketFactory.h"

#include <memory>

using std::logic_error;
using std::make_unique;
using std::unique_ptr;

namespace {
MockUdpSocketFactory *g_mock = nullptr;

void throwIfNull() {
  if (!g_mock) {
    throw logic_error("MockUdpSocketFactory could not be nullptr");
  }
}

void throwIfNotNull() {
  if (g_mock) {
    throw logic_error("MockUdpSocketFactory should be nullptr");
  }
}

} // namespace

MockUdpSocketFactory::MockUdpSocketFactory() {
  throwIfNotNull();
  g_mock = this;
}

MockUdpSocketFactory::~MockUdpSocketFactory() {
  throwIfNull();
  g_mock = nullptr;
}

UdpSocketFactory::UdpSocketFactory(boost::asio::io_context &ioContext)
    : m_ioContext(ioContext) {
  throwIfNull();
  g_mock->constructor(ioContext);
}

unique_ptr<UdpSocketFactory::UdpSocket>
UdpSocketFactory::createAndOpenSocket(Udp ipProtocol) {
  throwIfNull();
  return g_mock->createAndOpenSocket(ipProtocol);
}

unique_ptr<UdpSocketFactory::UdpSocket>
UdpSocketFactory::createOpenAndBindSocket(
    boost::asio::ip::udp::endpoint endpoint) {
  throwIfNull();
  return g_mock->createOpenAndBindSocket(endpoint);
}
