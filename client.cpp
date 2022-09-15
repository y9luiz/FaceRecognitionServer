#include <applicationMessages.h>
#include <faceDetectionRequest.h>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <udpClient.h>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace chrono_literals;

int main(int argc, char * argv[]) {
  UdpClient client("127.0.0.1", 5000);
  try {
    
    if(argc != 2)
    {
      throw invalid_argument("./Client <image_path>");
    }
    const auto imagePath = argv[1];
    auto image = cv::imread(imagePath);

    if(image.rows <= 0 || image.cols <= 0)
    {
      throw invalid_argument("invalid image");
    }

    auto message = FaceDetectionRequestMessage(image);
    cout << "payload size " << message.header().payloadSize << "\n";

    client.sendMessage(move(message));
  } catch (const exception &e) {
   cout << e.what() << endl;
  }

  return 0;
}
