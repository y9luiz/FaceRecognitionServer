#pragma once

#include <memory>
#include <vector>

class Serializable {
public:
  // Converts the object into a byte sequence
  virtual std::vector<uint8_t> serialize() const = 0;
};