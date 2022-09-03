#pragma once

#include "applicationMessages.h"

#include <gmock/gmock.h>
#include <vector>

class MockApplicationMessage {
public:
  MockApplicationMessage();
  virtual ~MockApplicationMessage();

  MOCK_METHOD(void, constructor, (uint8_t, uint16_t, std::vector<uint8_t> &&));
  MOCK_METHOD(void, constructor, (std::vector<uint8_t> &&));
  MOCK_METHOD(ApplicationMessage::Header, header, ());
  MOCK_METHOD(std::vector<uint8_t>&, payload,());
  MOCK_METHOD(std::vector<uint8_t>, convertToBytes,(), (const));
  MOCK_METHOD(std::size_t, size,(), (const));
};