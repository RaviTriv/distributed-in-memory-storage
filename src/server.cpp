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
#include "../include/tcp-client.h"
#include "../include/network-task.h"
#include <sys/types.h>
#include <signal.h>

using namespace std;
int port = 4200;
int nodeId = 1;
int slaveIndex = 0;
int slavePorts[5];
int slaveNodeId[5];
char message[256];
int replicaExists = 0;
char *serverIpAddress = "127.0.0.1";

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
      // on read request check if replicas exist
      // redirect request to replic
      // once request is written, loop through replicas and update

      printf("Thread %lu WAITING\n", (long unsigned int)getThreadId());
      NetworkTask *task = queue.remove();
      printf("Thread %lu, RECIEVED\n", (long unsigned int)getThreadId());
      NetworkStream *stream = task->getStream();

      while ((stream->recieve((char *)&message, sizeof(message))) > 0)
      {
        string tempMessage = message;

        // Recieve connection from slave
        if (tempMessage.find("CONNECTED FROM SLAVE:") != string::npos)
        {
          tempMessage = tempMessage.substr(22, tempMessage.size() - 22);
          string p = tempMessage.substr(0, 4);
          string nId = tempMessage.substr(5, 6);
          slavePorts[slaveIndex] = stoi(p);
          slaveNodeId[slaveIndex] = stoi(nId);
          slaveIndex++;
        }

        // TODO: Move Persistence Write to approriate place
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
            if (slaveIndex > 0)
            {
              for (int i = 0; i < slaveIndex; i++)
              {
                char *serverIpAddress = "127.0.0.1";

                char t[256];
                TCPClient *connector = new TCPClient();
                printf("PORT: %d\n", slavePorts[i]);
                NetworkStream *stream2 = connector->connect(slavePorts[i], serverIpAddress);
                strcpy(t, "SENT FROM MASTER TO SLAVE");
                stream2->send(t, sizeof(t));
                delete stream2;
              }
            }
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

  if (nodeId != 1)
  {
    Replication replica(port, nodeId);
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
