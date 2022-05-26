#include "udpServer.h"
#include <iostream>

using namespace std;

int main() {
  UdpServer server("127.0.0.1", 5000);
  server.setProcessMessageCallback([](std::vector<uint8_t> &buffer) {
    const auto threadId = this_thread::get_id();
    for (int i = 0; i < 10; i++) {
      cout << "Thread id " << threadId << " i = " << i << endl;
      this_thread::sleep_for(chrono::seconds(1));
    }

    for (const auto &val : buffer) {
      cout << val;
    }
    cout << endl;
  });

  server.start();
  return 0;
}