#pragma  once
#include <boost/asio.hpp>
#include <queue>
#include <vector>

class UdpServer
{
public:

  UdpServer(const std::string  &url, uint16_t port);
  ~UdpServer();

  void start();
  void stop();
protected:
  std::string m_url;
  uint16_t m_port;

  boost::asio::io_context m_ioContext;
  boost::asio::ip::udp::socket m_socket;
};