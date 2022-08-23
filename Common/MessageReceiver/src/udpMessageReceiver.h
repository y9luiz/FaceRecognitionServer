#pragma once

#include <applicationMessages.h>
#include <messageReceiverInterface.h>
#include <udpSocket.h>

class UdpMessageReceiver : public MessageReceiverInterface {
public:
  UdpMessageReceiver(std::unique_ptr<UdpSocket> udpSocket);

protected:
  virtual ApplicationMessage receiveMessage() override;

  ApplicationMessage::Header receiveMessageHeader();

  void receiveMessagePayload(std::vector<uint8_t> &buffer);

  std::unique_ptr<UdpSocket> m_socket;
  Endpoint m_endpoint;
};