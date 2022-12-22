#include "endpoint.h"
#include "udpMessageSender.h"
#include <applicationMessages.h>
#include <gmock/gmock-nice-strict.h>
#include <serializer.h>


#include <mockUdpSocket.h>
#include <mockApplicationMessages.h>

#include <gmock/gmock.h>
#include <iterator>
#include <memory>
#include <thread>


using namespace testing;
using namespace std::chrono_literals;

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

  vector<uint8_t> createMessage(ApplicationMessage::Code code, const vector<uint8_t>& payload){
    vector<uint8_t> messageBytes;
    messageBytes.reserve(sizeof(ApplicationMessage::Code) + sizeof(uint32_t) + payload.size());

    messageBytes.push_back(static_cast<uint8_t>(code));

    const auto payloadSizeBytes = Serializer::u32ToBytes(payload.size());

    copy(payloadSizeBytes.begin(),payloadSizeBytes.end(),back_inserter(messageBytes));
    copy(payload.begin(),payload.end(),back_inserter(messageBytes));

    return messageBytes;
  }

  unique_ptr<MockApplicationMessage> m_mockApplicationMessage;
  NiceMock<MockUdpSocket> m_mockUdpSocket;

  Endpoint m_destination{"127.0.0.1", 5000};
  unique_ptr<UdpMessageSender> m_uut;
};

TEST_F(TestUdpMessageSender, getSocket) {
  EXPECT_THAT(m_uut->socket(), NotNull());
}

TEST_F(TestUdpMessageSender, sendSmallMessage) {
  EXPECT_CALL(m_mockUdpSocket, sendTo(_, m_destination)).Times(1);

  m_mockApplicationMessage = make_unique<MockApplicationMessage>();

  m_uut->sendMessage(move(m_mockApplicationMessage), m_destination);
}

TEST_F(TestUdpMessageSender, sendBigMessage) {
  m_mockApplicationMessage = make_unique<MockApplicationMessage>();

  vector<uint8_t> messageBytes = createMessage(ApplicationMessage::Code::FaceDetectionResponse,DefaultPayload);

  const auto totalOfPackets =
      messageBytes.size() / MaximumPacketSize +
      (messageBytes.size() % MaximumPacketSize ? 1 : 0);

  EXPECT_CALL(*m_mockApplicationMessage,serialize()).WillRepeatedly(Return(messageBytes));

  EXPECT_CALL(m_mockUdpSocket, sendTo(_, m_destination))
      .Times(totalOfPackets)
      .WillRepeatedly(Return(MaximumPacketSize));

  m_uut->sendMessage(move(m_mockApplicationMessage), m_destination);
}