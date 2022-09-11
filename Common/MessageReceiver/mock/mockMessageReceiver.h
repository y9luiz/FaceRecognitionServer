#pragma once

#include "applicationMessages.h"
#include "messageReceiverInterface.h"

#include <gmock/gmock.h>

class MockMessageReceiver : public MessageReceiverInterface {
public:
  MockMessageReceiver();
  ~MockMessageReceiver();

  MOCK_METHOD(void, setReceiveMessageCallback, (ReceiveMessageCallbackT));
  MOCK_METHOD(void, start, ());
  MOCK_METHOD(void, stop, ());
  MOCK_METHOD(bool, isRunning, ());
  MOCK_METHOD(std::optional<ApplicationMessage>, receiveMessage, (),
              (override));
  MOCK_METHOD(void, processMessage, (ApplicationMessage &&));
};