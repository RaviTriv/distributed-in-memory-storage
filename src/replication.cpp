#include "../include/replication.h"
#include "../include/tcp-client.h"
#include <arpa/inet.h>
#include <netdb.h>

Replication::Replication(int p)
{
  port = p;
};

void Replication::connectToMaster(int connectToPort, int port, int nodeId)
{
  char *serverIpAddress = "127.0.0.1";
  TCPClient *connector = new TCPClient();
  NetworkStream *stream = connector->connect(connectToPort, serverIpAddress);
  char message[256];
  sprintf(message, "CONNECTED FROM SLAVE: %d %d", port, nodeId);
  stream->send(message, sizeof(message));
  delete stream;
};

void Replication::addSlave(string msg)
{
  string p = msg.substr(0, 4);
  slavePorts.push_back(stoi(p));
};

void Replication::addSlaves(string msg)
{
  while (msg.size() >= 5)
  {
    int sPort = atoi(msg.substr(0, 5).c_str());
    printf("RECIEVED SLAVE PORT:%d\n", atoi(msg.substr(0, 5).c_str()));
    if (sPort != port)
    {
      slavePorts.push_back(sPort);
    }
    msg = msg.substr(5, msg.size());
  }
};

int Replication::slavesExist()
{
  printf("SLAVE PORTS SIZE: %lu\n", slavePorts.size());
  return (slavePorts.size() > 0);
};

int Replication::slavePortsSize(){
  return slavePorts.size();
}

int Replication::getSlavePort(int index)
{
  return slavePorts.at(index);
}