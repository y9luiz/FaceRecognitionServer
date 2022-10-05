#pragma once

#include "messageReceiverInterface.h"

#include <udpSocket.h>

class UdpMessageReceiver : public MessageReceiverInterface {
public:
  UdpMessageReceiver();
  UdpMessageReceiver(std::shared_ptr<UdpSocket> socket);

  virtual std::optional<std::pair<ApplicationMessage, Origin>> receiveMessage();

private:
  std::shared_ptr<UdpSocket> m_socket;
};