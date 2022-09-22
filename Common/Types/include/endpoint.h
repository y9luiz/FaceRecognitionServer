#pragma once

#include <string>
#include <vector>

struct Endpoint {
  std::string address;
  uint16_t port;

  uint32_t size() const;

  bool operator==(const Endpoint &other) const {
    return address == other.address && port == other.port;
  }

  std::vector<uint8_t> toBytes() const;
  static Endpoint fromBytes(std::vector<uint8_t> &&input);
};
