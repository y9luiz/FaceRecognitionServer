#pragma once

#include "parallelMessageReceiver.h"

class UdpSocket;

class UdpParallelMessageReceiver : public ParallelMessageReceiver {
public:
  UdpParallelMessageReceiver(std::unique_ptr<UdpSocket> socket);
};