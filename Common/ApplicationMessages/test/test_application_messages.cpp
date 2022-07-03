#include "applicationMessages.h"

#include <gmock/gmock.h>
#include <memory>
#include <stdexcept>
#include <stdint.h>

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

  void createApplicationMessage(vector<uint8_t> &&message) {
    m_uut = make_unique<ApplicationMessage>(move(message));
  }

  unique_ptr<ApplicationMessage> m_uut;
};

TEST_F(TestApplicationMessages, ShouldCreateNotMessageWithNotValidSize) {

  EXPECT_THROW(createApplicationMessage({}), invalid_argument);
}

TEST_F(TestApplicationMessages, ShouldCreateMessageUsingParameters) {
  vector<uint8_t> payload = DefaultPayload;
  m_uut = make_unique<ApplicationMessage>(DefaultMessageCode,
                                          DefaultPayloadSize, move(payload));

  EXPECT_THAT(m_uut->code(), DefaultMessageCode);
  EXPECT_THAT(m_uut->payloadSize(), DefaultPayloadSize);
  EXPECT_THAT(m_uut->payload(), DefaultPayload);
}

TEST_F(TestApplicationMessages, ShouldCreateMessageUsingRawVector) {

  vector<uint8_t> message;

  message.push_back(DefaultMessageCode);
  message.push_back(static_cast<uint8_t>(DefaultPayloadSize));
  message.push_back(DefaultPayloadSize >> 8);

  createApplicationMessage(move(message));

  EXPECT_THAT(m_uut->code(), DefaultMessageCode);
  EXPECT_THAT(m_uut->payloadSize(), DefaultPayloadSize);
  EXPECT_THAT(m_uut->payload(), vector<uint8_t>());
}