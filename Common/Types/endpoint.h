#pragma once

#include <string>

struct Endpoint {
  std::string address;
  uint16_t port;

  uint32_t size() { return sizeof(uint16_t) + address.length(); }
  bool operator==(const Endpoint &other) const {
    return address == other.address && port == other.port;
  }
};
