#pragma once

#include "udpServer.h"

class FaceDetectionUdpServer : public UdpServer
{
    public:
        FaceDetectionUdpServer(const std::string &ip, uint16_t port);
        virtual ~FaceDetectionUdpServer() = default;
    private:
        void setMessageHandler(std::unique_ptr<MessageHandler> messageHandler);
};