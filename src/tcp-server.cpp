#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "../include/tcp-server.h"

TCPServer::TCPServer(int p)
{
  memset(&s1Address, '\0', sizeof(s1Address));

  s1Address.sin_family = AF_INET;
  s1Address.sin_addr.s_addr = htonl(INADDR_ANY);
  s1Address.sin_port = htons(p);

  c1Socket = socket(AF_INET, SOCK_STREAM, 0);
  bind(c1Socket, (struct sockaddr *)&s1Address, sizeof(s1Address));
  listen(c1Socket, 5);
}

TCPServer::~TCPServer()
{
  if (clientDescriptor > 0)
  {
    close(clientDescriptor);
  }
}

NetworkStream *TCPServer::accept()
{
  clientDescriptor = ::accept(c1Socket, (sockaddr *)&clientAddress, &clientAddressLength);

  if (clientDescriptor < 0)
  {
    perror("FAILED TO CONNECT TO SERVER");
    return NULL;
  }

  return new NetworkStream(clientDescriptor, &s2Address);
}