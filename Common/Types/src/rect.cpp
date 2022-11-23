#include "rect.h"

#include <serializer.h>

using cv::Rect2i;
using std::vector;

Rect::Rect(){}

Rect::Rect(cv::Rect rect) : m_rect(rect) {}

Rect2i Rect::get() const{ return m_rect; }

vector<uint8_t> Rect::serialize() const {
  vector<uint8_t> bytes;
  bytes.reserve(sizeof(Rect2i));

  Serializer::append(bytes, Serializer::u32ToBytes(m_rect.x));
  Serializer::append(bytes, Serializer::u32ToBytes(m_rect.y));
  Serializer::append(bytes, Serializer::u32ToBytes(m_rect.height));
  Serializer::append(bytes, Serializer::u32ToBytes(m_rect.width));

  return bytes;
}

void Rect::deserialize(vector<uint8_t> & bytes) {
    m_rect.x = Serializer::u32FromBytes(bytes);;
    m_rect.y = Serializer::u32FromBytes(bytes);;
    m_rect.height = Serializer::u32FromBytes(bytes);;
    m_rect.width = Serializer::u32FromBytes(bytes);;
}