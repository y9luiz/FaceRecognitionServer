#pragma once

#include <udpMessageSender.h>

#include <gmock/gmock.h>

class MockUdpMessageSender {
public:
  MockUdpMessageSender();
  virtual ~MockUdpMessageSender();

  MOCK_METHOD(void, constructor, ());
  MOCK_METHOD(void, sendMessage, (std::unique_ptr<ApplicationMessage>, const Endpoint &));
};