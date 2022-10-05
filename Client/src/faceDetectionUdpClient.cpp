#include "faceDetectionUdpClient.h"

#include <faceDetectionRequest.h>
#include <faceDetectionResponse.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using std::invalid_argument;
using std::move;
using cv::Mat;
using cv::imread;
using cv::rectangle;
using std::runtime_error;
using cv::imshow;
using cv::waitKey;


FaceDetectionUdpClient::FaceDetectionUdpClient(const std::string &ipAddress, uint16_t port)
: UdpClient(ipAddress,port){
}

void FaceDetectionUdpClient::run(int argc, char * argv[])
{
    if (argc != 2) {
      throw invalid_argument("./Client <image_path>");
    }

    const auto imagePath = argv[1];
    const auto image = imread(imagePath);
    
    auto message = FaceDetectionRequestMessage(image.clone());
    sendMessage(move(message));

    auto response = receiveMessage();
    if(response.header().code != static_cast<uint8_t>(ApplicationMessage::Types::FaceDetectionResponse))
    {
        throw runtime_error("invalid message response");
    }

    FaceDetectionResponseMessage faceDetectionResponse(move(response.payload()));

    for (const auto boundingBox : faceDetectionResponse.facesBoudingBoxes()) {
        rectangle(image, boundingBox, cv::Scalar(255, 0, 0));
    }

    imshow("face detector result", image);
    waitKey();
}