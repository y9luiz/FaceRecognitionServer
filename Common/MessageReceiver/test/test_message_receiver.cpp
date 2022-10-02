#include "messageReceiverFactory.h"
#include "parallelMessageReceiver.h"

#include <mockApplicationMessages.h>
#include <mockUdpSocket.h>

#include <functional>
#include <gmock/gmock.h>
#include <memory>
#include <optional>
#include <stdexcept>
#include <thread>
#include <iterator>
#include <queue>

using namespace testing;
using namespace std::chrono_literals;
namespace this_thread = std::this_thread;

using std::invalid_argument;
using std::unique_ptr;
using std::vector;
using std::array;
using std::queue;
using std::logic_error;
using std::make_shared;

namespace {
const auto LocalEndpoit = Endpoint{"127.0.0.1", 5000};

vector<uint8_t> createBigMessage()
{
  uint8_t code = 0;

  uint32_t payloadSize = MaximumPacketSize*2;
  array<uint8_t,4> payloadSizeBytes;
  payloadSizeBytes[0] = payloadSize;
  payloadSizeBytes[1] = payloadSize>>8;
  payloadSizeBytes[2] = payloadSize>>16;
  payloadSizeBytes[3] = payloadSize>>24;

  vector<uint8_t> messageBytes;

  messageBytes.emplace_back(code);

  for(const auto & byte:payloadSizeBytes)
  {
    messageBytes.emplace_back(byte);
  }

  vector<uint8_t> payload(payloadSize,'q');

  move(payload.begin(),payload.end(),back_inserter(messageBytes));

  return messageBytes;
}

queue<vector<uint8_t>> splitBigMessageInUdpPackets(const vector<uint8_t> & message)
{
  const auto numberOfPackets = (message.size() % MaximumPacketSize) > 0 ?
      message.size()/MaximumPacketSize + 1 :
         message.size()/MaximumPacketSize;

  queue<vector<uint8_t>> splittedMessages;
  for(int i = 0; i < numberOfPackets; i++)
  {
    auto remeinderBytes = message.size() - i*MaximumPacketSize;

    splittedMessages.push({});

    auto startIt = message.begin() + i*MaximumPacketSize;
    auto packetSize = remeinderBytes >= MaximumPacketSize? MaximumPacketSize: remeinderBytes;
    auto endIt = startIt + packetSize;
    copy(startIt, endIt,back_inserter(splittedMessages.back()));
  }

  return splittedMessages;
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

  void injectPacketsInSocket(std::queue<vector<uint8_t>> &packets)
  {
    auto firstPacket = packets.front();
    packets.pop();
    EXPECT_CALL(m_mockUdpSocket, receiveFrom).WillOnce(DoAll(SetArgReferee<0>(firstPacket),
                          SetArgReferee<1>(LocalEndpoit),
                          Return(firstPacket.size()))).WillRepeatedly(Return(0));

    auto injectPacketInSocket = [&packets](vector<uint8_t>& packet){
      if(!packets.empty())
      {
        packet = packets.front();
        packets.pop();

        return packet.size();
      }

      return size_t(0);
    };

    EXPECT_CALL(m_mockUdpSocket, receive(_)).WillRepeatedly(Invoke(injectPacketInSocket));
  }

  NiceMock<MockUdpSocket> m_mockUdpSocket;
  NiceMock<MockFunction<void(ApplicationMessage &&, const vector<uint8_t> &)>>
      m_mockReceiveMessageCallback;
  NiceMock<MockApplicationMessage> m_mockApplicationMessage;

  unique_ptr<ParallelMessageReceiver> m_uut;
};

TEST_F(TestUdpMessageReceiverForServer, shouldNotRegisterNullCallback) {
  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_, _)).Times(0);

  m_uut = MessageReceiverFactory::createUdpServerMessageReceiver(LocalEndpoit);

  EXPECT_THROW(m_uut->setReceiveMessageCallback(nullptr), invalid_argument);
  this_thread::sleep_for(2s);
}

TEST_F(TestUdpMessageReceiverForServer, shouldNotProcessWhenNotReceiveAnything) {
  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_, _)).Times(0);

  m_uut = MessageReceiverFactory::createUdpServerMessageReceiver(LocalEndpoit);
  registerMessageHandlerCallback();

  this_thread::sleep_for(2s);
}

TEST_F(TestUdpMessageReceiverForServer, shouldNotProcessWhenCallbackNotRegistered) {
  EXPECT_CALL(m_mockUdpSocket, receiveFrom)
      .WillOnce(DoAll(SetArgReferee<0>(m_mockApplicationMessage.convertToBytes()),
                      SetArgReferee<1>(LocalEndpoit),
                      Return(m_mockApplicationMessage.size())));

  EXPECT_CALL(m_mockUdpSocket, receive).Times(0);

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_, _)).Times(0);

  m_uut = MessageReceiverFactory::createUdpServerMessageReceiver(LocalEndpoit);

  this_thread::sleep_for(2s);

  EXPECT_FALSE(m_uut->isRunning());
}

TEST_F(TestUdpMessageReceiverForServer, processMessage) {
  vector<uint8_t> bytes = m_mockApplicationMessage.convertToBytes();

  EXPECT_CALL(m_mockUdpSocket, receiveFrom)
      .WillOnce(DoAll(SetArgReferee<0>(bytes),
                      SetArgReferee<1>(LocalEndpoit),
                      Return(m_mockApplicationMessage.size())))
      .WillRepeatedly(Return(0));

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_, _));

  m_uut = MessageReceiverFactory::createUdpServerMessageReceiver(LocalEndpoit);

  registerMessageHandlerCallback();

  this_thread::sleep_for(2s);

  EXPECT_TRUE(m_uut->isRunning());
}


TEST_F(TestUdpMessageReceiverForServer, processMessageBigMessage) {
  auto packets = splitBigMessageInUdpPackets(createBigMessage());

  injectPacketsInSocket(packets);

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_, _));

  m_uut = MessageReceiverFactory::createUdpServerMessageReceiver(LocalEndpoit);

  registerMessageHandlerCallback();

  this_thread::sleep_for(2s);

  EXPECT_TRUE(m_uut->isRunning());
}

TEST_F(TestUdpMessageReceiverForServer, shouldNotProcessIncompleteMessage) {
  queue<vector<uint8_t>> packets;
  auto bytes = m_mockApplicationMessage.convertToBytes();
  bytes.pop_back();
  packets.push(bytes);
  injectPacketsInSocket(packets);

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_, _)).Times(0);

  m_uut = MessageReceiverFactory::createUdpServerMessageReceiver(LocalEndpoit);
  registerMessageHandlerCallback();

  EXPECT_TRUE(m_uut->isRunning());

  this_thread::sleep_for(2s);
}

TEST_F(TestUdpMessageReceiverForServer, shouldNotProcessIncompleteBigMessage) {
  auto packets = splitBigMessageInUdpPackets(createBigMessage());
  packets.pop();
  injectPacketsInSocket(packets);

  EXPECT_CALL(m_mockReceiveMessageCallback, Call(_, _)).Times(0);

  m_uut = MessageReceiverFactory::createUdpServerMessageReceiver(LocalEndpoit);

  registerMessageHandlerCallback();

  this_thread::sleep_for(2s);

  EXPECT_TRUE(m_uut->isRunning());
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
