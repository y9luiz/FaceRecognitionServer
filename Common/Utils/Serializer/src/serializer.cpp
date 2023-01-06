#include "serializer.h"

#include <cstdint>
#include <iostream>
#include <iterator>

#include <opencv2/core/base.hpp>
#include <opencv2/core/types.hpp>
#include <stdexcept>

using cv::Mat;
using cv::Rect2i;

using std::back_inserter;
using std::invalid_argument;
using std::string;
using std::vector;
using std::copy;

namespace {
struct ImageHeader {
  uint32_t rows;
  uint32_t cols;
  uint32_t type;
};
} // namespace

vector<uint8_t> Serializer::u16ToBytes(uint16_t val) {
  vector<uint8_t> vec(2);

  vec[0] = val;
  vec[1] = val >> 8;

  return vec;
}

uint16_t Serializer::u16FromBytes(vector<uint8_t> &bytes) {
  auto val = u16FromBytes(cref(bytes));
  bytes.erase(bytes.begin(), bytes.begin() + sizeof(uint16_t));
  return val;
}

uint16_t Serializer::u16FromBytes(const vector<uint8_t> &bytes) {
  if (bytes.size() < sizeof(uint16_t)) {
    throw invalid_argument(
        "Byte sequence too short, could not extract uint16_t");
  }

  uint16_t val = bytes[0] | bytes[1] << 8;
  return val;
}

vector<uint8_t> Serializer::u32ToBytes(uint32_t val) {
  vector<uint8_t> vec(4);

  vec[0] = val;
  vec[1] = val >> 8;
  vec[2] = val >> 16;
  vec[3] = val >> 24;

  return vec;
}

uint32_t Serializer::u32FromBytes(vector<uint8_t> &bytes) {
  auto val = u32FromBytes(cref(bytes));
  bytes.erase(bytes.begin(), bytes.begin() + sizeof(uint32_t));
  return val;
}

uint32_t Serializer::u32FromBytes(const vector<uint8_t> &bytes) {
  if (bytes.size() < sizeof(uint32_t)) {
    throw invalid_argument(
        "Byte sequence too short, could not extract uint32_t");
  }

  uint32_t val = bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24;

  return val;
}

uint32_t Serializer::u32FromBytes(vector<uint8_t>::const_iterator start) {
  vector<uint8_t> bytes;
  bytes.reserve(sizeof(uint32_t));
  for (int i = 0; i < sizeof(uint32_t); i++) {
    bytes.emplace_back(*start++);
  }

  return u32FromBytes(bytes);
}

vector<uint8_t> Serializer::stringToBytes(const string &str) {
  vector<uint8_t> bytes;

  bytes.reserve(str.length() + sizeof(uint32_t));
  auto length = Serializer::u32ToBytes(str.size());
  bytes = move(length);
  copy(str.begin(), str.end(), back_inserter(bytes));
  return bytes;
}

string Serializer::stringFromBytes(vector<uint8_t> &bytes) {
  string str;
  auto length = Serializer::u32FromBytes(bytes);

  if (length > 0) {
    str.reserve(length);
    copy(bytes.begin(), bytes.begin() + length, back_inserter(str));

    bytes.erase(bytes.begin(), bytes.begin() + length);
  }

  return str;
}

uint32_t getNumberOfBytes(const Mat &image) {
  if (image.rows <= 0 || image.cols <= 0 || image.channels() <= 0) {
    return sizeof(ImageHeader);
  }

  const auto imageDataSize = image.total() * image.elemSize();

  return sizeof(ImageHeader) + imageDataSize;
}

vector<uint8_t> Serializer::matToBytes(const Mat &image) {
  vector<uint8_t> bytes( getNumberOfBytes(image));
  auto it = bytes.begin();

  const auto rows = u32ToBytes(image.rows);
  copy(rows.begin(), rows.end(), it);
  it += rows.size();

  const auto cols = u32ToBytes(image.cols);
  copy(cols.begin(), cols.end(), it);
  it += cols.size();

  const auto type = u32ToBytes(image.type());
  copy(type.begin(), type.end(), it);
  it += type.size();

  const auto size = image.total() * image.elemSize();

  memcpy(bytes.data() + sizeof(ImageHeader),image.data,size);

  return bytes;
}

Mat Serializer::matFromBytes(vector<uint8_t> &bytes) {
  ImageHeader header;
  header.rows = u32FromBytes(bytes);
  header.cols = u32FromBytes(bytes);
  header.type = u32FromBytes(bytes);

  Mat image(header.rows, header.cols, header.type);

  auto imageSize = image.total() * image.elemSize();

  memcpy(image.data, bytes.data(),imageSize);

  bytes.erase(bytes.cbegin(), bytes.cbegin() + imageSize);

  return image;
}

vector<uint8_t> Serializer::rectToBytes(const Rect2i &rect) {
  vector<uint8_t> bytes;
  bytes.reserve(sizeof(Rect2i));

  const auto positionX = u32ToBytes(rect.x);
  const auto positionY = u32ToBytes(rect.y);
  const auto height = u32ToBytes(rect.height);
  const auto width = u32ToBytes(rect.width);

  copy(positionX.begin(), positionX.end(), back_inserter(bytes));
  copy(positionY.begin(), positionY.end(), back_inserter(bytes));
  copy(height.begin(), height.end(), back_inserter(bytes));
  copy(width.begin(), width.end(), back_inserter(bytes));

  return bytes;
}

Rect2i Serializer::rectFromBytes(vector<uint8_t> &bytes) {
  const auto positionX = u32FromBytes(bytes);
  const auto positionY = u32FromBytes(bytes);
  const auto height = u32FromBytes(bytes);
  const auto width = u32FromBytes(bytes);

  return Rect2i(positionX, positionY, width, height);
}

vector<uint8_t> Serializer::vectorRectToBytes(const vector<Rect2i> &rects) {
  vector<uint8_t> bytes;
  bytes.reserve(rects.size() * sizeof(Rect2i) + sizeof(uint32_t));

  auto numberOfRects = u32ToBytes(rects.size());
  move(numberOfRects.begin(), numberOfRects.end(), back_inserter(bytes));
  for (const auto &rect : rects) {
    auto rectBytes = Serializer::rectToBytes(rect);
    move(rectBytes.begin(), rectBytes.end(), back_inserter(bytes));
  }

  return bytes;
}

vector<Rect2i> Serializer::vectorRectFromBytes(vector<uint8_t> &bytes) {
  vector<Rect2i> rects;
  rects.reserve(bytes.size() / sizeof(Rect2i));
  auto numberOfRects = u32FromBytes(bytes);

  if (numberOfRects * sizeof(Rect2i) > bytes.size()) {
    throw invalid_argument("Could not extract rectangles from byte set");
  }

  for (uint32_t i = 0; i < numberOfRects; i++) {
    rects.emplace_back(rectFromBytes(bytes));
  }

  return rects;
}