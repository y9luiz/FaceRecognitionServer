#include "udpClient.h"

#include <endpoint.h>

#include <mockApplicationMessages.h>
#include <mockMessageReceiver.h>
#include <mockMessageReceiverFactory.h>
#include <mockUdpMessageSender.h>
#include <mockUdpSocket.h>

#include <exception>
#include <gmock/gmock.h>
#include <optional>
#include <memory>


namespace {
const auto DefaultIpAddress = "127.0.0.1";
const auto DefaultPort = 5000;
} // namespace

using namespace testing;

using std::make_pair;
using std::make_unique;
using std::move;
using std::optional;
using std::pair;
using std::unique_ptr;
using std::vector;

class TestUdpClient : public Test {
public:
  TestUdpClient() {

    auto mockMessageReceiver = make_unique<NiceMock<MockMessageReceiver>>();
    m_mockMessageReceiver = mockMessageReceiver.get();
    EXPECT_CALL(m_mockMessageReceiverFactory,
                localCreateUdpClientMessageReceiver)
        .WillOnce(Return(ByMove(move(mockMessageReceiver))));

    EXPECT_CALL(m_mockUdpSocket, isOpen).WillRepeatedly(Return(true));

    createUut();
  }

  void createUut() {
    m_uut = make_unique<UdpClient>(DefaultIpAddress, DefaultPort);
  }

  NiceMock<MockUdpSocket> m_mockUdpSocket;
  NiceMock<MockMessageReceiverFactory> m_mockMessageReceiverFactory;
  NiceMock<MockMessageReceiver> *m_mockMessageReceiver;
  NiceMock<MockUdpMessageSender> m_mockMessageSender;

  unique_ptr<UdpClient> m_uut;
};

TEST_F(TestUdpClient, sendMessage) {
  unique_ptr<ApplicationMessage> message = make_unique<MockApplicationMessage>();
  EXPECT_CALL(m_mockMessageSender, sendMessage(_, _))
      .WillOnce(Invoke([&message](unique_ptr<ApplicationMessage> msg, const Endpoint &) {
        message = move(msg);
      }));

  m_uut->sendMessage(move(message));
}

TEST_F(TestUdpClient, invalidMessageWhenNotReceive) {

  EXPECT_CALL(*m_mockMessageReceiver, receiveMessage())
      .WillOnce(Return(ByMove(move(std::nullopt))));

  auto invalidMessage = move(m_uut->receiveMessage());

  EXPECT_THAT(invalidMessage, IsNull());
}

TEST_F(TestUdpClient, receive) {

  auto endpoint = Endpoint{DefaultIpAddress, DefaultPort};

  auto ret = optional<pair<unique_ptr<ApplicationMessage>, vector<uint8_t>>>(
      make_pair<unique_ptr<ApplicationMessage>, vector<uint8_t>>(make_unique<MockApplicationMessage>(),
                                                     endpoint.serialize()));

  EXPECT_CALL(*m_mockMessageReceiver, receiveMessage()).WillOnce(Return(ByMove(move(ret))));

  auto receivedMessage = m_uut->receiveMessage();

  EXPECT_THAT(receivedMessage, Not(IsNull()));
}
