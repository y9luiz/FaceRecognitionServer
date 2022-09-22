#include "messageReceiverFactory.h"
#include "parallelMessageReceiver.h"
#include "udpSocket.h"
#include <applicationMessages.h>

#include <gmock/gmock-spec-builders.h>
#include <gtest/gtest.h>
#include <mockUdpSocket.h>

#include <functional>
#include <gmock/gmock.h>
#include <memory>
#include <optional>
#include <stdexcept>
#include <thread>

using namespace testing;
using namespace std::chrono_literals;
namespace this_thread = std::this_thread;

using std::invalid_argument;
using std::logic_error;
using std::make_shared;
using std::optional;
using std::pair;
using std::unique_ptr;
using std::vector;

namespace {
const auto LocalEndpoit = Endpoint{"127.0.0.1", 5000};

vector<vector<uint8_t>>
splitBigApplicationMessage(const ApplicationMessage &message) {
  const auto numberOfPackets = message.size() / MaximumPacketSize + 1;
  auto bytes = message.convertToBytes();
  vector<vector<uint8_t>> splittedMessaged;
  for (int i = 0; i < numberOfPackets; i++) {
    splittedMessaged.emplace_back();
    bool isLastPacket = i == numberOfPackets - 1;

    if (!isLastPacket) {
      copy(bytes.begin() + i * MaximumPacketSize,
           bytes.begin() + (i + 1) * MaximumPacketSize,
           back_inserter(splittedMessaged.back()));
    } else {
      copy(bytes.begin() + i * MaximumPacketSize, bytes.end(),
           back_inserter(splittedMessaged.back()));
    }
  }
  return splittedMessaged;
}
} // namespace

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

  void injectMessageInMockUdpSocket(const ApplicationMessage &message,
                                    bool isIncompleteMessage = false) {
    const auto splittedBytes = splitBigApplicationMessage(message);
    {
      InSequence s;
      bool isFirstPacket = true;
      for (const auto &packet : splittedBytes) {
        if (isFirstPacket) {
          isFirstPacket = false;
          EXPECT_CALL(m_mockUdpSocket, receiveFrom)
              .WillOnce(DoAll(SetArgReferee<0>(packet), Return(packet.size())));
        } else {
          EXPECT_CALL(m_mockUdpSocket, receive)
              .WillOnce(DoAll(SetArgReferee<0>(packet), Return(packet.size())));
        }
      }
      if (isIncompleteMessage) {
        EXPECT_CALL(m_mockUdpSocket, receive).WillOnce(Return(0));
      }

      EXPECT_CALL(m_mockUdpSocket, receiveFrom).WillRepeatedly(Return(0));
    }
  }

  void
  injectMessagesInMockUdpSocket(const vector<ApplicationMessage> &messages) {
    {
      InSequence s;
      for (const auto &message : messages) {
        const auto splittedBytes = splitBigApplicationMessage(message);
        bool isFirstPacket = true;
        for (const auto &bytes : splittedBytes) {
          if (isFirstPacket) {
            isFirstPacket = false;
            EXPECT_CALL(m_mockUdpSocket, receiveFrom)
                .WillOnce(DoAll(SetArgReferee<0>(bytes), Return(bytes.size())));
          } else {
            EXPECT_CALL(m_mockUdpSocket, receive)
                .WillOnce(DoAll(SetArgReferee<0>(bytes), Return(bytes.size())));
          }
        }
      }

      EXPECT_CALL(m_mockUdpSocket, receiveFrom).WillRepeatedly(Return(0));
    }
  }

  NiceMock<MockUdpSocket> m_mockUdpSocket;
  NiceMock<MockFunction<void(ApplicationMessage &&, const vector<uint8_t> &)>>
      m_mockReceiveMessageCallback;

  unique_ptr<ParallelMessageReceiver> m_uut;
};

TEST_F(TestUdpMessageReceiverForServer, shouldNotRegisterNullCallback) {
  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_, _)).Times(0);

  m_uut = MessageReceiverFactory::createUdpServerMessageReceiver(LocalEndpoit);

  EXPECT_THROW(m_uut->setReceiveMessageCallback(nullptr), invalid_argument);
  this_thread::sleep_for(2s);
}

