#include "applicationMessages.h"
#include "faceDetectionRequest.h"
#include "faceDetectionResponse.h"

using std::make_unique;
using std::unique_ptr;
using std::vector;

ApplicationMessage::ApplicationMessage(Code code) : m_code(code) {}

ApplicationMessage::Code ApplicationMessage::code() const { return m_code; }

unique_ptr<ApplicationMessage>
FactoryApplicationMessage::create(std::vector<uint8_t> &&byteSequence) {
  const auto code = static_cast<ApplicationMessage::Code>(byteSequence[0]);

  byteSequence.erase(byteSequence.begin());
  byteSequence.erase(byteSequence.begin(),
                     byteSequence.begin() + sizeof(uint32_t));

  switch (code) {
  case ApplicationMessage::Code::FaceDetectionRequest:
    return make_unique<FaceDetectionRequestMessage>(move(byteSequence));
    break;
  case ApplicationMessage::Code::FaceDetectionResponse:
    return make_unique<FaceDetectionResponseMessage>(move(byteSequence));
    break;
  }

  return nullptr;
}