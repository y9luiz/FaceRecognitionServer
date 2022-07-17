#include <udpSocketFactory.h>

#include <gmock/gmock.h>
#include <memory>

using namespace testing;

namespace {
constexpr auto DefaultUrl = "127.0.0.1";
constexpr auto DefaultPort = 5000;
} // namespace

using std::make_unique;
using std::unique_ptr;

class TestUdpSocketFactory : public Test {

public:
  TestUdpSocketFactory()
      : m_endpoint(boost::asio::ip::make_address(DefaultUrl), DefaultPort) {
    m_uut = make_unique<UdpSocketFactory>(m_ioContext);
  }

  boost::asio::io_context m_ioContext;
  boost::asio::ip::udp::endpoint m_endpoint;

  unique_ptr<UdpSocketFactory> m_uut;
};

TEST_F(TestUdpSocketFactory, createAndOpenSocketUsingIpV4) {
  // m_uut->createAndOpenSocket(boost::asio::ip::udp::v4());
}
/*
TEST_F(TestUdpSocketFactory, createAndOpenSocketUsingIpV6) {
  m_uut->createAndOpenSocket(boost::asio::ip::udp::v4());
}

TEST_F(TestUdpSocketFactory, createOpenAndBindSocke) {

  m_uut->createOpenAndBindSocket(m_endpoint);
}*/