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
#include "../include/task.h"

using namespace std;
int port = 4200;
int nodeId = 1;
char message[256];

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

  char ipAddy[90];
  strcpy(ipAddy, "127.0.0.1");

  KeyValueStore store;
  DataPersistence backup;
  NetworkStream *stream = NULL;
  TCPServer *tcpServerSocket = new TCPServer(4200);


  stream = tcpServerSocket->accept();
  printf("CLIENT CONNECTED\n");
  memset(&message, 0, sizeof(message));
  stream->recieve((char *)&message, sizeof(message));
  printf("RECIEVED: %s\n", message);
  memset(&message, 0, sizeof(message));
  strcpy(message, "TEST RESPONSE");

  stream->send(message, sizeof(message));
  delete stream;
}
