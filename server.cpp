#include "udpServer.h"

#include <iostream>
#include <thread>

using namespace std;

int main() {

  unique_ptr<UdpServer> server = nullptr;

  try {
    server = make_unique<UdpServer>("127.0.0.1", 5000);
    auto messageHandler = make_unique<MessageHandler>();
    messageHandler->registerCallback('a', [](vector<uint8_t> &&buffer) {
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

    messageHandler->registerCallback('b', [](vector<uint8_t> &&buffer) {
      (void)buffer;
      cout << "callback B\n";
    });

    server->setMessageHandler(move(messageHandler));

    cout << "press enter key to finish the program\n";
    char key = getchar();

    if (key) {
      server->stop();
    }

  } catch (const exception &e) {
    cout << e.what() << endl;
  }
  return 0;
}