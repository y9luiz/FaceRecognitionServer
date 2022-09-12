#include "applicationMessages.h"

class FaceDetectionRequestMessage : public ApplicationMessage {
public:
  FaceDetectionRequestMessage(const cv::Mat &payload);
  FaceDetectionRequestMessage(std::vector<uint8_t> &&payload);

  cv::Mat& image();

private:
  cv::Mat m_image;
};