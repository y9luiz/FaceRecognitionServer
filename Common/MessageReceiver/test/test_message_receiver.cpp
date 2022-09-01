#include "messageReceiverInterface.h"
#include <gmock/gmock-spec-builders.h>
#include <messageReceiverBuilder.h>
#include <applicationMessages.h>

#include <mockUdpSocket.h>

#include <functional>
#include <gmock/gmock.h>
#include <memory>
#include <stdexcept>
#include <thread>

using namespace testing;
using namespace std::chrono_literals;

using std::function;
using std::invalid_argument;
using std::logic_error;
using std::unique_ptr;
using std::vector;

namespace {
const auto LocalEndpoit = Endpoint{"127.0.0.1", 5000};
}

class TestUdpMessageReceiverForServer : public Test {
public:
  TestUdpMessageReceiverForServer() { setupSocketServerExpectations(); }

  void setupSocketServerExpectations() {
    EXPECT_CALL(m_mockUdpSocket, constructor);
    EXPECT_CALL(m_mockUdpSocket, bind);
  }

  void registerMessageHandlerCallback() {
    m_uut->setReceiveMessageCallback(
        m_mockReceiveMessageCallback.AsStdFunction());
  }

  void injectMessageInMockUdpSocket(const ApplicationMessage &message) {
    const auto headerBytes = message.header().convertToBytes();
    
    if (message.size() > headerBytes.size()) {
      EXPECT_CALL(m_mockUdpSocket, receiveFrom)
          .WillOnce(DoAll(SetArgReferee<0>(headerBytes),
                          SetArgReferee<1>(LocalEndpoit), Return(headerBytes.size())))
          .WillOnce(DoAll(SetArgReferee<0>(message.convertToBytes()),
                          SetArgReferee<1>(LocalEndpoit),
                          Return(message.payloadSize())))
          .WillRepeatedly(Return(0));
    } else {
      EXPECT_CALL(m_mockUdpSocket, receiveFrom)
          .WillOnce(DoAll(SetArgReferee<0>(headerBytes),
                          SetArgReferee<1>(LocalEndpoit), Return(headerBytes.size())))
          .WillRepeatedly(DoAll(SetArgReferee<0>(vector<uint8_t>()),
                                SetArgReferee<1>(LocalEndpoit), Return(0)));
    }
  }

  NiceMock<MockUdpSocket> m_mockUdpSocket;
  NiceMock<MockFunction<function<void(ApplicationMessage &&)>>>
      m_mockReceiveMessageCallback;

  unique_ptr<MessageReceiverInterface> m_uut;
};

TEST_F(TestUdpMessageReceiverForServer, shouldNotRegisterNullCallback) {
  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_)).Times(0);

  m_uut = MessageReceiverBuilder().createUdpServerMessageReceiver(LocalEndpoit);

  EXPECT_THROW(m_uut->setReceiveMessageCallback(nullptr), invalid_argument);
}

TEST_F(TestUdpMessageReceiverForServer, shouldNotProcessWhenNotReceiveAnything) {
  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_)).Times(0);

  m_uut = MessageReceiverBuilder().createUdpServerMessageReceiver(LocalEndpoit);
  registerMessageHandlerCallback();
}

TEST_F(TestUdpMessageReceiverForServer,
       shouldNotProcessWhenReceiveIncompleteMessage) {

  ApplicationMessage testMessageWithIncompletePayload{0, 10, {}};

  injectMessageInMockUdpSocket(testMessageWithIncompletePayload);

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_)).Times(0);

  m_uut = MessageReceiverBuilder().createUdpServerMessageReceiver(LocalEndpoit);

  registerMessageHandlerCallback();

  std::this_thread::sleep_for(1ms);
}

TEST_F(TestUdpMessageReceiverForServer,
       shouldNotProcessWhenCallbackNotRegistered) {

  ApplicationMessage completeMessage{0, 5, {'0', '1', '2', '3', '4'}};

  injectMessageInMockUdpSocket(completeMessage);

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_)).Times(0);

  m_uut = MessageReceiverBuilder().createUdpServerMessageReceiver(LocalEndpoit);

  std::this_thread::sleep_for(1ms);

  EXPECT_FALSE(m_uut->isRunning());
}

TEST_F(TestUdpMessageReceiverForServer, processMessage) {

  ApplicationMessage completeMessage{0, 5, {'0', '1', '2', '3', '4'}};

  injectMessageInMockUdpSocket(completeMessage);

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_));

  m_uut = MessageReceiverBuilder().createUdpServerMessageReceiver(LocalEndpoit);
  registerMessageHandlerCallback();

  std::this_thread::sleep_for(1ms);

  EXPECT_TRUE(m_uut->isRunning());
}

class TestUdpMessageReceiverForClient : public Test {
public:
  TestUdpMessageReceiverForClient() { setupSocketClientExpectations(); }

  void setupSocketClientExpectations() {
    EXPECT_CALL(m_mockUdpSocket, constructor);
    EXPECT_CALL(m_mockUdpSocket, open);
  }

  NiceMock<MockUdpSocket> m_mockUdpSocket;

  unique_ptr<MessageReceiverInterface> m_uut;
};

TEST_F(TestUdpMessageReceiverForClient, createClient)
{
  m_uut = MessageReceiverBuilder().createUdpClientMessageReceiver();
}