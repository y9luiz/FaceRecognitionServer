#pragma once

#include <udpSocket.h>

#include <gmock/gmock.h>

class MockUdpSocket {
public:
  MockUdpSocket();
  virtual ~MockUdpSocket();

  MOCK_METHOD(void, constructor, (IpProtocolVersion));
  MOCK_METHOD(void, bind, (const Endpoint &));
  MOCK_METHOD(void, open, ());
  MOCK_METHOD(std::size_t, receiveFrom, (std::vector<uint8_t> &, Endpoint &),
              ());
  MOCK_METHOD(std::size_t, sendTo,
              (const std::vector<uint8_t> &, const Endpoint &), ());
};