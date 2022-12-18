#include "faceDetectionUdpClient.h"

#include <faceDetectionRequest.h>
#include <faceDetectionResponse.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using std::invalid_argument;
using std::move;
using std::make_unique;
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
    
    auto message = make_unique<FaceDetectionRequestMessage>(image.clone());
    sendMessage(move(message));

    auto response = receiveMessage();
    if(response->code() != ApplicationMessage::Code::FaceDetectionResponse)
    {
        throw runtime_error("invalid message response");
    }

    auto faceDetectionResponse = static_cast<FaceDetectionResponseMessage*>(response.get());

    for (const auto boundingBox : faceDetectionResponse->facesBoudingBoxes()) {
        rectangle(image, boundingBox, cv::Scalar(255, 0, 0));
    }

    imshow("face detector result", image);
    waitKey();
}