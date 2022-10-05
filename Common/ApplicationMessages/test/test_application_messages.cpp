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

namespace {
constexpr auto DefaultMessageCode = 10u;
const vector<uint8_t> DefaultPayload{'p', 'a', 'y', 'l', 'o', 'a', 'd'};

}; // namespace

class TestApplicationMessages : public testing::Test {
public:
  TestApplicationMessages() {}

  vector<uint8_t> createDefaultMessageAsBytes() {
    vector<uint8_t> message;

    message.push_back(DefaultMessageCode);
    message.push_back(static_cast<uint8_t>(DefaultPayload.size()));
    message.push_back(static_cast<uint8_t>(DefaultPayload.size() >> 8));
    message.push_back(static_cast<uint8_t>(DefaultPayload.size() >> 16));
    message.push_back(static_cast<uint8_t>(DefaultPayload.size() >> 24));

    copy(DefaultPayload.begin(), DefaultPayload.end(), back_inserter(message));

    return message;
  }

  void createApplicationMessage(vector<uint8_t> &&message) {
    auto messageCopy = message;
    m_uut = make_unique<ApplicationMessage>(move(message));

    EXPECT_THAT(m_uut->convertToBytes(), ContainerEq(messageCopy));
  }

  void createApplicationMessage(uint8_t messageCode, vector<uint8_t> payload) {
    auto payloadCopy = payload;
    m_uut = make_unique<ApplicationMessage>(messageCode, move(payload));

    EXPECT_THAT(m_uut->header().code, messageCode);
    EXPECT_THAT(m_uut->payload(), ContainerEq(payloadCopy));
    EXPECT_THAT(m_uut->size(),
                payloadCopy.size() + sizeof(uint32_t) + sizeof(uint8_t));
  }

  unique_ptr<ApplicationMessage> m_uut;
};

TEST_F(TestApplicationMessages, ShouldCreateNotMessageFromEmptyBuffer) {

  auto createEmptyMessage = [this]() {
    m_uut = make_unique<ApplicationMessage>(vector<uint8_t>{});
  };

  EXPECT_THROW(createEmptyMessage(), invalid_argument);
}

TEST_F(TestApplicationMessages, ShouldCreateNotMessageWithoutPayloadSize) {

  auto createEmptyMessage = [this]() {
    const auto code = 0;
    m_uut = make_unique<ApplicationMessage>(vector<uint8_t>{code});
  };

  EXPECT_THROW(createEmptyMessage(), invalid_argument);
}

TEST_F(TestApplicationMessages, CreateMessageUsingParameters) {
  createApplicationMessage(DefaultMessageCode, DefaultPayload);
}

TEST_F(TestApplicationMessages,reserveSpace)
{
  createApplicationMessage(DefaultMessageCode,DefaultPayload);
  const auto extraSize = DefaultPayload.size()*2;
  m_uut->reserve(extraSize);

  EXPECT_THAT(m_uut->payload().capacity(),Eq(extraSize));
}

TEST_F(TestApplicationMessages, CreateMessageUsingRawVector) {
  auto messageBuffer = createDefaultMessageAsBytes();
  createApplicationMessage(move(messageBuffer));
}

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

  m_uut->payload().clear();
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
