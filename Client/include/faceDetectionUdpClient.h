#pragma once

#include "udpClient.h"

class FaceDetectionUdpClient : public UdpClient
{
public:
    FaceDetectionUdpClient(const std::string &ipAddress, uint16_t port);
    void run(int argc, char * argv[]);
};