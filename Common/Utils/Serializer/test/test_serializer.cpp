#include "serializer.h"
#include <assets.h>

#include <gmock/gmock.h>
#include <opencv2/opencv.hpp>
#include <stdexcept>

using namespace testing;
using namespace cv;
using namespace assets::testUtils;

using std::invalid_argument;
using std::string;
using std::vector;

namespace {
MATCHER_P(isSameImage, image, "check if 2 images are the same") {
  bool isSame = true;
  if (arg.rows != image.rows || image.cols != arg.cols ||
      image.channels() != arg.channels()) {
    isSame = false;
  }

  isSame = (sum(image != arg) == Scalar(0, 0, 0, 0));

  return isSame;
}
} // namespace

TEST(TestSerializer, failToConvertEmptyBytesToU16) {
  vector<uint8_t> emptyBytes;
  EXPECT_THROW(Serializer::u16FromBytes(emptyBytes), invalid_argument);
}

TEST(TestSerializer, convertU16) {
  uint16_t testNumber = 61236;
  auto bytes = Serializer::u16ToBytes(testNumber);
  EXPECT_THAT(Serializer::u16FromBytes(bytes), testNumber);
}

TEST(TestSerializer, failToConvertEmptyBytesToU32) {
  vector<uint8_t> emptyBytes;
  EXPECT_THROW(Serializer::u32FromBytes(emptyBytes), invalid_argument);
}

TEST(TestSerializer, convertU32) {
  uint32_t testNumber = 789922623;
  auto bytes = Serializer::u32ToBytes(testNumber);
  EXPECT_THAT(Serializer::u32FromBytes(bytes), testNumber);
  EXPECT_TRUE(bytes.empty());
}

TEST(TestSerializer, failConvertStringFromEmptyBytes) {
  vector<uint8_t> emptyBytes;
  EXPECT_THROW(Serializer::stringFromBytes(emptyBytes), invalid_argument);
}

TEST(TestSerializer, convertString) {
  string testString = "Hello World";
  auto stringBytes = Serializer::stringToBytes(testString);

  EXPECT_THAT(Serializer::stringFromBytes(stringBytes), testString);
  EXPECT_EQ(stringBytes.size(), 0);
}

TEST(TestSerializer, failConvertMatFromEmptyBytes) {
  vector<uint8_t> emptyBytes;
  EXPECT_THROW(Serializer::matFromBytes(emptyBytes), invalid_argument);
}

TEST(TestSerializer, convertCvMat) {
  const auto image = imread(IMAGE1);
  auto imageBytes = Serializer::matToBytes(image);
  EXPECT_THAT(Serializer::matFromBytes(imageBytes), isSameImage(image));
  EXPECT_TRUE(imageBytes.empty());
}

TEST(TestSerializer, failConvertCvRectFromEmptyBytes) {
  vector<uint8_t> emptyBytes;
  EXPECT_THROW(Serializer::rectFromBytes(emptyBytes), invalid_argument);
}

TEST(TestSerializer, convertCvRect) {
  const auto rect = Rect2i(4, 2, 3, 6);
  auto rectBytes = Serializer::rectToBytes(rect);

  EXPECT_THAT(Serializer::rectFromBytes(rectBytes), rect);
  EXPECT_EQ(rectBytes.size(), 0);
}

TEST(TestSerializer, failConvertCvRectVectorFromEmptyBytes) {
  vector<uint8_t> emptyBytes;
  EXPECT_THROW(Serializer::vectorRectFromBytes(emptyBytes), invalid_argument);
}

TEST(TestSerializer, failConvertCvRectVectorFromIncompleteBytes) {
  vector<uint8_t> emptyBytes{6, 1, 2, 3, 4, 5, 6};
  EXPECT_THROW(Serializer::vectorRectFromBytes(emptyBytes), invalid_argument);
}

TEST(TestSerializer, convertCvRectVector) {
  vector<Rect2i> rects;
  rects.emplace_back(Rect2i(0, 0, 0, 1));
  rects.emplace_back(Rect2i(3, 0, 0, 1));
  rects.emplace_back(Rect2i(3, 8, 44, 1));
  rects.emplace_back(Rect2i(23, 8, 44, 1));

  auto rectsBytes = Serializer::vectorRectToBytes(rects);

  EXPECT_THAT(Serializer::vectorRectFromBytes(rectsBytes), rects);
  EXPECT_EQ(rectsBytes.size(), 0);
}