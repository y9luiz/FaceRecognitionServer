#include "udpSocket.h"

#include <chrono>
#include <future>
#include <gmock/gmock.h>
#include <memory>
#include <stdexcept>
#include <system_error>

using namespace testing;
using namespace std::chrono_literals;

using std::logic_error;
using std::vector;

namespace {
const auto DefaultAddress = "127.0.0.1";
const auto DefaultPort = 5000;

MATCHER_P(SameEndpoint, endpoint, "Check if two endpoints structs are equal") {
  return endpoint.port == arg.port && endpoint.address == arg.address;
}
} // namespace

TEST(TestUdpSocketConstructor, constructUsingIpV4) {
  UdpSocket socket(IpProtocolVersion::V4);
  EXPECT_FALSE(socket.isOpen());
}

TEST(TestUdpSocketConstructor, openUsingIpV4) {
  UdpSocket socket(IpProtocolVersion::V4);
  socket.open();
  EXPECT_TRUE(socket.isOpen());
}

TEST(TestUdpSocketConstructor, constructUsingIpV6) {
  UdpSocket socket(IpProtocolVersion::V6);
  EXPECT_FALSE(socket.isOpen());
}

TEST(TestUdpSocketConstructor, openUsingIpV6) {
  UdpSocket socket(IpProtocolVersion::V6);
  socket.open();
  EXPECT_TRUE(socket.isOpen());
}

class TestUdpSocket : public Test {
public:
  TestUdpSocket() {}

  void open() {
    m_uut.open();
    EXPECT_TRUE(m_uut.isOpen());
  }

  UdpSocket m_uut;
};

TEST_F(TestUdpSocket, open) { open(); }

TEST_F(TestUdpSocket, bind) {
  m_uut.bind({DefaultAddress, DefaultPort});
  EXPECT_TRUE(m_uut.isBindMode());
}

TEST_F(TestUdpSocket, sendTo) {
  open();
  vector<uint8_t> outputBuffer{'f', 'a', 'k', 'e'};
  m_uut.sendTo(outputBuffer, {DefaultAddress, DefaultPort});
}

TEST_F(TestUdpSocket, sendToShouldFailWithNotOpenSocket) {
  EXPECT_THROW(m_uut.sendTo({}, {DefaultAddress, DefaultPort}), logic_error);
}

TEST_F(TestUdpSocket, sendToShouldFailWithInvalidAddress) {
  Endpoint invalidEndpoint{"not valid", DefaultPort};
  EXPECT_THROW(m_uut.sendTo({}, invalidEndpoint), logic_error);
}

TEST_F(TestUdpSocket, sendToShouldFailWithInvalidPortNumber) {
  Endpoint invalidEndpoint{DefaultAddress, 0};
  EXPECT_THROW(m_uut.sendTo({}, invalidEndpoint), logic_error);
}

TEST_F(TestUdpSocket, receiveFromTimeoutReached) {
  vector<uint8_t> outputBuffer;
  Endpoint defaultEndpoint{DefaultAddress, DefaultPort};
  m_uut.bind(defaultEndpoint);

  EXPECT_THAT(m_uut.receiveFrom(outputBuffer, defaultEndpoint), 0);
}

TEST_F(TestUdpSocket, receiveFromNotOpenedSocket) {
  vector<uint8_t> outputBuffer;
  Endpoint defaultEndpoint{DefaultAddress, DefaultPort};

  EXPECT_THROW(m_uut.receiveFrom(outputBuffer, defaultEndpoint), logic_error);
}

TEST_F(TestUdpSocket, receiveData) {
  vector<uint8_t> sendBuffer{'h', 'e', 'l', 'l', 'o'};
  vector<uint8_t> receiveBuffer(sendBuffer.size());
  Endpoint receiverEndpoint{DefaultAddress, DefaultPort};
  Endpoint senderEndpoint;

  UdpSocket sender;
  sender.open();

  m_uut.bind(receiverEndpoint);

  std::thread t(
      [&, this]() { return m_uut.receiveFrom(receiveBuffer, senderEndpoint); });
  
  std::this_thread::sleep_for(1s);

  const auto numberOfSendedBytes = sender.sendTo(sendBuffer, receiverEndpoint);

  EXPECT_THAT(receiveBuffer, Eq(sendBuffer));
  EXPECT_THAT(receiverEndpoint, SameEndpoint(senderEndpoint));
  t.join();
}