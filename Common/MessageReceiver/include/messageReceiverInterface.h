#pragma once

#include <applicationMessages.h>

#include <functional>
#include <optional>

class MessageReceiverInterface {
public:
  using Origin = std::vector<uint8_t>;

  virtual ~MessageReceiverInterface() = default;

  virtual std::optional<std::pair<ApplicationMessage, Origin>>
  receiveMessage() = 0;
};