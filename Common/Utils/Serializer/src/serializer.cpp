#include "serializer.h"

#include <cstdint>
#include <iostream>
#include <iterator>

#include <opencv2/core/base.hpp>

using cv::Mat;
using cv::Rect2i;

using std::back_inserter;
using std::string;
using std::vector;

vector<uint8_t> Serializer::u16ToBytes(uint16_t val) {
  vector<uint8_t> vec(2);

  vec[0] = val;
  vec[1] = val >> 8;

  return vec;
}

uint16_t Serializer::u16FromBytes(const vector<uint8_t> &bytes) {
  return bytes[0] | bytes[1] << 8;
}

vector<uint8_t> Serializer::u32ToBytes(uint32_t val) {
  vector<uint8_t> vec(4);

  vec[0] = val;
  vec[1] = val >> 8;
  vec[2] = val >> 16;
  vec[3] = val >> 24;

  return vec;
}

uint32_t Serializer::u32FromBytes(const vector<uint8_t> &bytes) {
  return bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24;
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

  bytes.erase(bytes.begin(), bytes.begin() + sizeof(uint32_t));

  if (length > 0) {
    str.reserve(length);
    copy(bytes.begin(), bytes.begin() + length, back_inserter(str));

    bytes.erase(bytes.begin(), bytes.begin() + length);
  }

  return str;
}

void Serializer::concatenate(vector<uint8_t> &left,
                             const vector<uint8_t> &right) {
  copy(right.begin(), right.end(), back_inserter(left));
}

vector<uint8_t> Serializer::concatenate(const vector<uint8_t> &left,
                                        const vector<uint8_t> &right) {
  vector<uint8_t> output(left);

  copy(right.begin(), right.end(), back_inserter(output));

  return output;
}

uint32_t Serializer::getNumberOfBytes(const Mat &image) {
  const auto imageHeaderSize =
      sizeof(image.cols) + sizeof(image.rows) + sizeof(image.type());
  const auto imageDataSize = image.total() * image.elemSize();

  return imageHeaderSize + imageDataSize;
}

vector<uint8_t> Serializer::MatToBytes(const Mat &image) {
  vector<uint8_t> bytes;
  bytes.reserve(getNumberOfBytes(image));

  const auto rows = u32ToBytes(image.rows);
  bytes.insert(bytes.end(), rows.begin(), rows.end());

  const auto cols = u32ToBytes(image.cols);
  bytes.insert(bytes.end(), cols.begin(), cols.end());

  const auto type = u32ToBytes(image.type());
  bytes.insert(bytes.end(), type.begin(), type.end());

  for (int i = 0; i < image.rows; ++i) {
    bytes.insert(bytes.end(), image.ptr<uchar>(i),
                 image.ptr<uchar>(i) + image.cols * image.channels());
  }

  return bytes;
}

Mat Serializer::MatFromBytes(vector<uint8_t> &bytes) {
  const auto rows = u32FromBytes(bytes);
  bytes.erase(bytes.begin(), bytes.begin() + sizeof(uint32_t));

  const auto cols = u32FromBytes(bytes);
  bytes.erase(bytes.begin(), bytes.begin() + sizeof(uint32_t));

  const auto type = u32FromBytes(bytes);
  bytes.erase(bytes.begin(), bytes.begin() + sizeof(uint32_t));

  Mat image(rows, cols, type, reinterpret_cast<void *>(bytes.data()));
  return image;
}

vector<uint8_t> Serializer::RectToBytes(const Rect2i &rect) {
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

Rect2i Serializer::RectFromBytes(vector<uint8_t> &bytes) {
  const auto positionX = u32FromBytes(bytes);
  bytes.erase(bytes.begin(), bytes.begin() + sizeof(uint32_t));

  const auto positionY = u32FromBytes(bytes);
  bytes.erase(bytes.begin(), bytes.begin() + sizeof(uint32_t));

  const auto height = u32FromBytes(bytes);
  bytes.erase(bytes.begin(), bytes.begin() + sizeof(uint32_t));

  const auto width = u32FromBytes(bytes);
  bytes.erase(bytes.begin(), bytes.begin() + sizeof(uint32_t));

  return Rect2i(positionX, positionY, width, height);
}

vector<uint8_t> Serializer::VectorRectToBytes(const vector<Rect2i> &rects) {
  vector<uint8_t> bytes;
  bytes.reserve(rects.size() * sizeof(Rect2i) + sizeof(uint32_t));

  auto numberOfRects = u32ToBytes(rects.size());
  move(numberOfRects.begin(), numberOfRects.end(), back_inserter(bytes));
  for (const auto &rect : rects) {
    auto rectBytes = Serializer::RectToBytes(rect);
    move(rectBytes.begin(), rectBytes.end(), back_inserter(bytes));
  }

  return bytes;
}

vector<Rect2i> Serializer::VectorRectFromBytes(vector<uint8_t> &bytes) {
  vector<Rect2i> rects;
  rects.reserve(bytes.size() / sizeof(Rect2i));

  auto numberOfRects = u32FromBytes(bytes);
  bytes.erase(bytes.begin(), bytes.begin() + sizeof(uint32_t));
  for (uint32_t i = 0; i < numberOfRects; i++) {
    rects.emplace_back(RectFromBytes(bytes));
  }

  return rects;
}

vector<uint8_t> Serializer::EndpointToBytes(const Endpoint &endpoint) {

  vector<uint8_t> address = Serializer::stringToBytes(endpoint.address);
  vector<uint8_t> port = Serializer::u16ToBytes(endpoint.port);

  vector<uint8_t> bytes = move(address);
  copy(port.begin(), port.end(), back_inserter(bytes));

  return bytes;
}

Endpoint Serializer::EndpointFromBytes(vector<uint8_t> &bytes) {
  const auto address = Serializer::stringFromBytes(bytes);
  const auto port = Serializer::u16FromBytes(bytes);

  Endpoint endpoint;
  endpoint.address = address;
  endpoint.port = port;

  return endpoint;
}