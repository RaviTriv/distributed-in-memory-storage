#include "../include/replication.h"
#include <arpa/inet.h>
#include <netdb.h>

Replication::Replication()
{
  struct hostent *host = gethostbyname(masterIpAddress);
  memset(&masterSockAddress, '\0', sizeof(masterSockAddress));
  masterSockAddress.sin_family = AF_INET;
  masterSockAddress.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)*host->h_addr_list));
  masterSockAddress.sin_port = htons(4200);

  int slave = socket(AF_INET, SOCK_STREAM, 0);
  connect(slave, (sockaddr *)&masterSockAddress, sizeof(masterSockAddress));
  char message[2100];
  strcpy(message, "CONNECT FROM SLAVE");
  send(slave, (char *)&message, strlen(message), 0);
};

void Replication::connectToMaster() {

};