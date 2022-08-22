#include "udpClient.h"
#include <mockUdpSocket.h>

#include <boost/asio/io_context.hpp>
#include <boost/system/system_error.hpp>
#include <exception>
#include <gmock/gmock.h>
#include <type_traits>

namespace {
const auto DefaultIpAddress = "127.0.0.1";
const auto DefaultPort = 5000;
} // namespace

using namespace testing;
using namespace boost::asio;

using boost::system::system_error;
using std::make_unique;
using std::unique_ptr;

class TestUdpClient : public Test {
public:
  TestUdpClient() {
    EXPECT_CALL(m_mockUdpSocket, constructor(_));
    EXPECT_CALL(m_mockUdpSocket, open);

    createUut();
  }

  void createUut() {
    m_uut = make_unique<UdpClient>(DefaultIpAddress, DefaultPort);
  }

  NiceMock<MockUdpSocket> m_mockUdpSocket;

  unique_ptr<UdpClient> m_uut;
};

TEST_F(TestUdpClient, sendMessage) {
  EXPECT_CALL(m_mockUdpSocket, sendTo(_, _));
  m_uut->sendMessage({});
}

TEST_F(TestUdpClient, receiveMessage) {
  EXPECT_CALL(m_mockUdpSocket, receiveFrom(_, _));
  m_uut->receiveMessage();
}