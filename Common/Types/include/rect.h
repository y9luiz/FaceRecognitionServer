#pragma once

#include "serializable.h"

#include <opencv2/core/types.hpp>

class Rect : public Serializable {
public:
  Rect();
  Rect(cv::Rect rect);
  virtual ~Rect() = default;

  cv::Rect2i get() const;

  std::vector<uint8_t> serialize() const override;
  void deserialize(std::vector<uint8_t> &) override;

  bool operator==(const Rect &other) const {
    return other.get() == m_rect;
  }

private:
  cv::Rect2i m_rect;
};