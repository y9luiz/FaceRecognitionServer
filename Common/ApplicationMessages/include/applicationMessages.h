#pragma once

#include <cstdint>
#include <limits>
#include <stdint.h>
#include <vector>

#include <opencv2/core/mat.hpp>

constexpr uint16_t MaximumPacketSize = 1500;

class ApplicationMessage {
public:
  enum class Types : uint8_t {
    FaceDetectionRequest,
    FaceDetectionResponse,
    FaceRecognitionRequest,
    FaceRecognitionResponse,
    InvalidMessage = 255
  };

#pragma pack(push, 1)
  struct Header {
    uint8_t code;
    uint32_t payloadSize;

    Header(uint8_t code, uint32_t payloadSize);
    Header(std::vector<uint8_t> &message);

    std::vector<uint8_t> toBytes() const;
  };
#pragma pack(pop)

  ApplicationMessage(uint8_t code, std::vector<uint8_t> &&payload);
  ApplicationMessage(std::vector<uint8_t> &&message);

  virtual ~ApplicationMessage() = default;

  Header header() const;
  void reserve(uint32_t size);
  std::vector<uint8_t> &payload();
  std::vector<uint8_t> convertToBytes() const;
  std::size_t size() const;

  bool operator==(const ApplicationMessage &other) const {
    return other.m_header.code == m_header.code &&
           other.m_header.payloadSize == m_header.payloadSize &&
           other.m_payload == m_payload;
  }

protected:
  Header m_header;
  std::vector<uint8_t> m_payload;
};
