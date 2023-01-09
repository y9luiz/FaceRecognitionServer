#include "faceRecognitionUdpServer.h"
#include "udpServer.h"

#include <iostream>

using namespace std;

int main() {

  unique_ptr<UdpServer> server = nullptr;

  try {
    server = make_unique<FaceRecognitionUdpServer>("127.0.0.1", 5000);

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