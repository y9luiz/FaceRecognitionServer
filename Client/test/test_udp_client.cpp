#include "udpClient.h"

#include <applicationMessages.h>
#include <endpoint.h>

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
const auto DefaultMessage =
    ApplicationMessage(2, {'P', 'a', 'y', 'l', 'o', 'a', 'd'});

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

  ApplicationMessage message = DefaultMessage;

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

  auto movedMessage = DefaultMessage;
  auto endpoint = Endpoint{DefaultIpAddress, DefaultPort};

  auto ret = optional<pair<ApplicationMessage, vector<uint8_t>>>(
      make_pair<ApplicationMessage, vector<uint8_t>>(move(movedMessage),
                                                     endpoint.toBytes()));

  EXPECT_CALL(*m_mockMessageReceiver, receiveMessage()).WillOnce(Return(ret));

  auto receivedMessage = m_uut->receiveMessage();

  EXPECT_THAT(receivedMessage, DefaultMessage);
}
