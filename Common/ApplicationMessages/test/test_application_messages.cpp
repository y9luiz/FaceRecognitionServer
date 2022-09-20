#include "applicationMessages.h"
#include "faceDetectionRequest.h"
#include "faceDetectionResponse.h"

#include <serializer.h>

#include <gmock/gmock.h>
#include <memory>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <stdint.h>

using namespace testing;

using cv::imread;
using cv::Mat;
using cv::Rect2i;

using std::invalid_argument;
using std::make_unique;
using std::unique_ptr;
using std::vector;

namespace {
constexpr auto DefaultMessageCode = 10u;
constexpr auto DefaultPayloadSize = 9u;
const vector<uint8_t> DefaultPayload{'p', 'a', 'y', 'l', 'o', 'a', 'd'};

}; // namespace

class TestApplicationMessages : public testing::Test {
public:
  TestApplicationMessages() {}

  vector<uint8_t> createDefaultMessageAsBytes() {
    vector<uint8_t> message;

    message.push_back(DefaultMessageCode);
    message.push_back(static_cast<uint8_t>(DefaultPayloadSize));
    message.push_back(DefaultPayloadSize >> 8);
    message.push_back(DefaultPayloadSize >> 16);
    message.push_back(DefaultPayloadSize >> 24);

    copy(DefaultPayload.begin(), DefaultPayload.end(), back_inserter(message));

    return message;
  }

  void createApplicationMessage(vector<uint8_t> &&message) {
    auto messageCopy = message;
    m_uut = make_unique<ApplicationMessage>(move(message));

    EXPECT_THAT(m_uut->convertToBytes(), ContainerEq(messageCopy));
  }

  void createApplicationMessage(uint8_t messageCode, uint16_t payloadSize,
                                vector<uint8_t> payload) {
    auto payloadCopy = payload;
    m_uut = make_unique<ApplicationMessage>(
        ApplicationMessage::Header{messageCode, payloadSize}, move(payload));

    EXPECT_THAT(m_uut->header().code, messageCode);
    EXPECT_THAT(m_uut->header().payloadSize, payloadSize);
    EXPECT_THAT(m_uut->payload(), ContainerEq(payloadCopy));
    EXPECT_THAT(m_uut->size(),
                payloadSize + sizeof(messageCode) + sizeof(payloadSize));
  }

  unique_ptr<ApplicationMessage> m_uut;
};

TEST_F(TestApplicationMessages, ShouldCreateNotMessageWithNotValidSize) {

  auto createEmptyMessage = [this]() {
    m_uut = make_unique<ApplicationMessage>(vector<uint8_t>{});
  };

  EXPECT_THROW(createEmptyMessage(), invalid_argument);
}

TEST_F(TestApplicationMessages, CreateMessageUsingParameters) {
  createApplicationMessage(DefaultMessageCode, DefaultPayloadSize,
                           DefaultPayload);
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

TEST_F(TestFaceDetectionRequestMessage, DISABLED_internalMessageErase) {
  Mat img = imread("/home/lcorreia/Pictures/car.png");
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
  auto bytes = Serializer::VectorRectToBytes(rects);
  m_uut = make_unique<FaceDetectionResponseMessage>(move(bytes));
}

TEST_F(TestFaceDetectionResponseMessage, createFromEmptyVector) {
  vector<Rect2i> rects;
  auto bytes = Serializer::VectorRectToBytes(rects);
  m_uut = make_unique<FaceDetectionResponseMessage>(move(bytes));
  m_uut = make_unique<FaceDetectionResponseMessage>(rects);
}
