#include "serializer.h"

#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>

using namespace testing;

class TestSerializer : public Test {
public:
  TestSerializer() {}
};

TEST_F(TestSerializer, convertU16) {
  uint16_t testNumber = 61236;
  const auto bytes = Serializer::u16ToBytes(testNumber);
  EXPECT_THAT(Serializer::u16FromBytes(bytes), testNumber);
}

TEST_F(TestSerializer, convertU32) {
  uint32_t testNumber = 789922623;
  const auto bytes = Serializer::u32ToBytes(testNumber);
  EXPECT_THAT(Serializer::u32FromBytes(bytes), testNumber);
}

TEST_F(TestSerializer, convertCVMat) {
  // cv::Mat
}
