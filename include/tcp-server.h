#include <string>
#include <netinet/in.h>
#include "network-stream.h"

class TCPServer
{
  sockaddr_in s1Address;
  sockaddr_in s2Address;
  int c1Socket;
  int clientDescriptor;
  string ipAddress;
  int port;
  sockaddr_in clientAddress;
  socklen_t clientAddressLength = sizeof(clientAddress);

public:
  TCPServer(int port);
  ~TCPServer();

  NetworkStream *accept();
private:
};