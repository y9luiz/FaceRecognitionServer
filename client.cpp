#include <faceRecognitionUdpClient.h>

#include <stdexcept>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  FaceRecognitionUdpClient client("127.0.0.1", 5000);
  try {
    client.run(argc, argv);
  } 
  catch (const exception &e) {
    cout << e.what() << endl;
  }

  return 0;
}
