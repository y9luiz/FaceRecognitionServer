#include "image.h"

#include <serializer.h>

using cv::Mat;
using std::move;
using std::vector;

namespace {
uint32_t getCvImageSize(const Mat &image) {
  const auto imageHeaderSize =
      sizeof(image.cols) + sizeof(image.rows) + sizeof(image.type());

  if (image.rows <= 0 || image.cols <= 0 || image.channels() <= 0) {
    return imageHeaderSize;
  }

  const auto imageDataSize = image.total() * image.elemSize();

  return imageHeaderSize + imageDataSize;
}
} // namespace

Image::Image(Mat &&image) : m_image(move(image)) {}

Image::Image(std::vector<uint8_t> & data)
{
    deserialize(data);
}


std::vector<uint8_t> Image::serialize() const {
  vector<uint8_t> data;
  data.reserve(getCvImageSize(m_image));

  Serializer::append(data, Serializer::u32ToBytes(m_image.rows));
  Serializer::append(data, Serializer::u32ToBytes(m_image.cols));
  Serializer::append(data, Serializer::u32ToBytes(m_image.type()));
  for (int i = 0; i < m_image.rows; ++i) {
    data.insert(data.end(), m_image.ptr<uchar>(i),
                 m_image.ptr<uchar>(i) + m_image.cols * m_image.channels());
  }

  return data;
}

void Image::deserialize(std::vector<uint8_t> &bytes) {
  const auto rows = Serializer::u32FromBytes(bytes);
  const auto cols = Serializer::u32FromBytes(bytes);
  const auto type = Serializer::u32FromBytes(bytes);

  Mat image(rows, cols, type, reinterpret_cast<void *>(bytes.data()));

  image.copyTo(image);
}