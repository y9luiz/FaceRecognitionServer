#include <udpSocketFactory.h>

#include <gmock/gmock.h>
#include <memory>

using namespace testing;
using namespace boost::asio;

namespace {
constexpr auto DefaultUrl = "127.0.0.1";
constexpr auto DefaultPort = 5000;
} // namespace

using boost::asio::ip::udp;
using std::make_unique;
using std::unique_ptr;

class TestUdpSocketFactory : public Test {

public:
  TestUdpSocketFactory()
      : m_endpoint(ip::make_address(DefaultUrl), DefaultPort) {
    m_uut = make_unique<UdpSocketFactory>(m_ioContext);
  }

  io_context m_ioContext;
  udp::endpoint m_endpoint;

  unique_ptr<UdpSocketFactory> m_uut;
};

TEST_F(TestUdpSocketFactory, createAndOpenSocket) {
  m_uut->createAndOpenSocket(udp::v4());
}

TEST_F(TestUdpSocketFactory, createOpenAndBindSocket) {
  m_uut->createOpenAndBindSocket(
      udp::endpoint(ip::address::from_string(DefaultUrl), DefaultPort));
}