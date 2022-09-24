#include "applicationMessages.h"
#include "endpoint.h"
#include "udpMessageSender.h"
#include <applicationMessages.h>
#include <serializer.h>


#include <mockUdpSocket.h>

#include <gmock/gmock.h>
#include <iterator>
#include <memory>
#include <thread>


using namespace testing;
using namespace std::chrono_literals;

using std::back_inserter;
using std::copy;
using std::make_unique;
using std::move;
using std::unique_ptr;
using std::vector;

namespace {
const auto DefaultPayload = vector<uint8_t>(4500, 'p');
}; // namespace

class TestUdpMessageSender : public Test {

public:
  TestUdpMessageSender() {
    setupConstuctorExpectations();
    m_uut = make_unique<UdpMessageSender>();
  }

  ~TestUdpMessageSender() { std::this_thread::sleep_for(1s); }

  void setupConstuctorExpectations() {
    EXPECT_CALL(m_mockUdpSocket, constructor);
    EXPECT_CALL(m_mockUdpSocket, open);
  }

  NiceMock<MockUdpSocket> m_mockUdpSocket;

  Endpoint m_destination{"127.0.0.1", 5000};
  unique_ptr<UdpMessageSender> m_uut;
};

TEST_F(TestUdpMessageSender, getSocket) {
  EXPECT_THAT(m_uut->socket(), NotNull());
}

TEST_F(TestUdpMessageSender, sendSmallMessage) {
  ApplicationMessage applicationMessage(0, {});

  EXPECT_CALL(m_mockUdpSocket, sendTo(_, m_destination)).Times(1);

  m_uut->sendMessage(move(applicationMessage), m_destination);
}

TEST_F(TestUdpMessageSender, sendBigMessage) {
  auto payloadCopy = DefaultPayload;
  ApplicationMessage applicationMessage(0, move(payloadCopy));

  const auto totalOfPackets =
      applicationMessage.size() / MaximumPacketSize +
      (applicationMessage.size() % MaximumPacketSize ? 1 : 0);

  EXPECT_CALL(m_mockUdpSocket, sendTo(_, m_destination))
      .Times(totalOfPackets)
      .WillRepeatedly(Return(MaximumPacketSize));

  m_uut->sendMessage(move(applicationMessage), m_destination);
}