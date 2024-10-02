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
  Replication(int p);
  int port;
  vector<int> slavePorts;
  void addSlave(string msg);
  void addSlaves(string msg);
  void connectToMaster(int connectToPort, int port, int nodeId);
private:
  char *masterIpAddress = "127.0.0.1";
  sockaddr_in masterSockAddress;
};