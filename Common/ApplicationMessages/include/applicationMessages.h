#pragma once

#include "serializable.h"

#include <cstdint>
#include <vector>

constexpr auto MaximumPacketSize = 1500u;

class ApplicationMessage : public Serializable{
public:
  enum class Code : uint8_t {
    FaceDetectionRequest = 42,
    FaceDetectionResponse,
    FaceRecognitionRequest,
    FaceRecognitionResponse,
    InvalidMessage = 255
  };

  ApplicationMessage() = delete;
  ApplicationMessage(ApplicationMessage &) = delete;

  virtual ~ApplicationMessage() = default;

  Code code() const;

  virtual std::vector<uint8_t> serialize() const = 0;

protected:

  ApplicationMessage(Code code);

  Code m_code;
};

class FactoryApplicationMessage
{
public:
  static std::unique_ptr<ApplicationMessage> create(std::vector<uint8_t> && byteSequence);
};