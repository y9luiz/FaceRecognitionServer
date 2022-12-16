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

  auto header = message->header();
  const auto payload = message->payload();

  vector<uint8_t> buffer = header.toBytes();
  buffer.reserve(MaximumPacketSize);

  if (message->size() < MaximumPacketSize - buffer.size()) {
    copy(payload, payload + header.payloadSize, back_inserter(buffer));
    socket.sendTo(buffer, destination);
    return;
  }

  auto startAddr = payload;
  auto endAddr = payload + (MaximumPacketSize - buffer.size());

  uint32_t numberOfBytesSended = 0;

  copy(startAddr, endAddr, back_inserter(buffer));

  while (numberOfBytesSended < message->size()) {
    numberOfBytesSended += socket.sendTo(buffer, destination);
    startAddr = endAddr;
    auto reminderBytesAmount = message->size() - numberOfBytesSended;

    if (reminderBytesAmount > MaximumPacketSize) {
      endAddr += MaximumPacketSize;
    } else if (reminderBytesAmount > 0) {
      endAddr += reminderBytesAmount;
      buffer.resize(reminderBytesAmount);
    }

    copy(startAddr, endAddr, back_inserter(buffer));
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