#include "../include/replication.h"
#include "../include/tcp-client.h"
#include <arpa/inet.h>
#include <netdb.h>

Replication::Replication(int connectToPort, int port, int nodeId)
{
  char *serverIpAddress = "127.0.0.1";
  TCPClient *connector = new TCPClient();
  NetworkStream *stream = connector->connect(connectToPort, serverIpAddress);
  char message[256];
  sprintf(message, "CONNECTED FROM SLAVE: %d %d", port, nodeId);
  stream->send(message, sizeof(message));
  delete stream;
};

void Replication::connectToMaster() {

};