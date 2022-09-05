#include <applicationMessages.h>
#include <iostream>
#include <thread>
#include <udpClient.h>

using namespace std;
using namespace chrono_literals;

int main() {
  UdpClient client("127.0.0.1", 5000);
  try {
    vector<uint8_t> messageData =
        ApplicationMessage('b', 5, {'h', 'e', 'l', 'l', 'o'}).convertToBytes();

    client.sendMessage(move(messageData));

  } catch (const exception &e) {
    cout << e.what() << endl;
  }

  return 0;
}
