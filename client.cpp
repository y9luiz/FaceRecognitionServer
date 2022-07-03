#include <iostream>
#include <udpClient.h>

using namespace std;

int main() {
  UdpClient client("127.0.0.1", 5000);
  try {
    client.sendMessage({'b', 'h', 'e', 'l', 'l', 'o'});

    auto receivedMessage = client.receiveMessage();

    for (const auto &c : receivedMessage) {
      cout << c;
    }

    cout << endl;
  } catch (const exception &e) {
    cout << e.what() << endl;
  }

  return 0;
}
