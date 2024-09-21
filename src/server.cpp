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
int slaveToUse = 0;
int persistence = 0;
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
  KeyValueStore *store;

public:
  ConnectionThread(WorkQueue<NetworkTask *> &q, KeyValueStore *s) : queue(q)
  {
    store = s;
  }

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

        for (int i = 0; i < sizeof(slavePorts); i++)
        {
          printf("SLAVE PORT: %d\n", slavePorts[i]);
        }

        if (tempMessage.find("NEW MASTER") != string::npos)
        {
          nodeId = 1;
          // Update slavePorts
          // Update slaveNodeIds
        }

        if (tempMessage.find("CONNECTED FROM SLAVE:") != string::npos)
        {
          tempMessage = tempMessage.substr(22, tempMessage.size() - 22);
          string p = tempMessage.substr(0, 4);
          string nId = tempMessage.substr(5, 6);
          slavePorts[slaveIndex] = stoi(p);
          slaveNodeId[slaveIndex] = stoi(nId);
          slaveIndex++;
        }

        // Basic write message, TODO:implement fork
        if (tempMessage.find("Write:") != string::npos)
        {
          tempMessage = tempMessage.substr(6, tempMessage.size() - 6);
          string k = tempMessage.substr(0, tempMessage.find(" "));
          string v = tempMessage.substr(tempMessage.find(" ") + 1, tempMessage.length() - k.length());

          if (persistence == 1)
          {
            pid_t cPid = fork();
            if (cPid == 0)
            {
              backup.write(k.c_str(), v.c_str());
              exit(0);
            }
            else
            {
              store->set(k.c_str(), v.c_str());
            }
          }
          else
          {
            store->set(k.c_str(), v.c_str());
          }

          if (slaveIndex > 0)
          {
            pid_t cPid = fork();
            if (cPid == 0)
            {
              for (int i = 0; i < slaveIndex; i++)
              {
                char *serverIpAddress = "127.0.0.1";

                TCPClient *connector = new TCPClient();
                NetworkStream *stream2 = connector->connect(slavePorts[i], serverIpAddress);
                stream2->send(message, sizeof(message));
                delete stream2;
              }
              exit(0);
            }
          }
        }

        if (tempMessage.find("Read:") != string::npos)
        {
          tempMessage = tempMessage.substr(5, tempMessage.size() - 5);
          string k = tempMessage.substr(0, tempMessage.find(" "));

          if (nodeId == 1 && slaveIndex > 0)
          {
            TCPClient *connector = new TCPClient();
            NetworkStream *stream2 = connector->connect(slavePorts[slaveToUse], serverIpAddress);
            stream2->send(message, sizeof(message));
            if (slaveToUse >= 4)
            {
              slaveToUse = 0;
            }
            else
            {
              slaveToUse++;
            }
            delete stream2;
          }
          else
          {
            char t[256];
            printf("%s\n", k.c_str());
            strcpy(t, store->get(k).c_str());
            printf("SERVICED FROM SERVER ON PORT: %d, Value: %s\n", port, t);
            // if sending from replica, we need to send from replica server to client
            stream->send(t, sizeof(t));
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
  if (argc >= 3)
  {
    port = atoi(argv[1]);
    nodeId = atoi(argv[2]);
    persistence = atoi(argv[3]);
  }

  if (nodeId != 1)
  {
    // IMPLEMENT NEW MASTER IF EXISTING MASTER GOES DOWN
    Replication replica(4200, nodeId);
  }

  char ipAddy[90];
  strcpy(ipAddy, "127.0.0.1");

  KeyValueStore store;
  WorkQueue<NetworkTask *> queue;
  for (int i = 0; i < 5; i++)
  {
    ConnectionThread *cThread = new ConnectionThread(queue, &store);
    cThread->start();
  }
  NetworkStream *stream = NULL;
  NetworkTask *task;
  TCPServer *tcpServerSocket = new TCPServer(port);

  while (1)
  {
    NetworkStream *connection = tcpServerSocket->accept();
    task = new NetworkTask(connection);
    queue.add(task);
  }
  delete stream;
}
