#pragma once

#include "boost/asio/ip/udp.hpp"
#include "udpSocketFactory.h"

#include <gmock/gmock.h>

class MockUdpSocketFactory {

public:
  MockUdpSocketFactory();
  virtual ~MockUdpSocketFactory();

  MOCK_METHOD(UdpSocketFactory::UdpSocket, createAndOpenSocket,
              (boost::asio::ip::udp));
  MOCK_METHOD(UdpSocketFactory::UdpSocket, createOpenAndBindSocket,
              (boost::asio::ip::udp::endpoint));
};