#pragma once

#include "applicationMessages.h"

#include <gmock/gmock.h>
#include <vector>

class MockApplicationMessage : public ApplicationMessage {
public:
  MockApplicationMessage();
  virtual ~MockApplicationMessage();

  MOCK_METHOD(void, constructor, (uint8_t, std::vector<uint8_t> &&));
  MOCK_METHOD(void, constructor, (std::vector<uint8_t> &&));
  MOCK_METHOD(ApplicationMessage::Code, code, ());
  MOCK_METHOD(std::vector<uint8_t>, serialize, (), (const,override));
};