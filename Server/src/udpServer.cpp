#include "udpServer.h"
#include "iMessageReceiver.h"
#include "messageReceiverBuilder.h"
#include <udpSocket.h>

#include <functional>
#include <memory>

using std::bind;
using std::logic_error;
using std::make_unique;
using std::move;
using std::string;

using namespace boost::asio::ip;

UdpServer::UdpServer(const string &ip, uint16_t port) : m_ip(ip), m_port(port) {
  initializeMessageReceiver();
}

UdpServer::~UdpServer() {}

void UdpServer::initializeMessageReceiver() {

  MessageReceiverBuilder messageReceiverBuilder;
  m_messageReceiver =
      messageReceiverBuilder.createUdpServerMessageReceiver({m_ip, m_port});

  registerMessageReceiverCallback();
}

void UdpServer::registerMessageReceiverCallback() {
  m_messageReceiver->setReceiveMessageCallback(
      bind(&UdpServer::handleMessage, this, std::placeholders::_1));
}

void UdpServer::handleMessage(ApplicationMessage &&message) {
  if (!m_messageHandler) {
    throw logic_error("Could not process message, message hander is nullptr!");
  }
  m_messageHandler->processMessage(move(message.convertToBytes()));
}

void UdpServer::start() { m_messageReceiver->start(); }

void UdpServer::stop() { m_messageReceiver = nullptr; }