TEST_F(TestUdpMessageReceiverForServer,
       shouldNotProcessWhenNotReceiveAnything) {
  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_, _)).Times(0);

  m_uut = MessageReceiverFactory::createUdpServerMessageReceiver(LocalEndpoit);
  registerMessageHandlerCallback();

  this_thread::sleep_for(2s);
}

TEST_F(TestUdpMessageReceiverForServer,
       shouldNotProcessWhenCallbackNotRegistered) {
  ApplicationMessage completeMessage{0, {'0', '1', '2', '3', '4'}};

  injectMessageInMockUdpSocket(completeMessage);

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_, _)).Times(0);

  m_uut = MessageReceiverFactory::createUdpServerMessageReceiver(LocalEndpoit);

  this_thread::sleep_for(2s);

  EXPECT_FALSE(m_uut->isRunning());
}

TEST_F(TestUdpMessageReceiverForServer, processMessage) {
  vector<uint8_t> payload(MaximumPacketSize * 3, 'o');

  ApplicationMessage completeMessage{0, move(payload)};

  injectMessageInMockUdpSocket(completeMessage);

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(Eq(completeMessage), _));

  m_uut = MessageReceiverFactory::createUdpServerMessageReceiver(LocalEndpoit);

  registerMessageHandlerCallback();

  this_thread::sleep_for(2s);

  EXPECT_TRUE(m_uut->isRunning());
}

TEST_F(TestUdpMessageReceiverForServer, processMultipleMessages) {
  vector<uint8_t> payload(MaximumPacketSize * 3, 'o');

  ApplicationMessage completeMessage{0, move(payload)};

  vector<ApplicationMessage> messages{completeMessage, completeMessage,
                                      completeMessage};

  injectMessagesInMockUdpSocket(messages);

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(Eq(completeMessage), _))
      .Times(3);

  m_uut = MessageReceiverFactory::createUdpServerMessageReceiver(LocalEndpoit);
  registerMessageHandlerCallback();

  EXPECT_TRUE(m_uut->isRunning());

  this_thread::sleep_for(2s);
}

TEST_F(TestUdpMessageReceiverForServer, shouldNotProcessIncompleteMessage) {
  vector<uint8_t> payload(MaximumPacketSize, 'o');
  ApplicationMessage incompleteMessage{0, move(payload)};
  incompleteMessage.reserve(MaximumPacketSize + 10);
  injectMessageInMockUdpSocket(incompleteMessage, true);

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_, _)).Times(0);

  m_uut = MessageReceiverFactory::createUdpServerMessageReceiver(LocalEndpoit);
  registerMessageHandlerCallback();

  EXPECT_TRUE(m_uut->isRunning());

  this_thread::sleep_for(2s);
}

class TestUdpMessageReceiverForClient : public Test {
public:
  TestUdpMessageReceiverForClient() { setupSocketClientExpectations(); }

  void setupSocketClientExpectations() {
    EXPECT_CALL(m_mockUdpSocket, constructor);
  }

  NiceMock<MockUdpSocket> m_mockUdpSocket;

  unique_ptr<MessageReceiverInterface> m_uut;
};

TEST_F(TestUdpMessageReceiverForClient, notCreateIfSocketIsClosed) {
  EXPECT_CALL(m_mockUdpSocket, isOpen).WillOnce(Return(false));

  EXPECT_THROW(
      [this]() {
        auto socket = make_shared<UdpSocket>();
        m_uut = MessageReceiverFactory::createUdpClientMessageReceiver(socket);
      }(),
      logic_error);
}

TEST_F(TestUdpMessageReceiverForClient, createClient) {
  EXPECT_CALL(m_mockUdpSocket, isOpen).WillOnce(Return(true));

  auto socket = make_shared<UdpSocket>();
  m_uut = MessageReceiverFactory::createUdpClientMessageReceiver(socket);
}
