#include <gmock/gmock-spec-builders.h>
#include <messageHandler.h>

#include <gmock/gmock.h>
#include <stdexcept>
#include <vector>

using namespace testing;

using std::invalid_argument;
using std::move;
using std::vector;

namespace {
constexpr auto DefaultMessageCode = 0u;
}

class TestMessageHandler : public Test {
public:
  TestMessageHandler() {}

  MessageHandler m_uut;

  MockFunction<void(vector<uint8_t> &&)> m_mockProcessMessageCallback;
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
  EXPECT_THROW(m_uut.processMessage(move(emptyMessage)), invalid_argument);
}

TEST_F(TestMessageHandler,
       shouldNotProcessMessageWhenNotHaveRegisteredCallback) {
  vector<uint8_t> message{DefaultMessageCode};
  EXPECT_THROW(m_uut.processMessage(move(message)), invalid_argument);
}

TEST_F(TestMessageHandler, shouldProcessMessage) {
  m_uut.registerCallback(DefaultMessageCode,
                         m_mockProcessMessageCallback.AsStdFunction());

  EXPECT_CALL(m_mockProcessMessageCallback, Call(_));

  vector<uint8_t> message{DefaultMessageCode};
  m_uut.processMessage(move(message));
}