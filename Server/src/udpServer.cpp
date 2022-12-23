#include "udpServer.h"
#include "endpoint.h"
#include "messageReceiverFactory.h"
#include "messageReceiverInterface.h"
#include "serializer.h"
#include "udpMessageSender.h"
#include <udpSocket.h>

#include <functional>
#include <iostream>
#include <memory>

using std::cout;
using std::logic_error;
using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;

using namespace boost::asio::ip;

UdpServer::UdpServer(const string &ip, uint16_t port) : m_ip(ip), m_port(port) {
  initializeMessageReceiver();
  m_messageSender = make_unique<UdpMessageSender>();
}

UdpServer::~UdpServer() { stop(); }

void UdpServer::initializeMessageReceiver() {

  MessageReceiverFactory messageReceiverFactory;
  m_messageReceiver =
      messageReceiverFactory.createUdpServerMessageReceiver({m_ip, m_port});

  registerMessageReceiverCallback();
}

void UdpServer::registerMessageReceiverCallback() {

  m_messageReceiver->setReceiveMessageCallback(
      [this](auto message,
             const MessageReceiverInterface::Origin &origin) {
        auto originCopy = origin;
        auto endpoint = Endpoint::fromBytes(move(originCopy));
        handleMessage(move(message), endpoint);
      });
}

void UdpServer::handleMessage(unique_ptr<ApplicationMessage> message,
                              const Endpoint &endpoint) {
  if (!m_messageHandler) {
    throw logic_error("Could not process message, message hander is nullptr!");
  }
  cout << "[INFO::UdpServer]received application message\n";

  m_messageHandler->processMessage(move(message), endpoint);
}

void UdpServer::stop() { m_messageReceiver->stop(); }