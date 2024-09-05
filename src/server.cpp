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
char message[2100];

class ListenerThread : public Thread
{
  WorkQueue<Task *> &q;

public:
  ListenerThread(WorkQueue<Task *> &queue) : q(queue) {}

  void *run()
  {
    for (int i = 0;; i++)
    {
      Task *t = (Task *)q.remove();
    }
    return NULL;
  }
};

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

  WorkQueue<Task *>
      queue;
  Task *t1;
  Task *t2;

  // Thread Pool
  ListenerThread *thread1 = new ListenerThread(queue);
  ListenerThread *thread2 = new ListenerThread(queue);

  thread1->start();
  thread2->start();

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
