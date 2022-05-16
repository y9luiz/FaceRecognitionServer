#include <iostream>
#include "udpServer.h"

using namespace std;

int main()
{
  UdpServer server("127.0.0.1",5000);
  server.start();
  return 0;
}