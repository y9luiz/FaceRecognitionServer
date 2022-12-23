#include "udpMessageSender.h"
#include "applicationMessages.h"
#include "serializer.h"
#include "udpSocket.h"

#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

namespace this_thread = std::this_thread;

using std::invalid_argument;
using std::lock_guard;
using std::make_pair;
using std::make_shared;
using std::make_unique;
using std::move;
using std::pair;
using std::queue;
using std::string;
using std::thread;
using std::unique_ptr;
using std::vector;

UdpMessageSender::UdpMessageSender() : m_isRunning{true} {
  m_socket = make_shared<UdpSocket>();
  m_socket->open();
  m_sendMessageThread =
      make_unique<thread>(&UdpMessageSender::sendMessageWorker, this);
}

UdpMessageSender::~UdpMessageSender() {
  m_isRunning = false;
  m_messageQueueCondVar.notify_all();
  if (m_sendMessageThread) {
    if (m_sendMessageThread->joinable()) {
      m_sendMessageThread->join();
    }
  }
}

void UdpMessageSender::sendMessage(
    unique_ptr<ApplicationMessage> applicationMessage,
    const Endpoint &destination) {

  if(!applicationMessage)
  {
    throw invalid_argument("Could not send null application message");
  }

  auto endpoint = destination;
  {
    lock_guard _(m_messageQueueMutex);
    m_messageQueue.push(make_pair(move(applicationMessage), move(endpoint)));
  }
  m_messageQueueCondVar.notify_one();
}

void sendApplicationMessage(UdpSocket &socket,
                            unique_ptr<ApplicationMessage> message,
                            const Endpoint &destination) {
  if(!message)
  {
    throw invalid_argument("Could not send null application message");
  }

  const auto bytes = message->serialize();

  vector<uint8_t> buffer;
  buffer.reserve(MaximumPacketSize);

  if (bytes.size() <= MaximumPacketSize) {
    copy(bytes.begin(), bytes.end(), back_inserter(buffer));
    socket.sendTo(buffer, destination);
    return;
  }

  auto startIt = bytes.begin();
  auto endIt = startIt + (MaximumPacketSize);

  uint32_t numberOfBytesSended = 0;

  copy(startIt, endIt, back_inserter(buffer));

  while (numberOfBytesSended < bytes.size()) {
    numberOfBytesSended += socket.sendTo(buffer, destination);
    startIt = endIt;
    const auto reminderBytesAmount = bytes.size() - numberOfBytesSended;

    if (reminderBytesAmount > MaximumPacketSize) {
      endIt += MaximumPacketSize;
    } else if (reminderBytesAmount > 0) {
      endIt += reminderBytesAmount;
      buffer.resize(reminderBytesAmount);
    }

    copy(startIt, endIt, buffer.begin());
    this_thread::sleep_for(10us);
  }
}

void UdpMessageSender::sendMessageWorker() {
  queue<pair<unique_ptr<ApplicationMessage>, Endpoint>> temporaryMessageQueue;

  while (m_isRunning) {
    {
      std::unique_lock lock(m_messageQueueMutex);
      m_messageQueueCondVar.wait(
          lock, [this]() { return !m_isRunning || !m_messageQueue.empty(); });
      m_messageQueue.swap(temporaryMessageQueue);
    }

    while (!temporaryMessageQueue.empty()) {
      auto [message, destination] = move(temporaryMessageQueue.front());

      sendApplicationMessage(*m_socket, move(message), destination);

      temporaryMessageQueue.pop();
    }
  }
}