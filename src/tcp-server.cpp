#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include "../include/tcp-server.h"
#include <unistd.h>

using namespace std;

TcpServer::TcpServer(int port, int nodeId)
{
  node = nodeId;
  memset(&c1Address, '\0', sizeof(c1Address));
  c1Address.sin_family = AF_INET;
  c1Address.sin_addr.s_addr = htonl(INADDR_ANY);
  c1Address.sin_port = htons(port);
  c1Socket = socket(AF_INET, SOCK_STREAM, 0);
  bind(c1Socket, (struct sockaddr *)&c1Address, sizeof(c1Address));
}

void TcpServer::startListenting()
{
  listen(c1Socket, 5);
  clientDescriptor = accept(c1Socket, (sockaddr *)&clientAddress, &clientAddressLength);
}

void TcpServer::stopListenting()
{
  close(clientDescriptor);
  close(c1Socket);
}
