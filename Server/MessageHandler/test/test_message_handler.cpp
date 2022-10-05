#include "messageHandler.h"

#include <applicationMessages.h>
#include <endpoint.h>

#include <gmock/gmock.h>
#include <stdexcept>
#include <vector>

using namespace testing;

using std::invalid_argument;
using std::move;
using std::vector;

namespace {
constexpr auto DefaultMessageCode = 0u;
const Endpoint DefaultEndpoint{"127.0.0.1", 5000};
} // namespace

class TestMessageHandler : public Test {
public:
  TestMessageHandler() {}

  MessageHandler m_uut;

  MockFunction<void(ApplicationMessage &&, const Endpoint &)>
      m_mockProcessMessageCallback;
};

TEST_F(TestMessageHandler, shouldNotRegisterNullCallback) {
  EXPECT_THROW(m_uut.registerCallback(DefaultMessageCode, nullptr),
               invalid_argument);
}

TEST_F(TestMessageHandler, shouldRegisterCallback) {
  m_uut.registerCallback(DefaultMessageCode,
                         m_mockProcessMessageCallback.AsStdFunction());
}

TEST_F(TestMessageHandler, shouldNotProcessEmptyMessage) {
  vector<uint8_t> emptyMessage;
  EXPECT_THROW(m_uut.processMessage(move(emptyMessage), DefaultEndpoint),
               invalid_argument);
}

TEST_F(TestMessageHandler,
       shouldNotProcessMessageWhenNotHaveRegisteredCallback) {
  vector<uint8_t> message{DefaultMessageCode};
  EXPECT_THROW(m_uut.processMessage(move(message), DefaultEndpoint),
               invalid_argument);
}

TEST_F(TestMessageHandler, shouldProcessMessage) {
  m_uut.registerCallback(DefaultMessageCode,
                         m_mockProcessMessageCallback.AsStdFunction());
  ApplicationMessage message{DefaultMessageCode, {}};
  auto messageCopy = message;

  EXPECT_CALL(m_mockProcessMessageCallback,
              Call(move(messageCopy), DefaultEndpoint));

  m_uut.processMessage(move(message), DefaultEndpoint);
}