#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "../include/tcp-client.h"

TCPClient::TCPClient() {};

NetworkStream *
TCPClient::connect(int port, const char *s)
{
  struct hostent *host = gethostbyname(s);
  sockaddr_in serverSockAddress;
  memset(&serverSockAddress, '\0', sizeof(serverSockAddress));

  serverSockAddress.sin_family = AF_INET;
  serverSockAddress.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)*host->h_addr_list));
  serverSockAddress.sin_port = htons(port);

  int client = socket(AF_INET, SOCK_STREAM, 0);
  int res = ::connect(client, (sockaddr *)&serverSockAddress, sizeof(serverSockAddress));

  printf("RES: %d\n", res);
  return new NetworkStream(client, &serverSockAddress);
}
