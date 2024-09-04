#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../include/key-value.h"
#include "../include/persistence.h"
#include "../include/replication.h"
#include "../include/thread.h"
#include "../include/work-queue.h"
#include "../include/tcp-server.h"

using namespace std;
int port = 4200;
int nodeId = 1;
char message[2100];

int main(int argc, char *argv[])
{
  if (argc >= 2)
  {
    port = atoi(argv[1]);
    nodeId = atoi(argv[2]);
  }

  if (nodeId == 0)
  {
    Replication replica;
  }

  KeyValueStore store;
  DataPersistence backup;
  TcpServer TcpServer(port, nodeId);
  TcpServer.startListenting();
  while (true)
  {

    memset(&message, 0, sizeof(message));
    recv(TcpServer.clientDescriptor, (char *)&message, sizeof(message), 0);

    cout << "Client: " << message << endl;

    if (strcmp(message, "QUIT") == 0)
    {
      cout << "ENDING SESSION" << endl;
      send(TcpServer.clientDescriptor, (char *)"QUIT", strlen(message), 0);
      TcpServer.stopListenting();
      exit(0);
    }

    memset(&message, 0, sizeof(message));
    strcpy(message, "TEST RESPONSE");
    send(TcpServer.clientDescriptor, (char *)&message, strlen(message), 0);
  }
}
