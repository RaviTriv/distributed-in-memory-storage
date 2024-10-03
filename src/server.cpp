#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../include/tcp-server.h"
#include "./connection-thread.cpp"
#include <vector>
#include <sys/types.h>
#include <signal.h>

using namespace std;
char message[256];
char *serverIpAddress = "127.0.0.1";
vector<int> inputs;


int main(int argc, char *argv[])
{
  // PORT MASTER_PORT NODE_ID PERSISTENCE
  for(int i = 1; i < argc; i++){
    inputs.push_back(atoi(argv[i]));
  }

  Replication replica(inputs.at(0), inputs.at(2), inputs.at(3));

  if (inputs.at(2) != 1)
  {
    replica.connectToMaster(inputs.at(1), inputs.at(0), inputs.at(2));
  }

  char ipAddy[90];
  //allow us to set this from argv
  strcpy(ipAddy, "127.0.0.1");

  KeyValueStore store;
  WorkQueue<NetworkTask *> queue;
  for (int i = 0; i < 5; i++)
  {
    ConnectionThread *cThread = new ConnectionThread(queue, &store, &replica);
    cThread->start();
  }
  NetworkStream *stream = NULL;
  NetworkTask *task;
  TCPServer *tcpServerSocket = new TCPServer(inputs.at(0));

  while (1)
  {
    NetworkStream *connection = tcpServerSocket->accept();
    task = new NetworkTask(connection);
    queue.add(task);
  }

  delete stream;
}
