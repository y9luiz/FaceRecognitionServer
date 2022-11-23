#pragma once

#include <cstdint>
#include <vector>

class Serializable {
public:
  virtual std::vector<uint8_t> serialize() const = 0;
  virtual void deserialize(std::vector<uint8_t> &) = 0;
};