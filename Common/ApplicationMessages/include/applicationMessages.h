#pragma once

#include <stdint.h>
#include <vector>

constexpr uint16_t MaximumPacketSize = 1500;

class ApplicationMessage {
public:
#pragma pack(push, 1)
  struct Header {
    uint8_t code;
    uint16_t payloadSize;

    Header(uint8_t code, uint16_t payloadSize);
    Header(const std::vector<uint8_t> &message);

    std::vector<uint8_t> convertToBytes() const;
  };
#pragma pack(pop)
  ApplicationMessage(uint8_t code, uint16_t payloadSize,
                     std::vector<uint8_t> &&payload);

  ApplicationMessage(std::vector<uint8_t> &&message);

  virtual ~ApplicationMessage() = default;

  Header header() const;
  std::vector<uint8_t> &payload();
  std::vector<uint8_t> convertToBytes() const;
  std::size_t size() const;

  bool operator==(const ApplicationMessage & other) const;

private:
  Header m_header;
  std::vector<uint8_t> m_payload;
};
