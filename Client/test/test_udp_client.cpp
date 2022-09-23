#include "applicationMessages.h"
#include "endpoint.h"
#include "messageReceiverInterface.h"
#include "udpClient.h"

#include <mockMessageReceiver.h>
#include <mockMessageReceiverFactory.h>
#include <mockUdpMessageSender.h>
#include <mockUdpSocket.h>

#include <exception>
#include <gmock/gmock.h>

namespace {
const auto DefaultIpAddress = "127.0.0.1";
const auto DefaultPort = 5000;

MATCHER_P(isSameMessage, message, "check if two messages are equals") {
  return arg == message;
}

} // namespace

using namespace testing;

using std::make_unique;
using std::move;
using std::unique_ptr;

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

  ApplicationMessage message(33, {'h', 'e', 'l', 'l', 'o'});

  EXPECT_CALL(m_mockMessageSender, sendMessage(move(message), _))
      .WillOnce(Invoke([&message](ApplicationMessage &&msg, const Endpoint &) {
        message = move(msg);
      }));

  m_uut->sendMessage(move(message));
}

TEST_F(TestUdpClient, invalidMessageWhenNotReceive) {

  EXPECT_CALL(*m_mockMessageReceiver, receiveMessage())
      .WillOnce(Return(std::nullopt));

  auto invalidMessage = m_uut->receiveMessage();

  EXPECT_THAT(invalidMessage.header().code,
              static_cast<uint8_t>(ApplicationMessage::Types::InvalidMessage));
}

TEST_F(TestUdpClient, receive) {

  ApplicationMessage message(2, {'P', 'a', 'y', 'l', 'o', 'a', 'd'});
  auto messageCopy = message;

  auto ret = std::optional<std::pair<ApplicationMessage, std::vector<uint8_t>>>(
      std::make_pair<ApplicationMessage, std::vector<uint8_t>>(
          move(message), Endpoint{DefaultIpAddress, DefaultPort}.toBytes()));

  EXPECT_CALL(*m_mockMessageReceiver, receiveMessage()).WillOnce(Return(ret));

  message = m_uut->receiveMessage();

  EXPECT_THAT(message, isSameMessage(messageCopy));
}