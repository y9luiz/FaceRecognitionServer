#pragma once

#include "serializable.h"

#include <string>
#include <vector>

class Endpoint : public Serializable {
public:
  Endpoint();
  Endpoint(std::string address, uint16_t port);
  uint32_t size() const;

  bool operator==(const Endpoint &other) const {
    return address == other.address && port == other.port;
  }

  std::vector<uint8_t> serialize() const override;
  static Endpoint fromBytes(std::vector<uint8_t> &&input);

  std::string address;
  uint16_t port;
};
