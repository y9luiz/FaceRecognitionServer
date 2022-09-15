#pragma once

#include "applicationMessages.h"
#include "udpSocket.h"

#include <queue>

class MessageSenderInterface {
public:
  struct Destination {
    virtual ~Destination() = default;
  };

  virtual ~MessageSenderInterface() = default;

  virtual void sendMessage(ApplicationMessage &&applicationMessage,
                           const Destination &destination) = 0;

protected:
  MessageSenderInterface() = default;
};