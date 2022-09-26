#include "faceDetectionResponse.h"
#include <applicationMessages.h>
#include <faceDetectionRequest.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
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
    cv::VideoCapture cap(imagePath);
    /// auto image = cv::imread(imagePath);
    // if (image.rows <= 0 || image.cols <= 0) {
    if (!cap.isOpened()) {
      throw invalid_argument("invalid image");
    }

    while (cap.isOpened()) {
      cv::Mat image;
      cap >> image;

      cv::resize(image, image, cv::Size(320, 320));

      // cv::imshow("test", image);
      // cv::waitKey(10000);
      auto message = FaceDetectionRequestMessage(image.clone());
      std::cout << "Payload size " << message.payload().size() << "\n";
      client.sendMessage(move(message));
      std::cout << "waiting for response\n";
      auto response = client.receiveMessage();
      auto payload = response.payload();

      auto faceDetectionResponse =
          FaceDetectionResponseMessage(std::move(payload));

      for (const auto boundingBox : faceDetectionResponse.facesBoudingBoxes()) {
        cv::rectangle(image, boundingBox, cv::Scalar(255, 0, 0));
      }

      cv::imshow("faces", image);
      cv::waitKey(1);
    }
    std::cout << "client finished\n";
  } catch (const exception &e) {
    cout << e.what() << endl;
  }

  return 0;
}
