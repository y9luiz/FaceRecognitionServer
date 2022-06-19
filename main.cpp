#include "udpServer.h"
#include <iostream>

using namespace std;

int main() {
  UdpServer server("127.0.0.1", 5000);

  auto messageHandler = std::make_unique<MessageHandler>();
  messageHandler->registerCallback('a',[](std::vector<uint8_t> && buffer){
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

  messageHandler->registerCallback('b',[](std::vector<uint8_t> && buffer){
        (void) buffer;
        cout << "callback B\n";
  });

  server.setMessageHandler(std::move(messageHandler));

  server.start();
  return 0;
}