#include "applicationMessages.h"
#include "faceDetectionRequest.h"
#include "faceDetectionResponse.h"

#include <assets.h>
#include <serializer.h>

#include <gmock/gmock.h>
#include <memory>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <stdint.h>

using namespace testing;
using namespace assets::testUtils;

using cv::imread;
using cv::Mat;
using cv::Rect2i;

using std::invalid_argument;
using std::make_unique;
using std::unique_ptr;
using std::vector;

class TestFaceDetectionRequestMessage : public testing::Test {
public:
  TestFaceDetectionRequestMessage() {}

  unique_ptr<FaceDetectionRequestMessage> m_uut;
};

TEST_F(TestFaceDetectionRequestMessage, createUsingEmptyMat) {
  EXPECT_THROW(
      [this]() {
        Mat img;
        m_uut = make_unique<FaceDetectionRequestMessage>(img);
      }(),
      invalid_argument);
}

TEST_F(TestFaceDetectionRequestMessage, internalMessageErase) {
  Mat img = imread(IMAGE1);
  m_uut = make_unique<FaceDetectionRequestMessage>(img);
}

class TestFaceDetectionResponseMessage : public testing::Test {
public:
  TestFaceDetectionResponseMessage() {}

  unique_ptr<FaceDetectionResponseMessage> m_uut;
};

TEST_F(TestFaceDetectionResponseMessage, createFromRects) {
  vector<Rect2i> rects;
  rects.emplace_back(0, 1, 2, 3);
  m_uut = make_unique<FaceDetectionResponseMessage>(rects);
}

TEST_F(TestFaceDetectionResponseMessage, createFromBytes) {
  vector<Rect2i> rects;
  rects.emplace_back(0, 1, 2, 3);
  auto bytes = Serializer::vectorRectToBytes(rects);
  m_uut = make_unique<FaceDetectionResponseMessage>(move(bytes));

  EXPECT_THAT(rects, ContainerEq(m_uut->facesBoudingBoxes()));
}

TEST_F(TestFaceDetectionResponseMessage, createFromEmptyVector) {
  vector<Rect2i> rects;
  m_uut = make_unique<FaceDetectionResponseMessage>(rects);

  EXPECT_THAT(rects, ContainerEq(m_uut->facesBoudingBoxes()));
}
