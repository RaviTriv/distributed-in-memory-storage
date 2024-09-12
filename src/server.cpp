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
#include "../include/network-task.h"
#include <sys/types.h>
#include <signal.h>

using namespace std;
int port = 4200;
int nodeId = 1;
char message[256];

class ConnectionThread : public Thread
{
  WorkQueue<NetworkTask *> &queue;
  DataPersistence backup;
  KeyValueStore store;

public:
  ConnectionThread(WorkQueue<NetworkTask *> &q) : queue(q) {}

  void *run()
  {
    for (int i = 0;; i++)
    {
      printf("Thread %lu WAITING\n", (long unsigned int)getThreadId());
      NetworkTask *task = queue.remove();
      printf("Thread %lu, RECIEVED\n", (long unsigned int)getThreadId());
      NetworkStream *stream = task->getStream();

      while ((stream->recieve((char *)&message, sizeof(message))) > 0)
      {
        string tempMessage = message;
        if (tempMessage.find("Persistence Write:") != string::npos)
        {
          tempMessage = tempMessage.substr(18, tempMessage.size() - 18);
          string k = tempMessage.substr(0, tempMessage.find(" "));
          string v = tempMessage.substr(tempMessage.find(" ") + 1, tempMessage.length() - k.length());

          pid_t cPid = fork();
          int status = 0;
          if (cPid == 0)
          {
            backup.write(k.c_str(), v.c_str());
            exit(0);
          }
          else
          {
            store.set(k.c_str(), v.c_str());
          }
        }
        printf("Thread %lu CLIENT SENT: %s\n", (long unsigned int)getThreadId(), message);
        memset(&message, 0, sizeof(message));
        strcpy(message, "TEST RESPONSE");
        stream->send(message, sizeof(message));
      }
      delete task;
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

  char ipAddy[90];
  strcpy(ipAddy, "127.0.0.1");

  // KeyValueStore store;

  WorkQueue<NetworkTask *> queue;
  for (int i = 0; i < 3; i++)
  {
    ConnectionThread *cThread = new ConnectionThread(queue);
    cThread->start();
  }

  NetworkStream *stream = NULL;
  NetworkTask *task;
  TCPServer *tcpServerSocket = new TCPServer(4200);

  while (1)
  {
    NetworkStream *connection = tcpServerSocket->accept();
    task = new NetworkTask(connection);
    queue.add(task);
  }
  delete stream;
}
