#include "utils.h"

using cv::Mat;
using cv::Rect2i;
using std::vector;

uint32_t utils::getNumberOfBytes(const vector<Rect2i> &payload) {
  return sizeof(Rect2i) * payload.size() + sizeof(uint32_t);
}

uint32_t utils::getNumberOfBytes(const Mat &image) {
  const auto imageHeaderSize =
      sizeof(image.cols) + sizeof(image.rows) + sizeof(image.type());

  if (image.rows <= 0 || image.cols <= 0 || image.channels() <= 0) {
    return imageHeaderSize;
  }

  const auto imageDataSize = image.total() * image.elemSize();

  return imageHeaderSize + imageDataSize;
}