#include <applicationMessages.h>
#include <iostream>
#include <udpClient.h>

using namespace std;

int main() {
  UdpClient client("127.0.0.1", 5000);
  try {
    ApplicationMessage::Header messageHeader{'b', 5};
    std::vector<uint8_t> payload = {'h', 'e', 'l', 'l', 'o'};

    client.sendMessage(messageHeader.convertToBytes());

    client.sendMessage(move(payload));

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
