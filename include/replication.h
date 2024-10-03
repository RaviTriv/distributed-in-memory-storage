#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstdlib>
#include <vector>

using namespace std;

class Replication
{
public:
  Replication(int p, int nId, int per);
  int port;
  int nodeId;
  int persistence;
  vector<int> slavePorts;
  void setNodeId(int nId);
  int getNodeId();
  void addSlave(string msg);
  void addSlaves(string msg);
  void connectToMaster(int connectToPort, int port, int nodeId);
  int slavesExist();
  int slavePortsSize();
  int getSlavePort(int index);

private:
  char *masterIpAddress = "127.0.0.1";
  sockaddr_in masterSockAddress;
};