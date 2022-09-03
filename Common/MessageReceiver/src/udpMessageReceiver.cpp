#include "udpMessageReceiver.h"
#include <applicationMessages.h>
#include <exception>
#include <udpSocket.h>

#include <memory>
#include <stdexcept>
#include <vector>
#include <optional>
#include <iostream>

using std::move;
using std::nullopt;
using std::optional;
using std::runtime_error;
using std::unique_ptr;
using std::vector;
using std::cout;

UdpMessageReceiver::UdpMessageReceiver(unique_ptr<UdpSocket> socket)
    : m_socket(move(socket)) {
      start();
}

UdpMessageReceiver::~UdpMessageReceiver()
{
  try{
    stop();
  }
  catch(std::exception &e)
  {
    cout << "[ERROR::~UdpMessageReceiver()] " << e.what() <<"\n";
  }
}

optional<ApplicationMessage> UdpMessageReceiver::receiveMessage() {
  try{
    
    vector<uint8_t> buffer(MaximumPacketSize);

    auto receivedDataSize = m_socket->receiveFrom(buffer, m_endpoint);

    if (!receivedDataSize) {
      throw runtime_error("Could not receive the message");
    }

    ApplicationMessage::Header header(buffer);
    
    for(int i = 0; i < sizeof(ApplicationMessage::Header); i++)
    {
      buffer.erase(buffer.begin());
    }

    ApplicationMessage message(header.code,header.payloadSize,move(buffer));

    uint32_t totalMessageSize = receivedDataSize;

    while(receivedDataSize < header.payloadSize)
    {
      auto receivedDataSize = m_socket->receiveFrom(message.payload(), m_endpoint);

      if(receivedDataSize)
      {
        totalMessageSize += receivedDataSize;
      }
      else{
        throw runtime_error("Message incomplete");
      }
    }

    return message;
  }
  catch(const runtime_error&runtimeError)
  {
    cout << runtimeError.what() <<"\n";
    return nullopt;
  }
}

void UdpMessageReceiver::start()
{
  cout << "[INFO::UdpMessageReceiver] created UdpMessageReceiver\n";
  MessageReceiverInterface::start();
}

void UdpMessageReceiver::stop()
{
  MessageReceiverInterface::stop();
}
