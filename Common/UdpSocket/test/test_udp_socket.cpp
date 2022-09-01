#include "udpSocket.h"

#include <chrono>
#include <future>
#include <gmock/gmock.h>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <system_error>
#include <thread>

using namespace testing;
using namespace std::chrono_literals;

using std::cref;
using std::logic_error;
using std::make_unique;
using std::ref;
using std::thread;
using std::unique_ptr;
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

  unique_ptr<thread> createReceiverThread(vector<uint8_t> &receivedData, uint16_t expectedNumberOfBytesToReceive) {
    return make_unique<thread>(
        [this,expectedNumberOfBytesToReceive](vector<uint8_t> &buffer) {
          Endpoint receiverEndpoint{DefaultAddress, DefaultPort};
          UdpSocket socket;

          socket.bind(receiverEndpoint);
          EXPECT_THAT(socket.receiveFrom(buffer, receiverEndpoint),expectedNumberOfBytesToReceive);
        },
        ref(receivedData));
  }

  unique_ptr<thread> createSenderThread(UdpSocket &senderSocket,
                                        const vector<uint8_t> &sendedData) {
    return make_unique<thread>(
        [this](UdpSocket &senderSocket, const vector<uint8_t> &buffer) {
          const auto expectedNumberOfBytesToSend = buffer.size();
          EXPECT_THAT(senderSocket.sendTo(buffer, {DefaultAddress, DefaultPort}),expectedNumberOfBytesToSend);
        },
        ref(senderSocket), cref(sendedData));
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
  UdpSocket senderSocket;
  senderSocket.open();
  const vector<uint8_t> sendedData = {'h', 'e', 'l', 'l', 'o'};
  vector<uint8_t> receivedData(sendedData.size());
  {
    auto receiveThread = createReceiverThread(receivedData, sendedData.size());

    std::this_thread::sleep_for(200ms);

    auto senderThread = createSenderThread(senderSocket, sendedData);

    receiveThread->join();
    senderThread->join();
  }
  EXPECT_THAT(receivedData, Eq(sendedData));
}