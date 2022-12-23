#pragma once

#include "messageReceiverInterface.h"
#include <applicationMessages.h>

#include <gmock/gmock.h>

class MockMessageReceiver : public MessageReceiverInterface {
public:
  MockMessageReceiver();
  ~MockMessageReceiver();

  MOCK_METHOD((std::optional<std::pair<std::unique_ptr<ApplicationMessage>, Origin>>),
              receiveMessage, (), (override));
};