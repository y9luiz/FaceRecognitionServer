#include "udpClient.h"
#include <mockUdpSocketFactory.h>

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
    EXPECT_CALL(m_mockUdpSocketFactory, constructor(_));
    EXPECT_CALL(m_mockUdpSocketFactory, createAndOpenSocket(_))
        .WillOnce(Return(
            ByMove(make_unique<boost::asio::ip::udp::socket>(m_ioContext))));

    createUut();
  }

  void createUut() {
    m_uut = make_unique<UdpClient>(DefaultIpAddress, DefaultPort);
  }

  MockUdpSocketFactory m_mockUdpSocketFactory;

  io_context m_ioContext;
  unique_ptr<UdpClient> m_uut;
};

TEST_F(TestUdpClient, sendMessage) {
  EXPECT_THROW(m_uut->sendMessage({}), system_error);
}

TEST_F(TestUdpClient, receiveMessage) {
  EXPECT_THROW(m_uut->receiveMessage(), system_error);
}