#pragma once

#include "serializable.h"

#include <cstdint>
#include <vector>

constexpr auto MaximumPacketSize = 1500u;

class ApplicationMessage : public Serializable{
public:
  // The header is a set of 5 bytes where
  // byte 0 -> Code
  // byte 1 to 4 -> payload size
  using Header = std::array<uint8_t, 5>;

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

  template <typename T, typename... Args>
  std::enable_if_t<std::is_constructible<T, Args&&...>::value, std::unique_ptr<T>>
  static create(Args&&... args)
  {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }
};