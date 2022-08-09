#pragma once
#include <stdint.h>
#include <vector>

class ApplicationMessage {
public:
#pragma pack(push, 1)
  struct Header {
    uint8_t m_code;
    uint16_t m_payloadSize;

    Header(uint8_t code, uint16_t payloadSize);
    Header(const std::vector<uint8_t> &message);

    std::vector<uint8_t> convertToBytes();
  };
#pragma pack(pop)
  ApplicationMessage(uint8_t code, uint16_t payloadSize,
                     std::vector<uint8_t> &&payload);

  ApplicationMessage(std::vector<uint8_t> &&message);

  virtual ~ApplicationMessage() = default;

  uint8_t code();
  uint16_t payloadSize();
  std::vector<uint8_t> &payload();
  std::vector<uint8_t> convertToBytes();

private:
  Header m_header;
  std::vector<uint8_t> m_payload;
};
