#include "faceDetectionUdpServer.h"
#include "applicationMessages.h"
#include "messageHandler.h"
#include "serializer.h"
#include "udpServer.h"
#include "faceDetectionRequest.h"

#include <memory>
#include <opencv2/opencv.hpp>

using std::string;
using std::vector;
using std::move;
using std::make_unique;

FaceDetectionUdpServer::FaceDetectionUdpServer(const std::string &ip, uint16_t port) : UdpServer(ip,port)
{
    m_messageHandler = std::make_unique<MessageHandler>();
    m_messageHandler->registerCallback(static_cast<uint8_t>(ApplicationMessage::Types::FaceDetectionRequest),[](std::vector<uint8_t> && message){         
        auto payloadSize = Serializer::u32FromBytes(message);
        
        message.erase(message.begin(),message.begin()+sizeof(uint32_t));
        message.resize(payloadSize);
        
        auto image = Serializer::MatFromBytes(message);
        cv::imshow("img", image);
        cv::waitKey();
        
    });
}