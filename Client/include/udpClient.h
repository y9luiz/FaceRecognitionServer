#pragma once

#include <applicationMessages.h>
#include <messageReceiverInterface.h>
#include <udpMessageSender.h>

#include <memory>
#include <string>

class UdpClient {
public:
  UdpClient(const std::string &url, uint16_t port);

  void sendMessage(ApplicationMessage &&message);
  ApplicationMessage receiveMessage();

private:
  Endpoint m_destinationEndpoint;
  std::unique_ptr<MessageReceiverInterface> m_udpMessageReceiver;
  std::unique_ptr<UdpMessageSender> m_udpMessageSender;
};