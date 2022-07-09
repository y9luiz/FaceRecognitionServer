#include <socketManager.h>

#include <gmock/gmock.h>
#include <memory>

using namespace testing;

namespace {
constexpr auto DefaultUrl = "127.0.0.1";
constexpr auto DefaultPort = 5000;
} // namespace

using std::make_unique;
using std::unique_ptr;

class TestSocketManager : public Test {

public:
  TestSocketManager()
      : m_endpoint(boost::asio::ip::make_address(DefaultUrl), DefaultPort) {}

  unique_ptr<SocketManager> m_uut;
  boost::asio::io_context m_ioContext;
  boost::asio::ip::udp::endpoint m_endpoint;
};

TEST_F(TestSocketManager, createAsServer) {
  m_uut = make_unique<SocketManager>(m_ioContext, m_endpoint, true);
}

TEST_F(TestSocketManager, createAsClient) {
  m_uut = make_unique<SocketManager>(m_ioContext, m_endpoint, false);
}