#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstdlib>

using namespace std;

class Replication
{
public:
  Replication();
  void connectToMaster();

private:
  char *masterIpAddress = "127.0.0.1";
  sockaddr_in masterSockAddress;
};