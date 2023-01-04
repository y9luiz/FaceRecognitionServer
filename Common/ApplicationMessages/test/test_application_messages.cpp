#include "applicationMessages.h"
#include "faceDetectionRequest.h"
#include "faceDetectionResponse.h"
#include "faceRecognitionRequest.h"
#include "faceRecognitionResponse.h"

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
using cv::Scalar;

using std::invalid_argument;
using std::make_pair;
using std::make_unique;
using std::unique_ptr;
using std::vector;

namespace {
vector<Rect2i> DefaultRectSet{{1, 2, 3, 6}, {14, 7, 99, 8}, {77, 88, 99, 77}};

MATCHER_P(isSameImage, img, "Check if two opencv images are the same") {
  const bool isEqual = sum(img != arg) == Scalar(0, 0, 0);

  return isEqual;
}

MATCHER_P(isSameDescriptors, descriptors, "Check if two descriptors set are the same") {
  if(descriptors.size() != arg.size())
  {
    return false;
  }

  for(int i = 0; i < descriptors.size();i++)
  {
    const bool isEqual = sum(descriptors[i] != arg[i]) == Scalar(0, 0, 0);

    if(!isEqual)
    {
      return false;
    }
  }

  return true;
}
} // namespace

class TestBase : public Test {
public:
  TestBase() { m_testImage = imread(IMAGE1); }

  cv::Mat m_testImage;
};

class TestFactoryApplicationMessage : public TestBase {
public:
  TestFactoryApplicationMessage() {}

  FactoryApplicationMessage m_uut;
};

TEST_F(TestFactoryApplicationMessage, createFaceDetectionRequestMessage) {
  const auto msg =
      FactoryApplicationMessage::create<FaceDetectionRequestMessage>(
          m_testImage);

  EXPECT_THAT(msg, Not(IsNull()));
  EXPECT_THAT(FactoryApplicationMessage::create(msg->serialize()),
              Not(IsNull()));

  EXPECT_THAT(msg->image(),isSameImage(m_testImage));
}

TEST_F(TestFactoryApplicationMessage,
       createFaceDetectionRequestMessageSadPath) {
  Mat emptyImg;
  EXPECT_THROW(
      FactoryApplicationMessage::create<FaceDetectionRequestMessage>(emptyImg),
      invalid_argument);
  EXPECT_THROW(FactoryApplicationMessage::create({}), invalid_argument);

  ApplicationMessage::Header emptyHeader;
  vector<uint8_t> bytes;
  copy(emptyHeader.begin(), emptyHeader.end(), back_inserter(bytes));
  EXPECT_EQ(FactoryApplicationMessage::create(move(bytes)), nullptr);
}

TEST_F(TestFactoryApplicationMessage, createFaceDetectionResponseMessage) {
  const auto msg =
      FactoryApplicationMessage::create<FaceDetectionResponseMessage>(
          DefaultRectSet);

  EXPECT_THAT(msg, Not(IsNull()));
  EXPECT_THAT(FactoryApplicationMessage::create(msg->serialize()),
              Not(IsNull()));
}

TEST_F(TestFactoryApplicationMessage,
       createFaceDetectionResponseMessageSadPath) {
  EXPECT_THROW(FactoryApplicationMessage::create({}), invalid_argument);
}

TEST_F(TestFactoryApplicationMessage, createFaceRecognitionRequestMessage) {
  const auto msg =
      FactoryApplicationMessage::create<FaceRecognitionRequestMessage>(
          make_pair(m_testImage, DefaultRectSet));

  EXPECT_THAT(msg, Not(IsNull()));
  EXPECT_THAT(FactoryApplicationMessage::create(msg->serialize()),
              Not(IsNull()));
}

TEST_F(TestFactoryApplicationMessage,
       createFaceRecognitionRequestMessageSadPath) {
  auto createUsingEmptyImage = [this]() {
    const auto msg =
        FactoryApplicationMessage::create<FaceRecognitionRequestMessage>(
            make_pair(Mat(), DefaultRectSet));
  };

  auto createUsingNoBoudingBoxes = [this](){
        vector<Rect2i> emptyVec;
        FactoryApplicationMessage::create<FaceRecognitionRequestMessage>(
            make_pair(m_testImage, emptyVec));
  };

  EXPECT_THROW(createUsingNoBoudingBoxes(), invalid_argument);
}

