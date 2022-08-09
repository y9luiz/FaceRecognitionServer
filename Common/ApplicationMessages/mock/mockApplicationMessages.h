#pragma once

#include "applicationMessages.h"

#include <vector>

#include <gmock/gmock.h>

class MockApplicationMessage {
public:
  MockApplicationMessage();
  virtual ~MockApplicationMessage();

  MOCK_METHOD(void, constructor, (uint8_t, uint16_t, std::vector<uint8_t> &&));
  MOCK_METHOD(void, constructor, (std::vector<uint8_t> &&));
  MOCK_METHOD(uint8_t, code, ());
  MOCK_METHOD(uint16_t, payloadSize, ());
  MOCK_METHOD(std::vector<uint8_t> &, payload, ());
  MOCK_METHOD(std::vector<uint8_t>, convertToBytes, ());
};