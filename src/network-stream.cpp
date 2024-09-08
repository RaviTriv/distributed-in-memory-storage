#include "../include/network-stream.h"
#include <arpa/inet.h>

NetworkStream::NetworkStream(int sd, struct sockaddr_in *addy)
{
  socketDescriptor = sd;
  char ip[50];
  inet_ntop(AF_INET, (struct sockaddr_in *)&(addy->sin_addr.s_addr), ip, sizeof(addy) - 1);
  socketIP = ip;
  socketPort = ntohs(addy->sin_port);
}

NetworkStream::~NetworkStream()
{
  close(socketDescriptor);
}

ssize_t NetworkStream::send(char *buffer, size_t len)
{
  return ::send(socketDescriptor, buffer, len, 0);
}

ssize_t NetworkStream::recieve(char *buffer, size_t len)
{
  return ::recv(socketDescriptor, buffer, len, 0);
}

int NetworkStream::getSocketPort()
{
  return socketPort;
}

string NetworkStream::getSocketIP()
{
  return socketIP;
}