#include "applicationMessages.h"
#include "faceDetectionRequest.h"
#include "faceDetectionResponse.h"

#include <iostream>
#include <stdexcept>

using std::cout;
using std::invalid_argument;
using std::make_unique;
using std::unique_ptr;
using std::vector;

ApplicationMessage::ApplicationMessage(Code code) : m_code(code) {}

ApplicationMessage::Code ApplicationMessage::code() const { return m_code; }

unique_ptr<ApplicationMessage>
FactoryApplicationMessage::create(std::vector<uint8_t> &&byteSequence) {
  if (byteSequence.size() < sizeof(ApplicationMessage::Header)) {
    throw invalid_argument(
        "Could not create a message, provided byte sequence is too short");
  }

  const auto code = static_cast<ApplicationMessage::Code>(byteSequence[0]);

  byteSequence.erase(byteSequence.begin(),
                     byteSequence.begin() + sizeof(ApplicationMessage::Header));
  try {
    switch (code) {
    case ApplicationMessage::Code::FaceDetectionRequest:
      return make_unique<FaceDetectionRequestMessage>(move(byteSequence));
      break;
    case ApplicationMessage::Code::FaceDetectionResponse:
      return make_unique<FaceDetectionResponseMessage>(move(byteSequence));
      break;
    default:
      throw invalid_argument("Invalid message code");
      break;
    }
  } catch (const std::exception &ex) {
    cout << "Could not create Application message, payload missmatch\n";
  }

  return nullptr;
}