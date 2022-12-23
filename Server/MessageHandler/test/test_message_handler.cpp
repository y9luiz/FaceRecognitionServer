#include "messageHandler.h"

#include <endpoint.h>

#include <mockApplicationMessages.h>

#include <gmock/gmock.h>
#include <stdexcept>
#include <vector>

using namespace testing;

using std::invalid_argument;
using std::unique_ptr;

namespace {
constexpr auto DefaultMessageCode = 0u;
const Endpoint DefaultEndpoint{"127.0.0.1", 5000};
} // namespace

class TestMessageHandler : public Test {
public:
  TestMessageHandler() {}

  unique_ptr<MockApplicationMessage> m_mockApplicationMessage;
  MessageHandler m_uut;

  MockFunction<void(unique_ptr<ApplicationMessage>, const Endpoint &)>
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

TEST_F(TestMessageHandler, shouldNotProcessNullMessage) {
  EXPECT_THROW(m_uut.processMessage(nullptr, DefaultEndpoint),
               invalid_argument);
}

TEST_F(TestMessageHandler,
       shouldNotProcessMessageWhenNotHaveRegisteredCallback) {
  unique_ptr<ApplicationMessage> msg = std::make_unique<MockApplicationMessage>();
  EXPECT_THROW(m_uut.processMessage(move(m_mockApplicationMessage), DefaultEndpoint),
               invalid_argument);
}

TEST_F(TestMessageHandler, shouldProcessMessage) {
  unique_ptr<ApplicationMessage> msg = std::make_unique<MockApplicationMessage>();

  m_uut.registerCallback(static_cast<uint8_t>(msg->code()),
                         m_mockProcessMessageCallback.AsStdFunction());

  EXPECT_CALL(m_mockProcessMessageCallback,
              Call(_, DefaultEndpoint));

  m_uut.processMessage(move(msg), DefaultEndpoint);
}