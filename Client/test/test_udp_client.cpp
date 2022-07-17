#include <exception>
#include <gtest/gtest.h>
#include <type_traits>
#include <udpClient.h>

#include <gmock/gmock.h>

namespace {
const auto DefaultIpAddress = "127.0.0.1";
const auto InvalidIpAddress = "amkamka";
const auto DefaultPort = 5000;
const auto InvalidPort = -1;
} // namespace

using namespace testing;

using std::make_unique;
using std::move;
using std::unique_ptr;

class TestUdpClient : public Test {
public:
  TestUdpClient() {}

  void createUuTWithInvalidIp() {
    m_uut = make_unique<UdpClient>(InvalidIpAddress, DefaultPort);
  }

  unique_ptr<UdpClient> m_uut;
};

TEST_F(TestUdpClient, ShouldThrowWithInvalidAddr) {
  EXPECT_THROW(createUuTWithInvalidIp(), std::exception);
}