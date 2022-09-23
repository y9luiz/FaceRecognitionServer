#pragma once

#include "messageReceiverFactory.h"

#include <gmock/gmock-function-mocker.h>
#include <gmock/gmock.h>

class MockMessageReceiverFactory {
public:
  MockMessageReceiverFactory();
  virtual ~MockMessageReceiverFactory();

  MOCK_METHOD(std::unique_ptr<ParallelMessageReceiver>,
              localCreateUdpServerMessageReceiver,
              (const Endpoint &localEndpoint));

  MOCK_METHOD(std::unique_ptr<MessageReceiverInterface>,
              localCreateUdpClientMessageReceiver,
              (std::shared_ptr<UdpSocket> socket));
};
