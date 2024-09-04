#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class TcpServer
{
public:
  TcpServer(int port, int node);
  //~TcpServer();
  int clientDescriptor;

private:
  sockaddr_in c1Address;
  sockaddr_in c2Address;
  int node;
  int c1Socket;
  sockaddr_in clientAddress;
  socklen_t clientAddressLength = sizeof(clientAddress);
};