class TestFaceDetectionRequestMessage : public TestBase {
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

TEST_F(TestFaceDetectionRequestMessage, happyPath) {
  m_uut = make_unique<FaceDetectionRequestMessage>(m_testImage);
  EXPECT_THAT(m_uut->image(),isSameImage(m_testImage));
}

class TestFaceDetectionResponseMessage : public TestBase {
public:
  TestFaceDetectionResponseMessage() {}

  unique_ptr<FaceDetectionResponseMessage> m_uut;
};

TEST_F(TestFaceDetectionResponseMessage, createFromRects) {
  m_uut = make_unique<FaceDetectionResponseMessage>(DefaultRectSet);
}

TEST_F(TestFaceDetectionResponseMessage, createFromBytes) {
  auto bytes = Serializer::vectorRectToBytes(DefaultRectSet);
  m_uut = make_unique<FaceDetectionResponseMessage>(move(bytes));

  EXPECT_THAT(DefaultRectSet, ContainerEq(m_uut->facesBoudingBoxes()));
}

TEST_F(TestFaceDetectionResponseMessage, createFromEmptyVector) {
  m_uut = make_unique<FaceDetectionResponseMessage>(DefaultRectSet);

  EXPECT_THAT(DefaultRectSet, ContainerEq(m_uut->facesBoudingBoxes()));
}

class TestFaceRecognitionRequestMessage : public TestBase {
public:
  TestFaceRecognitionRequestMessage() {}

  unique_ptr<FaceRecognitionRequestMessage> m_uut;
};

TEST_F(TestFaceRecognitionRequestMessage, createFromPair) {
  FaceRecognitionRequestMessage::PayloadT payload =
      make_pair(m_testImage, DefaultRectSet);

  m_uut = make_unique<FaceRecognitionRequestMessage>(payload);

  EXPECT_THAT(payload.first, isSameImage(m_uut->image()));
  EXPECT_THAT(payload.second, ContainerEq(m_uut->facesBoudingBoxes()));
}

TEST_F(TestFaceRecognitionRequestMessage, createFromBytes) {
  FaceRecognitionRequestMessage::PayloadT payload =
      make_pair(m_testImage, DefaultRectSet);

  m_uut = make_unique<FaceRecognitionRequestMessage>(payload);

  auto bytes = m_uut->serialize();

  // remove the header and leave just the payload
  bytes.erase(bytes.cbegin(),
              bytes.cbegin() + sizeof(ApplicationMessage::Header));

  m_uut = make_unique<FaceRecognitionRequestMessage>(move(bytes));

  // TODO: replace this by the appropriated matcher
  EXPECT_FALSE(m_uut->image().empty());
  EXPECT_THAT(payload.second, ContainerEq(m_uut->facesBoudingBoxes()));
}

class TestFaceRecognitionResponseMessage : public TestBase {
public:
  TestFaceRecognitionResponseMessage() {
    constexpr auto numberOftestDescriptors = 6u;
    for(int i = 0; i < numberOftestDescriptors;i++)
    {
        m_testDescriptors.push_back(Mat(12,1,CV_32F,Scalar(i)));
    }
  }

  vector<Mat> m_testDescriptors;
  unique_ptr<FaceRecognitionResponseMessage> m_uut;
};

TEST_F(TestFaceRecognitionResponseMessage, createFromDescriptors) {
  m_uut = make_unique<FaceRecognitionResponseMessage>(m_testDescriptors);

  EXPECT_THAT(m_uut->descriptors(),isSameDescriptors(m_testDescriptors));
}

TEST_F(TestFaceRecognitionResponseMessage, createFromBytes) {
  m_uut = make_unique<FaceRecognitionResponseMessage>(m_testDescriptors);

  auto bytes = m_uut->serialize();

  // remove the header and leave just the payload
  bytes.erase(bytes.cbegin(),
              bytes.cbegin() + sizeof(ApplicationMessage::Header));

  m_uut = make_unique<FaceRecognitionResponseMessage>(move(bytes));

  EXPECT_THAT(m_uut->descriptors(),isSameDescriptors(m_testDescriptors));
}