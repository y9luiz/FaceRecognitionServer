#include "mockUdpSocket.h"
#include "udpSocket.h"
#include <stdexcept>

using std::logic_error;
using std::vector;

namespace {
MockUdpSocket *g_mock = nullptr;

void assertMockUdpSocketExists() {
  if (!g_mock) {
    throw logic_error("MockUdpSocket doesn't exist");
  }
}

void assertMockUdpSocketNotExists() {
  if (g_mock) {
    throw logic_error("MockUdpSocket already exist");
  }
}

} // namespace

MockUdpSocket::MockUdpSocket() {
  assertMockUdpSocketNotExists();
  g_mock = this;
}

MockUdpSocket::~MockUdpSocket() {
  assertMockUdpSocketExists();
  g_mock = nullptr;
}

UdpSocket::UdpSocket(IpProtocolVersion ipProtocolVersion) {
  assertMockUdpSocketExists();
  g_mock->constructor(ipProtocolVersion);
}

bool UdpSocket::isOpen() const {
  assertMockUdpSocketExists();
  return g_mock->isOpen();
}

bool UdpSocket::isBindMode() const {
  assertMockUdpSocketExists();
  return g_mock->isBindMode();
}

void UdpSocket::bind(const Endpoint &endpoint) {
  assertMockUdpSocketExists();
  g_mock->bind(endpoint);
}

void UdpSocket::open() {
  assertMockUdpSocketExists();
  g_mock->open();
}

size_t UdpSocket::receiveFrom(vector<uint8_t> &buffer, Endpoint &endpoint) {
  assertMockUdpSocketExists();
  return g_mock->receiveFrom(buffer, endpoint);
}

size_t UdpSocket::receive(vector<uint8_t> &buffer) {
  assertMockUdpSocketExists();
  return g_mock->receive(buffer);
}

size_t UdpSocket::sendTo(const vector<uint8_t> &buffer,
                         const Endpoint &endpoint) {
  assertMockUdpSocketExists();
  return g_mock->sendTo(buffer, endpoint);
}