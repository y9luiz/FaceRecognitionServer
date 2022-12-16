#pragma once

#include "serializable.h"

#include <cstdint>
#include <vector>

constexpr auto MaximumPacketSize = 1500u;

class ApplicationMessage : public Serializable{
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

    std::vector<uint8_t> toBytes() const;
  };
#pragma pack(pop)

  ApplicationMessage();

  virtual ~ApplicationMessage() = default;

  Header header() const;
  void reserve(uint32_t size);
  const char * payload();
  std::size_t size() const;

  std::vector<uint8_t> serialize() const override;

  bool operator==(const ApplicationMessage &other) const {
    return other.m_header.code == m_header.code &&
           other.m_header.payloadSize == m_header.payloadSize &&
           other.m_payload == m_payload;
  }

protected:
  Header m_header;
  char* m_payload;
};

class FactoryApplicationMessage
{
public:
  static std::unique_ptr<ApplicationMessage> create(std::vector<uint8_t> && byteSequence);
};