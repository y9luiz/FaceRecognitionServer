#include "udpServer.h"
#include <iostream>

using namespace std;

int main() {
  UdpServer server("127.0.0.1", 5000);
  server.setProcessMessageCallback([](std::vector<uint8_t> &buffer) {
    for (const auto &val : buffer) {
      cout << val;
    }
    cout << endl;
  });

  server.start();
  return 0;
}