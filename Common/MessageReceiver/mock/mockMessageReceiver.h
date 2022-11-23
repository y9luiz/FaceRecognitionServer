#pragma once

#include "messageReceiverInterface.h"
#include <applicationMessages.h>

#include <gmock/gmock.h>

class MockMessageReceiver : public MessageReceiverInterface {
public:
  MockMessageReceiver();
  ~MockMessageReceiver();

  MOCK_METHOD((std::optional<std::pair<ApplicationMessage, Origin>>),
              receiveMessage, (), (override));
};