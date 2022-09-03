#include "udpClient.h"

#include <mockUdpSocket.h>
#include <mockApplicationMessages.h>
#include <mockMessageReceiver.h>

#include <exception>
#include <gmock/gmock.h>
#include <type_traits>

namespace {
const auto DefaultIpAddress = "127.0.0.1";
const auto DefaultPort = 5000;
} // namespace

using namespace testing;

using std::make_unique;
using std::unique_ptr;

class TestUdpClient : public Test {
public:
  TestUdpClient() {
    EXPECT_CALL(m_mockUdpSocket, constructor(_)).Times(2);
    EXPECT_CALL(m_mockUdpSocket, open).Times(2);
    EXPECT_CALL(m_mockMessageReceiver,start);

    createUut();
  }

  void createUut() {
    m_uut = make_unique<UdpClient>(DefaultIpAddress, DefaultPort);
  }

  NiceMock<MockUdpSocket> m_mockUdpSocket;
  NiceMock<MockApplicationMessage> m_mockApplicationMessage;
  NiceMock<MockMessageReceiver> m_mockMessageReceiver;

  unique_ptr<UdpClient> m_uut;
};

TEST_F(TestUdpClient, sendMessage) {
  EXPECT_CALL(m_mockUdpSocket, sendTo(_, _));
  m_uut->sendMessage({});
}
