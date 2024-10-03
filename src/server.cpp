#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// #include "../include/key-value.h"
// #include "../include/persistence.h"
// #include "../include/replication.h"
// #include "../include/thread.h"
// #include "../include/work-queue.h"
#include "../include/tcp-server.h"
// #include "../include/tcp-client.h"
// #include "../include/network-task.h"
#include "./connection-thread.cpp"
#include <vector>
#include <sys/types.h>
#include <signal.h>

using namespace std;
int port = 4200;
int masterPort = 4200;
int nodeId = 1;
int slaveToUse = 0;
int persistence = 0;
int slaveIndex = 0;
int slaveNodeId[5];
char message[256];
char *serverIpAddress = "127.0.0.1";
vector<int> inputs;
//Move this class probably
// class ConnectionThread : public Thread
// {
//   WorkQueue<NetworkTask *> &queue;
//   DataPersistence backup;
//   KeyValueStore *store;
//   Replication *replica;

// public:
//   ConnectionThread(WorkQueue<NetworkTask *> &q, KeyValueStore *s, Replication *r) : queue(q)
//   {
//     store = s;
//     replica = r;
//   }

//   void *run()
//   {
//     for (int i = 0;; i++)
//     {
//       NetworkTask *task = queue.remove();
//       NetworkStream *stream = task->getStream();

//       while ((stream->recieve((char *)&message, sizeof(message))) > 0)
//       {
//         string tempMessage = message;
//         if (tempMessage.find("NEW MASTER") != string::npos)
//         {
//           nodeId = 1;
//           tempMessage = tempMessage.substr(11, tempMessage.size() - 11);
//           replica->addSlaves(tempMessage);
//         }

//         if (tempMessage.find("CONNECTED FROM SLAVE:") != string::npos)
//         {
//           tempMessage = tempMessage.substr(22, tempMessage.size() - 22);
//           replica->addSlave(tempMessage);
//         }

//         // Basic write message, TODO:implement fork
//         if (tempMessage.find("Write:") != string::npos)
//         {
//           tempMessage = tempMessage.substr(6, tempMessage.size() - 6);
//           string k = tempMessage.substr(0, tempMessage.find(" "));
//           string v = tempMessage.substr(tempMessage.find(" ") + 1, tempMessage.length() - k.length());

//           if (persistence == 1)
//           {
//             pid_t cPid = fork();
//             if (cPid == 0)
//             {
//               backup.write(k.c_str(), v.c_str());
//               exit(0);
//             }
//             else
//             {
//               store->set(k.c_str(), v.c_str());
//             }
//           }
//           else
//           {
//             store->set(k.c_str(), v.c_str());
//           }
//           printf("BEFORE SLAVES EXIST\n");
//           if (replica->slavesExist())
//           {
//             printf("INSIDE SLAVES EXIST\n");
//             pid_t cPid = fork();
//             if (cPid == 0)
//             {
//               for (int i = 0; i < replica->slavePortsSize(); i++)
//               {
//                 if (replica->getSlavePort(i) != port)
//                 {
//                   char *serverIpAddress = "127.0.0.1";

//                   TCPClient *connector = new TCPClient();
//                   NetworkStream *stream2 = connector->connect(replica->slavePorts.at(i), serverIpAddress);
//                   stream2->send(message, sizeof(message));
//                   delete stream2;
//                 }
//               }
//               exit(0);
//             }
//           }
//         }

//         if (tempMessage.find("Read:") != string::npos)
//         {
//           tempMessage = tempMessage.substr(5, tempMessage.size() - 5);
//           string k = tempMessage.substr(0, tempMessage.find(" "));

//           char t[256];
//           printf("%s\n", k.c_str());
//           strcpy(t, store->get(k).c_str());
//           printf("SERVICED FROM SERVER ON PORT: %d, Value: %s\n", port, t);
//           stream->send(t, sizeof(t));
//         }

//         printf("Thread %lu CLIENT SENT: %s\n", (long unsigned int)getThreadId(), message);
//         memset(&message, 0, sizeof(message));
//         strcpy(message, "TEST RESPONSE");
//         stream->send(message, sizeof(message));
//       }
//       delete task;
//     }
//     return NULL;
//   }
// };

int main(int argc, char *argv[])
{
  // PORT MASTER_PORT NODE_ID PERSISTENCE
  for(int i = 1; i < argc; i++){
    inputs.push_back(atoi(argv[i]));
  }

  if(inputs.size() >= 1){
    port = inputs.at(0);
  }
  if (inputs.size() >= 2)
  {
    masterPort = inputs.at(1);
  }
  if (inputs.size() >= 3)
  {
    nodeId = inputs.at(2);
  }
  if (inputs.size() >= 4)
  {
    persistence = inputs.at(3);
  }

  Replication replica(port, nodeId, persistence);

  if (nodeId != 1)
  {
    replica.connectToMaster(masterPort, port, nodeId);
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
  TCPServer *tcpServerSocket = new TCPServer(port);

  while (1)
  {
    NetworkStream *connection = tcpServerSocket->accept();
    task = new NetworkTask(connection);
    queue.add(task);
  }

  delete stream;
}
