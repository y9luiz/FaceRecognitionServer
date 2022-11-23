#include "udpParallelMessageReceiver.h"
#include "udpMessageReceiver.h"

#include <memory>

using std::make_unique;
using std::move;
using std::unique_ptr;

UdpParallelMessageReceiver::UdpParallelMessageReceiver(
    unique_ptr<UdpSocket> socket)
    : ParallelMessageReceiver(make_unique<UdpMessageReceiver>(move(socket))) {
  start();
}