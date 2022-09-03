#include "applicationMessages.h"

#include <gmock/gmock.h>
#include <memory>
#include <stdexcept>
#include <stdint.h>

using namespace testing;

using std::invalid_argument;
using std::make_unique;
using std::unique_ptr;
using std::vector;

namespace {
constexpr auto DefaultMessageCode = 10u;
constexpr auto DefaultPayloadSize = 300u;
const vector<uint8_t> DefaultPayload{'p', 'a', 'y', 'l', 'o', 'a', 'd'};

}; // namespace

class TestApplicationMessages : public testing::Test {
public:
  TestApplicationMessages() {}

  vector<uint8_t> createDefaultMessageAsBytes() {
    vector<uint8_t> message;

    message.push_back(DefaultMessageCode);
    message.push_back(static_cast<uint8_t>(DefaultPayloadSize));
    message.push_back(DefaultPayloadSize >> 8);

    copy(DefaultPayload.begin(), DefaultPayload.end(), back_inserter(message));

    return message;
  }

  void createApplicationMessage(vector<uint8_t> &&message) {
    auto messageCopy = message;
    m_uut = make_unique<ApplicationMessage>(move(message));

    EXPECT_THAT(m_uut->convertToBytes(), ContainerEq(messageCopy));
  }

  void createApplicationMessage(uint8_t messageCode, uint16_t payloadSize,
                                vector<uint8_t> payload) {
    auto payloadCopy = payload;
    m_uut = make_unique<ApplicationMessage>(messageCode, payloadSize,
                                            move(payload));

    EXPECT_THAT(m_uut->header().code, messageCode);
    EXPECT_THAT(m_uut->header().payloadSize, payloadSize);
    EXPECT_THAT(m_uut->payload(), ContainerEq(payloadCopy));
  }

  unique_ptr<ApplicationMessage> m_uut;
};

TEST_F(TestApplicationMessages, ShouldCreateNotMessageWithNotValidSize) {

  auto createEmptyMessage = [this]() {
    m_uut = make_unique<ApplicationMessage>(vector<uint8_t>{});
  };

  EXPECT_THROW(createEmptyMessage(), invalid_argument);
}

TEST_F(TestApplicationMessages, CreateMessageUsingParameters) {
  createApplicationMessage(DefaultMessageCode, DefaultPayloadSize,
                           DefaultPayload);
}

TEST_F(TestApplicationMessages, CreateMessageUsingRawVector) {
  auto messageBuffer = createDefaultMessageAsBytes();
  createApplicationMessage(move(messageBuffer));
}
