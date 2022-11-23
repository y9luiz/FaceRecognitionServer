#include "faceDetectionResponse.h"
#include <applicationMessages.h>
#include <faceDetectionRequest.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <stdexcept>
#include <thread>
#include <faceDetectionUdpClient.h>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace chrono_literals;

int main(int argc, char *argv[]) {
  FaceDetectionUdpClient client("127.0.0.1", 5000);
  try {
    client.run(argc, argv);
  } 
  catch (const exception &e) {
    cout << e.what() << endl;
  }

  return 0;
}
