#pragma once

#include "udpClient.h"

class FaceRecognitionUdpClient : public UdpClient
{
public:
    FaceRecognitionUdpClient(const std::string &ipAddress, uint16_t port);
    void run(int argc, char * argv[]);
};