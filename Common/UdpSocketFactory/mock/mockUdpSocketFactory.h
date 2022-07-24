#pragma once

#include "udpSocketFactory.h"

#include <gmock/gmock.h>

class MockUdpSocketFactory {

public:
  MockUdpSocketFactory();
  virtual ~MockUdpSocketFactory();

  MOCK_METHOD(void, constructor, (boost::asio::io_context &));
  MOCK_METHOD(std::unique_ptr<UdpSocketFactory::UdpSocket>, createAndOpenSocket,
              (boost::asio::ip::udp));
  MOCK_METHOD(std::unique_ptr<UdpSocketFactory::UdpSocket>,
              createOpenAndBindSocket, (boost::asio::ip::udp::endpoint));
};