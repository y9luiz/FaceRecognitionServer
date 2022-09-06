#pragma once

#include <applicationMessages.h>
#include <messageReceiverInterface.h>
#include <udpSocket.h>

class UdpMessageReceiver : public MessageReceiverInterface {
public:
  UdpMessageReceiver(std::unique_ptr<UdpSocket> udpSocket);
  virtual ~UdpMessageReceiver() override;

protected:
  virtual void start() override;

  virtual void stop() override;

  virtual std::optional<ApplicationMessage> receiveMessage() override;

  void receiveMessagePayload(std::vector<uint8_t> &buffer);

  std::unique_ptr<UdpSocket> m_socket;
};