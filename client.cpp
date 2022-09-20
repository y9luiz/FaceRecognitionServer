#include "faceDetectionResponse.h"
#include <applicationMessages.h>
#include <faceDetectionRequest.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdexcept>
#include <thread>
#include <udpClient.h>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace chrono_literals;

int main(int argc, char *argv[]) {
  UdpClient client("127.0.0.1", 5000);
  try {

    if (argc != 2) {
      throw invalid_argument("./Client <image_path>");
    }
    const auto imagePath = argv[1];
    auto image = cv::imread(imagePath);
    if (image.rows <= 0 || image.cols <= 0) {
      throw invalid_argument("invalid image");
    }

    auto message = FaceDetectionRequestMessage(image.clone());

    client.sendMessage(move(message));
    auto response = client.receiveMessage();
    auto payload = response.payload();
    auto faceDetectionResponse =
        FaceDetectionResponseMessage(std::move(payload));

    for (const auto boundingBox : faceDetectionResponse.facesBoudingBoxes()) {
      cv::rectangle(image, boundingBox, cv::Scalar(255, 0, 0));
    }

    cv::imshow("faces", image);
    cv::waitKey(10000);

    std::cout << "client finished\n";

  } catch (const exception &e) {
    cout << e.what() << endl;
  }

  return 0;
}
