#include "udpServer.h"
#include "messageReceiverFactory.h"
#include <udpSocket.h>

#include <functional>
#include <iostream>
#include <memory>

using std::bind;
using std::cout;
using std::logic_error;
using std::move;
using std::string;

using namespace boost::asio::ip;

UdpServer::UdpServer(const string &ip, uint16_t port) : m_ip(ip), m_port(port) {
  initializeMessageReceiver();
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
      bind(&UdpServer::handleMessage, this, std::placeholders::_1));
}

void UdpServer::handleMessage(ApplicationMessage &&message) {
  if (!m_messageHandler) {
    throw logic_error("Could not process message, message hander is nullptr!");
  }
  cout << "[INFO::UdpServer]received application message\n";
  m_messageHandler->processMessage(move(message.convertToBytes()));
}

void UdpServer::stop() { m_messageReceiver->stop(); }