#pragma once
#include <stdint.h>
#include <vector>

class ApplicationMessage {
public:
  ApplicationMessage(uint8_t code, uint16_t payloadSize,
                     std::vector<uint8_t> &&payload);

  ApplicationMessage(std::vector<uint8_t> &&message);

  uint8_t code();
  uint16_t payloadSize();
  std::vector<uint8_t> &payload();

private:
  uint8_t m_code;
  uint16_t m_payloadSize;
  std::vector<uint8_t> m_payload;
};