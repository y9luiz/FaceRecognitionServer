#include <exception>
#include <gtest/gtest.h>
#include <type_traits>
#include <udpClient.h>

#include <gmock/gmock.h>

namespace {
const auto DefaultIpAddress = "127.0.0.1";
const auto DefaultPort = 5000;
} // namespace

using namespace testing;

using std::make_unique;
using std::move;
using std::unique_ptr;

class TestUdpClient : public Test {
public:
  TestUdpClient() {}

  void createUut() {
    m_uut = make_unique<UdpClient>(DefaultIpAddress, DefaultPort);
  }

  unique_ptr<UdpClient> m_uut;
};
