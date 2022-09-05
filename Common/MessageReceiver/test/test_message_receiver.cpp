#include "messageReceiverFactory.h"
#include "messageReceiverInterface.h"
#include <applicationMessages.h>

#include <mockUdpSocket.h>

#include <functional>
#include <gmock/gmock.h>
#include <memory>
#include <stdexcept>
#include <thread>

using namespace testing;
using namespace std::chrono_literals;
namespace this_thread = std::this_thread;

using std::function;
using std::invalid_argument;
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
    EXPECT_CALL(m_mockUdpSocket, receiveFrom)
        .WillOnce(DoAll(SetArgReferee<0>(message.convertToBytes()),
                        SetArgReferee<1>(LocalEndpoit), Return(message.size())))
        .WillRepeatedly(Return(0));
  }

  NiceMock<MockUdpSocket> m_mockUdpSocket;
  NiceMock<MockFunction<function<void(ApplicationMessage &&)>>>
      m_mockReceiveMessageCallback;

  unique_ptr<MessageReceiverInterface> m_uut;
};

TEST_F(TestUdpMessageReceiverForServer, shouldNotRegisterNullCallback) {
  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_)).Times(0);

  m_uut = MessageReceiverFactory().createUdpServerMessageReceiver(LocalEndpoit);

  EXPECT_THROW(m_uut->setReceiveMessageCallback(nullptr), invalid_argument);
}

TEST_F(TestUdpMessageReceiverForServer,
       shouldNotProcessWhenNotReceiveAnything) {
  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_)).Times(0);

  m_uut = MessageReceiverFactory().createUdpServerMessageReceiver(LocalEndpoit);
  registerMessageHandlerCallback();
}

TEST_F(TestUdpMessageReceiverForServer,
       shouldNotProcessWhenReceiveIncompleteMessage) {

  ApplicationMessage testMessageWithIncompletePayload{0, 10, {}};

  injectMessageInMockUdpSocket(testMessageWithIncompletePayload);

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_)).Times(0);

  m_uut = MessageReceiverFactory().createUdpServerMessageReceiver(LocalEndpoit);

  registerMessageHandlerCallback();

  this_thread::sleep_for(100ms);
}

TEST_F(TestUdpMessageReceiverForServer,
       shouldNotProcessWhenCallbackNotRegistered) {

  ApplicationMessage completeMessage{0, 5, {'0', '1', '2', '3', '4'}};

  injectMessageInMockUdpSocket(completeMessage);

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_)).Times(0);

  m_uut = MessageReceiverFactory().createUdpServerMessageReceiver(LocalEndpoit);

  this_thread::sleep_for(1ms);

  EXPECT_FALSE(m_uut->isRunning());
}

TEST_F(TestUdpMessageReceiverForServer, processMessage) {

  vector<uint8_t> payload(1024 * 1024, 'o');

  ApplicationMessage completeMessage{0, static_cast<uint16_t>(payload.size()),
                                     move(payload)};

  injectMessageInMockUdpSocket(completeMessage);

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(Eq(completeMessage)));

  m_uut = MessageReceiverFactory().createUdpServerMessageReceiver(LocalEndpoit);
  registerMessageHandlerCallback();

  this_thread::sleep_for(10ms);

  EXPECT_TRUE(m_uut->isRunning());
}

TEST_F(TestUdpMessageReceiverForServer, processMultipleMessages) {

  vector<uint8_t> payload(1024 * 1024, 'o');

  ApplicationMessage completeMessage{0, static_cast<uint16_t>(payload.size()),
                                     move(payload)};

  EXPECT_CALL(m_mockUdpSocket, receiveFrom)
      .WillOnce(DoAll(SetArgReferee<0>(completeMessage.convertToBytes()),
                      SetArgReferee<1>(LocalEndpoit),
                      Return(completeMessage.size())))
      .WillOnce(DoAll(SetArgReferee<0>(completeMessage.convertToBytes()),
                      SetArgReferee<1>(LocalEndpoit),
                      Return(completeMessage.size())))
      .WillOnce(DoAll(SetArgReferee<0>(completeMessage.convertToBytes()),
                      SetArgReferee<1>(LocalEndpoit),
                      Return(completeMessage.size())))
      .WillRepeatedly(Return(0));

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(Eq(completeMessage))).Times(3);

  m_uut = MessageReceiverFactory().createUdpServerMessageReceiver(LocalEndpoit);
  registerMessageHandlerCallback();

  this_thread::sleep_for(10ms);

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

TEST_F(TestUdpMessageReceiverForClient, createClient) {
  m_uut = MessageReceiverFactory().createUdpClientMessageReceiver();
}